#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {
  struct AlgebraicIdentity: PassInfoMixin<AlgebraicIdentity> {

    bool runOnFunction(Function &F) {
      bool transformed=false;
      
       outs()<<"PRIMA DELL OTTIMIZZAZIONE----------\n";
       for(auto &BB : F ){
        for(auto &Inst : BB )
         outs()<<Inst<<"\n";
         outs()<<"---------------------------------\n";
       }

       /*1.controllare se istruzione è add
         2.controllo se uno dei 2 operandi è 0
         3.sostituire tutti gli usi dell altro operando  
         4.eliminare quell'istruzione  */
        
         for( BasicBlock &BB : F ){
          for( auto it=BB.begin(); it != BB.end(); ){
           Instruction &Inst = *it++;
          
            if( auto *op = dyn_cast<BinaryOperator>(&Inst)){
              if( op->getOpcode() == Instruction::Add){
                auto *op_const0 = dyn_cast<ConstantInt>(Inst.getOperand(0));
                auto *op_const1 = dyn_cast<ConstantInt>(Inst.getOperand(1));
                if( op_const0 && op_const0->getValue() == 0){
                  Inst.replaceAllUsesWith(Inst.getOperand(1));
                  Inst.eraseFromParent();
                }
                else {
                  if( op_const1 && op_const1->getValue() == 0){
                  Inst.replaceAllUsesWith(Inst.getOperand(0));
                  Inst.eraseFromParent();
                 }
                }
              }
              else {
                if(op->getOpcode() == Instruction::Mul){
                 auto *op_const0 = dyn_cast<ConstantInt>(Inst.getOperand(0));
                 auto *op_const1 = dyn_cast<ConstantInt>(Inst.getOperand(1));
                  if( op_const0 && op_const0->getValue() == 1){
                   Inst.replaceAllUsesWith(Inst.getOperand(1));
                   Inst.eraseFromParent();
                }
                else {
                  if( op_const1 && op_const1->getValue() == 1){
                  Inst.replaceAllUsesWith(Inst.getOperand(0));
                  Inst.eraseFromParent();
                 }
                }
               }
              }
             }  
            }
           }
         

       outs()<<"DOPO L'OTTIMIZZAZIONE----------\n";
       for(auto &BB : F ){
        for(auto &Inst : BB )
         outs()<<Inst<<"\n";
         outs()<<"---------------------------------\n";
       }

      return transformed;
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
