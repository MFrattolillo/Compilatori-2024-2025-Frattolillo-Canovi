#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Module.h"
#include "llvm/Analysis/CFGPrinter.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"

using namespace llvm;

namespace {
  struct LoopPass : PassInfoMixin<LoopPass> {
    bool runOnFunction(Function &F, FunctionAnalysisManager &AM) {
      outs() << "\n"; 
      outs() << " Eseguito il pass `LoopPass` sulla Funzione: `" << F.getName() << "`\n";
      outs() << "============================================================\n\n";

      LoopInfo &LI = AM.getResult<LoopAnalysis>(F);

      for (BasicBlock &BB : F) {
        //get loop of the basic block
        Loop *L = LI.getLoopFor(&BB);
        if (L) {
          outs() << "Loop trovato in: " << BB.getName() << "\n";
        } else {
          outs() << "Nessun loop trovato in: " << BB.getName() << "\n";
        }
      }

      return false;
    }

    PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
      if (!runOnFunction(F, AM))
        return PreservedAnalyses::none();
      return PreservedAnalyses::all();
    }

    static bool isRequired() { return true; }
  };
}

llvm::PassPluginLibraryInfo getLoopPassPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "LoopPass", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "loop-pass") {
                    FPM.addPass(LoopPass());
                    return true;
                  }
                  return false;
                });
          }};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getLoopPassPluginInfo();
}
