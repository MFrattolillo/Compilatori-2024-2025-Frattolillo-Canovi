
#include "LoopFusionPass.h"
#include "llvm/IR/InstrTypes.h"
#include <llvm/Analysis/LoopInfo.h>
#include <llvm/Analysis/ScalarEvolution.h>
#include <llvm/ADT/SmallSet.h>
#include <llvm/Transforms/Utils/LoopSimplify.h>
#include <llvm/Transforms/Scalar/LoopRotation.h>

#include <cmath>
#include <stdint.h>

using namespace llvm;

PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM){
    LoopInfo &LI = AM.getResult<LoopAnalysis>(F);

    for (auto *L : LI) {
        for (auto *S : LI) {
            // Make sure we are not checking the same loop
            if (L == S)
                continue;

            // // Check if loop fusion is possible
            // if (isLoopAdjacent(L, S)) {
            //     outs() << "Loop fusion possible between loops: " << L->getHeader()->getName() << " and " << S->getHeader()->getName() << "\n";
            // }
        }
    }

    outs() << "END\n";

    return PreservedAnalyses::all();
}

bool LoopFusionPass::isLoopGuarded(Loop *L) 
{
    // Ottieni il preheader del loop
    BasicBlock *Preheader = L->getLoopPreheader();
    if (!Preheader)
        return false; // Nessun preheader → il loop non è guarded

    // Ottieni l'ultima istruzione del preheader
    Instruction *Terminator = Preheader->getTerminator();
    if (!Terminator)
        return false;

    // Controlla se è un branch condizionale
    if (BranchInst *BI = dyn_cast<BranchInst>(Terminator)) {
        if (BI->isConditional()) {
            // Ottieni i successori del branch
            BasicBlock *TrueSucc = BI->getSuccessor(0);
            BasicBlock *FalseSucc = BI->getSuccessor(1);

            // Controlla se uno dei successori è l'header del loop
            BasicBlock *Header = L->getHeader();
            if (TrueSucc == Header || FalseSucc == Header) {
                return true; // Il loop è guarded
            }
        }
    }
}

/*
    Due loop L0 e L1 sono adiacenti se non ci sono basic blocks 
    aggiuntivi nel CFG tra l’uscita di L0 e l’entry di L1.

    Se i loop sono guarded il successore non loop del guard 
    branch di L0 deve essere l’entry block di L1.

    Se i loop non sono guarded l’exit block di L0 deve essere il 
    preheader di L1
*/
bool LoopFusionPass::isLoopAdjacent(Loop *a, Loop *b) 
{
    if (!a || !b)
        return false;

}