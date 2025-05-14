#ifndef LOOP_FUSION_PASS_H
#define LOOP_FUSION_PASS_H

#include "llvm/IR/PassManager.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Analysis/LoopInfo.h"

using namespace llvm;

class LoopFusionPass final : public llvm::PassInfoMixin<LoopFusionPass> {
public:
    PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);

private:
    bool isLoopGuarded(Loop *L);
    bool isLoopAdjacent(Loop *a, Loop *b);
}; 

#endif // LOOP_FUSION_PASS_H