
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
#include <llvm/Transforms/Utils/LoopSimplify.h>
#include <llvm/Transforms/Scalar/LoopRotation.h>
#include <llvm/Analysis/DependenceAnalysis.h>
#include <optional>

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

bool isDom_Postdom( Loop *a , Loop *b , DominatorTree *DT , PostDominatorTree *PDT ){
   

    SmallVector<BasicBlock *, 2> exits;
    a->getExitBlocks(exits);

    auto *Preheader = b->getLoopPreheader();

    for( auto *exit : exits ){
      if (!(DT->dominates(exit, Preheader) && PDT->dominates(Preheader, exit))) {
          return false;
        }
    }

    return true;
}

/*
bool isDep(Loop *L1, Loop *L2, DependenceInfo &DI) {
    for (BasicBlock *BB1 : L1->getBlocks()) {
        for (Instruction &I1 : *BB1) {
            if (!I1.mayReadOrWriteMemory())
                continue;

            for (BasicBlock *BB2 : L2->getBlocks()) {
                for (Instruction &I2 : *BB2) {
                    if (!I2.mayReadOrWriteMemory())
                        continue;

                    auto dep = DI.depends(&I1, &I2, true);
                    if (dep) {
                        return true;  // dipendenza trovata
                    }
                }
            }
        }
    }
    return false;  // nessuna dipendenza trovata
}
*/

void loopFusion( Loop *a , Loop *b, ScalarEvolution &SCE ){


   auto *header_a = a->getHeader();
   auto *header_b= b->getHeader();
    
   auto &blocks_a = a->getBlocksVector();
   auto &blocks_b = b->getBlocksVector();
    
   auto *latch_a = a->getLoopLatch();
   auto *latch_b = b->getLoopLatch();
    
   auto *preheader_a = a->getLoopPreheader();
   auto *preheader_b = b->getLoopPreheader();

   auto *indvar_a  = a->getInductionVariable(SCE);
   auto *indvar_b  = b->getInductionVariable(SCE);

   auto *exitBlock_a = a->getExitBlock();
   auto *exitBlock_b = b->getExitBlock();

   /*if( !indvar1 || indvar2 )
   outs()<<"errore non riconosciuti indvar\n";*/

   /*indvar_b->moveBefore(indvar_a);
    indvar_a->replaceAllUsesWith(indvar_b);
    indvar_a->eraseFromParent();
    
    
    indvar_b->replaceIncomingBlockWith(preheader_b, preheader_a);
    
    
    header_b->replaceAllUsesWith(header_a);*/
   
   
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
            
                if (!isDom_Postdom(loops[i], loops[i+1], &DT, &PDT))
                    continue;

                outs() << "loop dominance ok \n";
                
            }
              
            if( isScalEv( loops[i], loops[i+1], SCE))
             outs() << "loop bounds ok \n";


            /*if (!isDep(loops[i], loops[i + 1], DI,SCE))
                    outs() << "loop dep ok\n";*/

              loopFusion(loops[i], loops[i + 1], SCE);
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

