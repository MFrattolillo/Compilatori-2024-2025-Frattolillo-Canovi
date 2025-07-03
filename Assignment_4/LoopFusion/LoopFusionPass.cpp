#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/InstrTypes.h"
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/Analysis/ScalarEvolution.h>
#include <llvm/ADT/SmallSet.h>
#include "llvm/IR/Dominators.h"
#include "llvm/Analysis/PostDominators.h"
#include <llvm/IR/Module.h>
#include "llvm/IR/CFG.h"
#include "llvm/IR/InstrTypes.h"     
#include "llvm/IR/Instructions.h"
#include <llvm/Transforms/Utils/LoopSimplify.h>
#include <llvm/Transforms/Scalar/LoopRotation.h>
#include <llvm/Analysis/DependenceAnalysis.h>
#include <optional>
#include <llvm/Transforms/Scalar/IndVarSimplify.h>


#include <cmath>
#include <stdint.h>

using namespace llvm;

bool isGuarded(Loop *L) {
    BasicBlock *Header = L->getHeader();
    Instruction *TI = Header->getTerminator();

    // Verifica se l'istruzione finale è un branch condizionale
    if (BranchInst *br = dyn_cast<BranchInst>(TI)) {
        if (br->isConditional()) {
            // Se uno dei successori del branch NON è nel loop, allora è guarded
            for (unsigned i = 0; i < br->getNumSuccessors(); ++i) {
                if (!L->contains(br->getSuccessor(i))) {
                    return true;  // exit prima del corpo → guarded
                }
            }
        }
    }

    return false;  // Non condizionale o entrambi i successori sono nel loop → non guarded
}

bool isAdjacent( Loop *a , Loop *b ){
   
   if( !a || !b )
     return false;

   BasicBlock *eb = a->getExitBlock();
   if( !eb ) return false;

   SmallVector<BasicBlock *, 4> exits;
   a->getExitBlocks(exits);
   
   BasicBlock *Preheader = b->getLoopPreheader();
   
    bool combacia = false;
    for( BasicBlock *bb : exits ){
      if( bb == Preheader){
        combacia = true;
        break;
      }
    }

    if( !combacia )
     return false;

    if( Preheader->size() >= 2)
     return false;

    Instruction *instr = nullptr;
     for (auto &I : *Preheader) {
        instr = &I;
        break; // solo la prima
      }

     BranchInst *br = dyn_cast<BranchInst>(instr);
     if( !br || br->getNumSuccessors() != 1 )
      return false;

     return (br->getSuccessor(0) == b->getHeader());
}

bool isScalEv(Loop *A, Loop *B, ScalarEvolution &SCE) {
    const SCEV *tripA = SCE.getBackedgeTakenCount(A);
    const SCEV *tripB = SCE.getBackedgeTakenCount(B);

    if (isa<SCEVCouldNotCompute>(tripA) || isa<SCEVCouldNotCompute>(tripB)) {
        /*outs() << "Trip count non calcolabile\n";*/
        return false;
    }

    if (tripA == tripB) {
        /*outs() << "Trip count simbolicamente uguali\n";*/
        return true;
    }

    /*outs() << "Trip count diversi\n";*/
    return false;
}

bool controlFlowEq(Loop *L0, Loop *L1, DominatorTree &DT, PostDominatorTree &PDT) {
  BasicBlock *Header0 = L0->getHeader();
  BasicBlock *Header1 = L1->getHeader();

  return DT.dominates(Header0, Header1) && PDT.dominates(Header1, Header0);
}

//Funzione che controlla se ci sono dipendenze tra le istruzioni di loop1 con istruzioni di loop0
bool hasDependence(Loop *L0, Loop *L1, DependenceInfo &DI) {
  SmallVector<Instruction *, 8> LSInsts0;
  SmallVector<Instruction *, 8> LSInsts1;

  for (BasicBlock *BB : L0->blocks()) {
    for (Instruction &I : *BB) {
      if (isa<LoadInst>(&I) || isa<StoreInst>(&I))
        LSInsts0.push_back(&I);
    }
  }

  for (BasicBlock *BB : L1->blocks()) {
    for (Instruction &I : *BB) {
      if (isa<LoadInst>(&I) || isa<StoreInst>(&I))
        LSInsts1.push_back(&I);
    }
  }

  for (Instruction *I0 : LSInsts0) {
    for (Instruction *I1 : LSInsts1) {
      // Ignora dipendenze tra due load (solo letture)
      if (isa<LoadInst>(I0) && isa<LoadInst>(I1))
        continue;

      if (DI.depends(I0, I1, /*exact=*/true)) {
        errs() << "Dipendenza trovata tra " << *I0 << " e " << *I1 << "\n";
        return true;
      }
    }
  }

  return false;
}


/*void loopFusion(Loop *a, Loop *b, ScalarEvolution &SCE) {
    auto *header_a = a->getHeader();
    auto *header_b = b->getHeader();

    auto &blocks_a = a->getBlocksVector();
    auto &blocks_b = b->getBlocksVector();

    auto *latch_a = a->getLoopLatch();
    auto *latch_b = b->getLoopLatch();

    auto *preheader_a = a->getLoopPreheader();
    auto *preheader_b = b->getLoopPreheader();

    PHINode *indvar_a = a->getCanonicalInductionVariable();
    PHINode *indvar_b = b->getCanonicalInductionVariable();

    auto *exitBlock_a  = a->getExitBlock();
    auto *exitBlock_b = b->getExitBlock();

    auto *aLoopG = a->getLoopGuardBranch();
    auto *bLoopG = b->getLoopGuardBranch();

    if (!indvar_a || !indvar_b) {
        errs() << "Errore: induction variable non riconosciute\n";
        return;
    }

   for (BasicBlock *BB : blocks_b) {
        for (Instruction &I : *BB) {
            for (unsigned i = 0; i < I.getNumOperands(); ++i) {
                if (I.getOperand(i) == indvar_b) {
                    I.setOperand(i, indvar_a);
                }
            }
        }
    }

    // Elimina indvar_b se non usata
    if (indvar_b->use_empty()) {
        indvar_b->eraseFromParent();
    }

}*/

/*void loopFusion(Loop *a, Loop *b, ScalarEvolution &SCE) {
    auto *header_a = a->getHeader();
    auto *header_b = b->getHeader();

    auto &blocks_a = a->getBlocksVector();
    auto &blocks_b = b->getBlocksVector();

    auto *latch_a = a->getLoopLatch();
    auto *latch_b = b->getLoopLatch();

    auto *preheader_a = a->getLoopPreheader();
    auto *preheader_b = b->getLoopPreheader();
    

    PHINode *indvar_a = a->getCanonicalInductionVariable();
    PHINode *indvar_b = b->getCanonicalInductionVariable();

    auto *exitBlock_a  = a->getExitBlock();
    auto *exitBlock_b = b->getExitBlock();

    auto *aLoopG = a->getLoopGuardBranch();
    auto *bLoopG = b->getLoopGuardBranch();

    indvar_b->replaceAllUsesWith(indvar_a);
    indvar_b->eraseFromParent();
    
    BasicBlock *lastBlockA = nullptr;
    for (BasicBlock *pred : predecessors(latch_a)) {
       if (a->contains(pred)) {
        lastBlockA = pred;
        break; // puoi estendere se ci sono più candidati
      }
     }

     auto *instr = lastBlockA->getTerminator();
     auto *br = dyn_cast<BranchInst>(instr);
     br->setSuccessor(0, header_b);

     auto *br_b = dyn_cast<BranchInst>(latch_b->getTerminator());
     br_b->setSuccessor(0, header_a);

     //ciclo sul blocco di latch_a per trovare l'istruzione che aggiorna l'indvar
     Instruction *phi;
     for( Instruction &I : *latch_b ){
        if( auto *binOp = dyn_cast<BinaryOperator>(&I) ){
            if( binOp->getOperand(0) == indvar_a || binOp->getOperand(1) == indvar_a ){
                phi = &I;
            }
        }
     }

    indvar_a->removeIncomingValue(latch_a,  false);
    indvar_a->addIncoming(phi, latch_b);
    header_a->removePredecessor(latch_a, false);
    if (latch_a->use_empty()) {
  // nessuna istruzione fa più riferimento a latch_a
} else {
  // c'è almeno un use
  unsigned n = latch_a->getNumUses();
  errs() << "latch_a ha " << n << " use ancora aperti\n";
}

}*/

//Funzione che restituisce il blocco body di un loop
BasicBlock *getLoopBody(Loop *L) {
  BasicBlock *Header   = L->getHeader();
  BasicBlock *Latch = L->getLoopLatch();
  BasicBlock *Body  = nullptr;

  for (BasicBlock *Succ : successors(Header)) {
    if (!L->contains(Succ))
      continue; 

    if (Succ == Latch) //Se l'header ha come successore il latch, allora il body è l'header (caso dei loop guarded)
      Body = Header;
    else 
      Body = Succ;
    break;
  }

  assert(Body && "Body non trovato");
  return Body;
}

//Funzione che percmette di unire i body di più di due loop
void moreLoop(BasicBlock *L1Body, BasicBlock *L0Body, BasicBlock *L0Latch) {
  Instruction *T0 = L0Body->getTerminator(); 
  if (T0->getSuccessor(0) == L0Latch) { 
    T0->eraseFromParent();                   
    BranchInst::Create(L1Body, L0Body); 
  } else {
    moreLoop( L1Body, T0->getSuccessor(0), L0Latch); 
  }
}



/*void loopFusion(Loop *L0, Loop *L1, Function &F, LoopInfo &LI) {
 
  
  PHINode *indvar0 = L0->getCanonicalInductionVariable();
  PHINode *indvar1 = L1->getCanonicalInductionVariable();
  if (!indvar0 || !indvar1) {
    errs() << "Induction variable non trovata\n";
    return;
  }

  outs()<<"indvar0: "<<*indvar0<<"\n";
  outs()<<"indvar1: "<<*indvar1<<"\n";
  // 2) Branch da L0Body → L1Body e da L1Body → L0Latch

  
}*/

//Funzione che fonde i loop NON Guarded
void loopFusion(Loop *L0, Loop *L1, Function &F, LoopInfo &LI) {

  BasicBlock *L0Body = getLoopBody(L0);
  BasicBlock *L1Body = getLoopBody(L1);
 
  // 1) Sostiuiamo tutti gli IV (Induction Variables) di L1 con quelli di L0

  PHINode *PhiI0 = L0->getCanonicalInductionVariable();
  PHINode *PhiI1 = L1->getCanonicalInductionVariable();

  if( !PhiI0 || !PhiI1 ){
    errs() << "Induction variable non trovata\n";
    return;
  } 

  PhiI1->replaceAllUsesWith(PhiI0); //non cancella PhiI1 in sé, ma semplicemente reindirizza tutti gli “usi” (uses) verso PhiI0.
  PhiI1->eraseFromParent();  //Cnacella la phi di %i1

  BasicBlock *L1Pre   = L1->getLoopPreheader();
  BasicBlock *L1Hdr   = L1->getHeader();
  BasicBlock *L1Latch = L1->getLoopLatch();
  BasicBlock *L1Exit  = L1->getExitBlock();

  BasicBlock *L0Latch = L0->getLoopLatch(); 

  // 2) Branch da L0Body → L1Body e da L1Body → L0Latch

  
  moreLoop(L1Body, L0Body, L0Latch); 
  
  
  Instruction *T1 = L1Body->getTerminator();
  T1->eraseFromParent();                  
  BranchInst::Create(L0Latch, L1Body);     
  
  // 3) Branch da L0Header → L1Exit
  
  Instruction *THeader0 = L0->getHeader()->getTerminator();
  for (int i=0; i < THeader0->getNumSuccessors(); i++) {
    if (THeader0->getSuccessor(i) == L1Pre) { 
      THeader0->setSuccessor(i, L1Exit); 
      break;
    }
  }

  // 4) Cancelliamo Preheader, Header e Latch di L1
  
  L1Pre->eraseFromParent();  
  L1Hdr->eraseFromParent();  
  L1Latch->eraseFromParent(); 
  
}

void loopFusionUnGuarded(Loop *L0, Loop *L1, Function &F, LoopInfo &LI) {
  BasicBlock *L0Body  = getLoopBody(L0);
  BasicBlock *L1Body  = getLoopBody(L1);
  BasicBlock *L0Latch = L0->getLoopLatch();
  BasicBlock *L1Latch = L1->getLoopLatch();
  BasicBlock *L1Exit  = L1->getExitBlock();

  if (!L0Body || !L1Body || !L0Latch || !L1Latch || !L1Exit) {
    errs() << "Loop structure incompleta: impossibile fondere\n";
    return;
  }

  // 1) Sostituire le IV canoniche (se ci sono)
  if (auto *PhiI0 = L0->getCanonicalInductionVariable()) {
    if (auto *PhiI1 = L1->getCanonicalInductionVariable()) {
      PhiI1->replaceAllUsesWith(PhiI0);
      PhiI1->eraseFromParent();
    }
  }

  // 2) Inseriamo il corpo di L1 tra L0Body e L0Latch
  moreLoop(L1Body, L0Body, L0Latch);

  // 3) Modifica del terminator di L1Body: punta a L0Latch
  if (auto *T1 = L1Body->getTerminator()) {
    T1->eraseFromParent();
    BranchInst::Create(L0Latch, L1Body);
  }

  // 4) Modifica del terminator del latch (condizionale do-while):
  //    Il true-branch deve tornare a L0Body e il false-branch andare a L1Exit.
  if (auto *BI = dyn_cast<BranchInst>(L0Latch->getTerminator())) {
    assert(BI->isConditional() && "Latch non condizionale in do-while!");
    // Individua quale successore era il corpo:
    BasicBlock *Succ0 = BI->getSuccessor(0);
    BasicBlock *Succ1 = BI->getSuccessor(1);

    // Supponiamo che 0 fosse true→loop e 1 fosse false→exit (o viceversa).
    // Verifichiamo:
    if (Succ0 != L0Body && Succ1 == L0Body) {
      // swap se necessario
      BI->swapSuccessors();
      std::swap(Succ0, Succ1);
    }

    // Ora Succ0 == L0Body (ritorno al loop) e Succ1 era vecchio exit intermedio
    // Rimpiazziamo Succ1 con L1Exit
    BI->setSuccessor(1, L1Exit);
  }

  // 5) Rimuoviamo L1Latch e L1Body se orfani
  if (L1Latch->hasNPredecessors(0))
    L1Latch->eraseFromParent();
  if (L1Body->hasNPredecessors(0))
    L1Body->eraseFromParent();

  // Non tocchiamo header/preheader: supporta anche do-while
}




struct LoopFusionPass : PassInfoMixin<LoopFusionPass> { 
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {

      bool changed = true;
    while (changed) {
        changed = false;
        
        auto &loopinfo = AM.getResult<LoopAnalysis>(F); 
        ScalarEvolution &SCE = AM.getResult<ScalarEvolutionAnalysis>(F);
        auto &DT = AM.getResult<DominatorTreeAnalysis>(F);
        auto &PDT = AM.getResult<PostDominatorTreeAnalysis>(F);
        DependenceInfo &DI = AM.getResult<DependenceAnalysis>(F);
        
        auto loops = loopinfo.getLoopsInPreorder();

        for (int i = 0; i + 1 < loops.size(); i++) {

             if (isGuarded(loops[i]) != isGuarded(loops[i+1]))
                continue;

            if (isGuarded(loops[i]) == isGuarded(loops[i+1])){
              if (!isAdjacent(loops[i], loops[i + 1]))
                 continue;
                  
            
                outs() << "loop adjacent ok \n";
            
                if (!controlFlowEq(loops[i], loops[i+1], DT, PDT))
                    continue;

                outs() << "loop dominance ok \n";
                
            }
              
            if( isScalEv( loops[i], loops[i+1], SCE))
             outs() << "loop bounds ok \n";


            if (!hasDependence(loops[i], loops[i + 1], DI))
                    outs() << "loop dep ok\n";

             loopFusionUnGuarded(loops[i], loops[i + 1],F,loopinfo);
        }

       
        for( BasicBlock &BB : F){
             outs()<<BB<<"\n";
         }
    }

        return PreservedAnalyses::none();
    }

  static bool isRequired() { return true; }
};

// Plugin registration
llvm::PassPluginLibraryInfo getLoopFusionPassPluginInfo() {
  return {
    LLVM_PLUGIN_API_VERSION, "LoopFusionPass", LLVM_VERSION_STRING,
    [](PassBuilder &PB) {
      PB.registerPipelineParsingCallback(
        [](StringRef Name, FunctionPassManager &FPM,
           ArrayRef<PassBuilder::PipelineElement>) {
          if (Name == "LoopFusionPass") {
            FPM.addPass(LoopFusionPass());
            return true;
          }
          return false;
        });
    }
  };
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getLoopFusionPassPluginInfo();
}