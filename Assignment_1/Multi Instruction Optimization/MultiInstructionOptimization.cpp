#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
  class InstructionInformation {
    public:
    InstructionInformation(Instruction *instr) : m_instr(instr) {}

    bool isInstructionSupported() {
      if (!m_instr)
        return false;

      switch (m_instr->getOpcode()) {
        case Instruction::Add:
        case Instruction::Sub:
          return true;
        }
        return false;
    }
    unsigned oppositeInstructionType() {
      if (!isInstructionSupported())
        return 0;
        
      switch (m_instr->getOpcode()) {
        case Instruction::Add:
          return Instruction::Sub;
        case Instruction::Sub:
          return Instruction::Add;
      }
      return 0;
    }
    bool isInstructionOpposite(InstructionInformation *other) {
      if (!isInstructionSupported() || !other->isInstructionSupported())
        return false;

      if (this->oppositeInstructionType() != other->m_instr->getOpcode())
        return false;

      bool isInstructionOpposite = false;
      
      auto op1 = dyn_cast<ConstantInt>(m_instr->getOperand(0)); 
      auto op2 = dyn_cast<ConstantInt>(m_instr->getOperand(1)); 
      auto otherOp1 = dyn_cast<ConstantInt>(other->m_instr->getOperand(0)); 
      auto otherOp2 = dyn_cast<ConstantInt>(other->m_instr->getOperand(1)); 

      ConstantInt *opToCheck = nullptr;
      ConstantInt *otherOpToCheck = nullptr;
      if (op1)
        opToCheck = op1;
      else if (op2)
        opToCheck = op2;

      if (otherOp1)
        otherOpToCheck = otherOp1;
      else if (otherOp2)
        otherOpToCheck = otherOp2;

      if (opToCheck && otherOpToCheck) {
        if (opToCheck->getValue() == otherOpToCheck->getValue())
          return true;
      }
      
      return false;
    }
    Value *getNonConstantOperand() {
      if (!isInstructionSupported())
        return nullptr;

      if (!isa<Constant>(m_instr->getOperand(0)))
        return m_instr->getOperand(0);

      if (!isa<Constant>(m_instr->getOperand(1)))
        return m_instr->getOperand(1);
    
      return nullptr;
    }

    private:
      Instruction *m_instr;
  };

  struct MultiInstructionOptimization: PassInfoMixin<MultiInstructionOptimization> {
    bool runOnFunction(Function &F) {
      bool Transformed = false;
      outs() << "\n";
      outs() << " Eseguito il pass `MultiInstructionOptimization` sulla Funzione: `" << F.getName() << "`\n";
      outs() << "============================================================\n\n";

      outs() << "--------------------------------------\n";
      outs() << " Istruzioni prima dell'ottimizzazione: \n";
      outs() << "--------------------------------------\n\n";
      for (BasicBlock &BB : F) {
        for (Instruction &Instr : BB) {
          Instr.print(outs());
          outs() << "\n";
        }
      }

      std::vector<Instruction *> instructionsToRemove;
      for (BasicBlock &basicBlock : F) {
        for (Instruction &instr : basicBlock) {
          InstructionInformation instrInfo(&instr);
          for (Use &use : instr.uses()) {
            auto instrToOptimize = dyn_cast<Instruction>(use.getUser());
            InstructionInformation instrToOptimizeInfo(instrToOptimize);
            if (instrToOptimizeInfo.isInstructionOpposite(&instrInfo)) {
              auto *value = instrInfo.getNonConstantOperand();
              if (!value)
                continue;
              
              instrToOptimize->replaceAllUsesWith(value);              
              instructionsToRemove.push_back(instrToOptimize);
            }
          }
        }
      }

      for (auto instr : instructionsToRemove) {
        instr->eraseFromParent();
        Transformed = true;
      }
    

      outs() << "----------------------------------\n";
      outs() << "Istruzioni dopo l'ottimizzazione:|\n";
      outs() << "----------------------------------\n";
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

llvm::PassPluginLibraryInfo getMultiInstructionOptimizationPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "MultiInstructionOptimization", LLVM_VERSION_STRING,
          [](PassBuilder &PB) {
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                  if (Name == "multi-instruction-optimization") {
                    FPM.addPass(MultiInstructionOptimization());
                    return true;
                  }
                  return false;
                });
          }};
}

extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return getMultiInstructionOptimizationPluginInfo();
}