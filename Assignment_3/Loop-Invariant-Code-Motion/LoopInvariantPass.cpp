#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Dominators.h"
#include <set>
#include <map>

using namespace llvm;

bool isLoopInvariant( Instruction *Instr , Loop *loop) {
   if(dyn_cast<ReturnInst>(Instr)) return false;
   if(dyn_cast<StoreInst>(Instr)) return false;
   if(dyn_cast<LoadInst>(Instr)) return false;
   if(dyn_cast<PHINode>(Instr)) return false;
   if(dyn_cast<BranchInst>(Instr)) return false;
   if(dyn_cast<CallInst>(Instr)) return false;

   for( auto it=Instr->op_begin(); it != Instr->op_end(); ++it ) {
      auto def=dyn_cast<Instruction>(*it);
      auto constOp=dyn_cast<Constant>(*it);
      auto argument=dyn_cast<Argument>(*it);

      if( !def && !constOp && !argument ) return false;
      if(dyn_cast<PHINode>(*it)) return false;
      if(dyn_cast<LoadInst>(*it)) return false;
      if(dyn_cast<StoreInst>(*it)) return false;
      if(dyn_cast<CallInst>(*it)) return false;

      if( !argument && !constOp && loop->contains(def) && !def->hasMetadata("LoopInvariant") )
         return false;

   }

   LLVMContext &C = Instr->getContext();
   auto *LoopInvMD = MDNode::get(C, MDString::get(C, "loopinvariant"));
   Instr->setMetadata("LoopInvariant", LoopInvMD);
   return true;
}

bool isSafeToHoist(Instruction *Instr, Loop *L, DominatorTree &DT) {
    BasicBlock *BB = Instr->getParent();
    BasicBlock *Header = L->getHeader();

    // L'istruzione può essere spostata solo se il suo blocco è dominato dall'header del loop
    return DT.dominates(Header, BB);
}

bool preheaderDominatesAllExits(Loop *L, DominatorTree &DT) {
    BasicBlock *PreHeader = L->getLoopPreheader();
    if (!PreHeader)
        return false; // Non c'è un preheader → non puoi spostare codice

    // Trova tutte le uscite del loop
    SmallVector<BasicBlock *, 8> ExitBlocks;
    L->getExitBlocks(ExitBlocks);

    // Controlla se il PreHeader domina tutte le uscite
    for (BasicBlock *ExitBB : ExitBlocks) {
        if (!DT.dominates(PreHeader, ExitBB)) {
            return false; // Se anche una sola uscita non è dominata → non va bene
        }
    }

    return true; // Ok, il PreHeader domina tutte le uscite
}


SmallVector<BasicBlock *, 8> getLoopExitBlocks(Loop *L) {
    SmallVector<BasicBlock *, 8> exitBlocks;

    for (BasicBlock *BB : L->blocks()) {
        for (BasicBlock *Succ : successors(BB)) {
            if (!L->contains(Succ)) {
                exitBlocks.push_back(Succ);
            }
        }
    }

    return exitBlocks;
}

bool isDeadOutsideLoop(Instruction *instr, Loop *loop) {
    bool result = true;
    
    for(auto user = instr->use_begin(); user != instr->use_end() && result; ++user) {
        Instruction *instr = dyn_cast<Instruction>(&*user);
        if(!loop->contains(instr))
            result = false;
    }
    
    return result;
}

bool isBlockValidForCodeMotion(Instruction *I, Loop *L, DominatorTree *DT) {
    BasicBlock *instBlock = I->getParent();
    BasicBlock *preheader = L->getLoopPreheader();

    // Controlla che il blocco dell'istruzione sia dominato dal preheader
    if (!DT->dominates(preheader, instBlock))
        return false;

    // Ottieni i blocchi di uscita
    SmallVector<BasicBlock *, 8> exitBlocks = getLoopExitBlocks(L);

    // Verifica che il blocco dell'istruzione domini tutte le uscite
    for (BasicBlock *exit : exitBlocks) {
        if (!DT->dominates(instBlock, exit))
            return false;
    }

    return true;
}

bool dominatesAllUses(Instruction *Instr, Loop *L, DominatorTree &DT) {
    BasicBlock *defBB = Instr->getParent();

    for (User *U : Instr->users()) {
        if (Instruction *UI = dyn_cast<Instruction>(U)) {
            if (!L->contains(UI)) continue;    // uso fuori dal loop non conta
            BasicBlock *useBB = UI->getParent();
            if (!DT.dominates(defBB, useBB))
                return false;
        }
    }
    return true;
}

bool isNotRedefinedInLoop(Instruction *Instr, Loop *L) {
    // Per le istruzioni store, verifichiamo che la stessa memoria non venga modificata altrove
    if (StoreInst *Store = dyn_cast<StoreInst>(Instr)) {
        Value *Ptr = Store->getPointerOperand();
        
        for (BasicBlock *BB : L->blocks()) {
            for (auto &I : *BB) {
                if (&I == Instr) continue;
                
                // Controlla altri store
                if (StoreInst *OtherStore = dyn_cast<StoreInst>(&I)) {
                    if (OtherStore->getPointerOperand() == Ptr) {
                        return false;  // Un altro store modifica la stessa memoria
                    }
                }
            }
        }
        return true;
    }
    
    // Per altre istruzioni in SSA, ogni definizione è già unica
    // ma dobbiamo assicurarci che non ci siano istruzioni che potrebbero
    // modificare indirettamente il valore (attraverso puntatori)
    
    // Se l'istruzione definisce un valore
    if (!Instr->getType()->isVoidTy()) {
        // In SSA, ogni valore è definito una sola volta, quindi non può essere ridefinito
        // Ma dobbiamo verificare che non ci siano altre istruzioni che modificano
        // la stessa memoria attraverso puntatori
        
        // Questo è un controllo semplificato, in pratica servirebbe un'analisi di alias
        return true;
    }
    
    return true;
}

static void dfsLoopBlocks(BasicBlock *BB,Loop *L,SmallPtrSetImpl<BasicBlock*> &Vis,SmallVectorImpl<BasicBlock*> &Order) {

  Vis.insert(BB);
   for (BasicBlock *Succ : successors(BB)) {
    if (L->contains(Succ) && !Vis.count(Succ))
       dfsLoopBlocks(Succ, L, Vis, Order);
    }
   Order.push_back(BB);  // post‐order: i figli prima del padre
}

static bool allDefsMoved(Instruction *I,Loop *L,SmallPtrSetImpl<Instruction*> &Moved) {

    for (Value *Op : I->operands()) {
        auto *Def = dyn_cast<Instruction>(Op);
        if (Def && L->contains(Def) && !Moved.count(Def))
            return false;
    }

  return true;
}

struct LoopInvariantPass : PassInfoMixin<LoopInvariantPass> { 

    PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
        LoopInfo &LI = AM.getResult<LoopAnalysis>(F);
        DominatorTree &DT = AM.getResult<DominatorTreeAnalysis>(F);

        for (Loop *L : LI) {
            BasicBlock *Preheader = L->getLoopPreheader();
            if (!Preheader) continue;

            SmallPtrSet<BasicBlock *, 16> Visited;
            SmallVector<BasicBlock *, 16> LoopOrder;
            dfsLoopBlocks(L->getHeader(), L, Visited, LoopOrder);

            SmallPtrSet<Instruction *, 16> Moved;

            for (BasicBlock *BB : LoopOrder) {
                for (auto &I : *BB) {
                    Instruction *Instr = &I;

                    if (isLoopInvariant(Instr, L) &&
                        allDefsMoved(Instr, L, Moved) &&
                        (isBlockValidForCodeMotion(Instr, L, &DT) ||
                         isDeadOutsideLoop(Instr, L)) &&
                        dominatesAllUses(Instr, L, DT) &&
                        isNotRedefinedInLoop(Instr, L)) {

                        Instr->setMetadata("LoopInvariant", nullptr);
                        outs() << "Istruzione su cui fare la code-motion : " << *Instr << "\n";
                        Moved.insert(Instr);
                    }
                }
            }

            for (Instruction *Instr : Moved) {
                Instr->moveBefore(Preheader->getTerminator());
                outs() << "Istruzione spostata : " << *Instr << "\n";
            }

        }

        return PreservedAnalyses::all();
    }

  static bool isRequired() { return true; }
};

// Plugin registration
llvm::PassPluginLibraryInfo getLoopInvariantPassPluginInfo() {
  return {
    LLVM_PLUGIN_API_VERSION, "LoopInvariantPass", LLVM_VERSION_STRING,
    [](PassBuilder &PB) {
      PB.registerPipelineParsingCallback(
        [](StringRef Name, FunctionPassManager &FPM,
           ArrayRef<PassBuilder::PipelineElement>) {
          if (Name == "LoopInvariantPass") {
            FPM.addPass(LoopInvariantPass());
            return true;
          }
          return false;
        });
    }
  };
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getLoopInvariantPassPluginInfo();
}

