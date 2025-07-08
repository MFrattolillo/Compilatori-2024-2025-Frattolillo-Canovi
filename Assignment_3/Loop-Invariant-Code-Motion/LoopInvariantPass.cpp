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

    if (!preheader) {
        errs() << "Loop has NO preheader\n";
        return false;
    }
    if (!DT->dominates(preheader, instBlock)) {
        errs() << "Preheader (" << *preheader << ") does NOT dominate instruction block (" << *instBlock << ")\n";
        return false;
    }

    SmallVector<BasicBlock *, 8> exitBlocks = getLoopExitBlocks(L);

    for (BasicBlock *exit : exitBlocks) {
        if (!DT->dominates(instBlock, exit)) {
            return false;
        }
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
        if (Def && L->contains(Def) && !Moved.count(Def)){
            errs() << "Definizione non ancora spostata: " << *Def << "\n";
            return false;
        }
            
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
                        dominatesAllUses(Instr, L, DT)) {

                        
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
