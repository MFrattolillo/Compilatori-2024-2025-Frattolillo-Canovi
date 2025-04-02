#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Module.h"

using namespace llvm;

namespace {
  enum OptimizationType {
    Mul15,
    Div8,
    None
  };

  struct StrengthReduction: PassInfoMixin<StrengthReduction> {
    void optimizeInstructionMul15(Instruction &instr) {
      Value *x = nullptr;
      if (auto *op = dyn_cast<ConstantInt>(instr.getOperand(0))) {
        if (op->getValue() == 15)
            x = instr.getOperand(1);
      } else if (auto *op = dyn_cast<ConstantInt>(instr.getOperand(1))) {
          if (op->getValue() == 15)
            x = instr.getOperand(0);
      }

      if (!x) 
        return;
      
      auto shiftCount = ConstantInt::get(x->getType(), 4);
      auto *shiftedInstruction = BinaryOperator::CreateShl(x, shiftCount, "shifted", &instr);
      auto *optimizedInstruction = BinaryOperator::CreateSub(shiftedInstruction, x, "sub", &instr);  
      instr.replaceAllUsesWith(optimizedInstruction);
      instr.eraseFromParent();
    }

    void optimizeInstructionDiv8(Instruction &instr) {    
      Value *x = instr.getOperand(0);
      auto shiftCount = ConstantInt::get(x->getType(), 3);
      auto *optimizeInstruction = BinaryOperator::CreateAShr(x, shiftCount, "shifted", &instr);
      instr.replaceAllUsesWith(optimizeInstruction);
      instr.eraseFromParent();
    }

    bool runOnModule(Module &M) {
      bool transformed = false;
      for (Function &F : M)
        transformed |= runOnFunction(F);
        
      return transformed;
    }

    bool runOnFunction(Function &F) {
      bool transformed = false;
      outs() << "\n";
      outs() << " Eseguito il pass `StrengthReduction` sulla Funzione: `" << F.getName() << "`\n";
      outs() << "============================================================\n\n";

      outs() << "--------------------------------------\n";
      outs() << " Istruzioni prima dell'ottimizzazione: \n";
      outs() << "--------------------------------------\n\n";
      for (BasicBlock &BB : F) {
        for (Instruction &Instr : BB) {
          Instr.print(errs());
          errs() << "\n";
        }
      }

      for (BasicBlock &BB : F) {
        for (auto I = BB.begin(); I != BB.end(); ) {
          auto &instr = *I++;
          auto optimizationType = OptimizationType::None;

          if (instr.getOpcode() == Instruction::Mul) { 
              auto op0 = dyn_cast<ConstantInt>(instr.getOperand(0));
              auto op1 = dyn_cast<ConstantInt>(instr.getOperand(1)); 
              if ((!op0 || op0->getValue() != 15) &&
                  (!op1 || op1->getValue() != 15))
                continue;

              optimizationType = OptimizationType::Mul15;
          } else if (instr.getOpcode() == Instruction::SDiv) {
              auto op1 = dyn_cast<ConstantInt>(instr.getOperand(1));
              if (!op1 || op1->getValue() != 8)
                continue;
                  
              optimizationType = OptimizationType::Div8;
          } else {
              continue;
          }


          switch (optimizationType)
          {
          case Mul15:
            optimizeInstructionMul15(instr);
            break;
          
          case Div8:
            optimizeInstructionDiv8(instr);
            break;

          default:
            break;
          }
            
        }
    }
    

    outs() << "----------------------------------\n";
    outs() << "Istruzioni dopo l'ottimizzazione:|\n";
    outs() << "----------------------------------\n";
      for (BasicBlock &BB : F) {
        for (Instruction &Instr : BB) {
          Instr.print(errs());
          errs() << "\n";
        }
      }

      return transformed;
    }

    PreservedAnalyses run(Module &M, ModuleAnalysisManager &) {
      if (!runOnModule(M))
        return PreservedAnalyses::none();
      return PreservedAnalyses::all();
    }

    static bool isRequired() { return true; }
  };
}

llvm::PassPluginLibraryInfo getStrengthReductionPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "StrengthReduction", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, ModulePassManager &MPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "strength-reduction") {
                    MPM.addPass(StrengthReduction());
                    return true;
                  }
                  return false;
                });
          }};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getStrengthReductionPluginInfo();
}
