#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
  struct AlgebraicIdentity: PassInfoMixin<AlgebraicIdentity> {

    bool runOnFunction(Function &F) {
      bool Transformed = false;
      outs() << "\n";
      outs() << " Eseguito il pass `AlgebraicIdentity` sulla Funzione: `" << F.getName() << "`\n";
      outs() << "============================================================\n\n";

      // Stampa delle istruzioni PRIMA dell'ottimizzazione
      outs() << "--------------------------------------\n";
      outs() << " Istruzioni prima dell'ottimizzazione: \n";
      outs() << "--------------------------------------\n\n";
      for (BasicBlock &BB : F) {
        for (Instruction &Instr : BB) {
          Instr.print(outs());
          outs() << "\n";
        }
      }

      for (BasicBlock &BB : F) {
        for (auto I = BB.begin(); I != BB.end(); ) {
            Instruction &Instr = *I++;

            if (Instr.getOpcode() == Instruction::Add) {
                Value *operands[2] = { Instr.getOperand(0), Instr.getOperand(1) };
    
                // Trova l'operando non zero
                Value *nonZeroOperand = nullptr;
                for (Value *op : operands) {
                    if (ConstantInt *CI = dyn_cast<ConstantInt>(op)) {
                        if (CI->isZero()) continue;  // Se è zero, lo ignoriamo
                    }
                    nonZeroOperand = op;  // Salviamo l'operando valido
                    break;
                }
    
                // Se esiste un operando valido, sostituiamo l'istruzione
                if (nonZeroOperand) {
                    Instr.replaceAllUsesWith(nonZeroOperand);
                    Instr.eraseFromParent();
                    Transformed = true;
                }
            }
            else if(Instr.getOpcode() == Instruction::Mul) {
              Value *operands[2] = {Instr.getOperand(0), Instr.getOperand(1) };
              
              Value *nonOneOperand = nullptr;
              for( Value *op : operands){
                if(ConstantInt *CI = dyn_cast<ConstantInt>(op)) {
                  if(CI->getValue() == 1) continue;
                }
                nonOneOperand = op;
                break;
              }

              if(nonOneOperand) {
                Instr.replaceAllUsesWith(nonOneOperand);
                Instr.eraseFromParent();
                Transformed = true;
              }
            }
        }
    }
    

      // Stampa delle istruzioni DOPO l'ottimizzazione
      outs() << "\n";
      outs() << "--------------------------------------\n";
      outs() << " Istruzioni dopo dell'ottimizzazione: \n";
      outs() << "--------------------------------------\n\n";
      for (BasicBlock &BB : F) {
        for (Instruction &Instr : BB) {
          Instr.print(outs());
          outs() << "\n";
        }
      }

      return Transformed;
    }

    PreservedAnalyses run(Function &F, FunctionAnalysisManager &) {
      if (!runOnFunction(F))
        return PreservedAnalyses::none();
      return PreservedAnalyses::all();
    }

    static bool isRequired() { return true; }
  };
}

llvm::PassPluginLibraryInfo getAlgebraicIdentityPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "AlgebraicIdentity", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "algebric-identity") {
                    FPM.addPass(AlgebraicIdentity());
                    return true;
                  }
                  return false;
                });
          }};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getAlgebraicIdentityPluginInfo();
}
