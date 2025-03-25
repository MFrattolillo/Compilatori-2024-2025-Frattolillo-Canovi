# Compilatori 2024-2025 Frattolillo-Canovi
## Multi Instruction Optimization Pass

The `MultiInstructionOptimization` pass simplifies redundant or opposite arithmetic operations in LLVM IR by identifying and removing unnecessary instructions while preserving the program's semantics.

The `MultiInstructionOptimization` pass performs the following optimizations:

1. **Opposite Instructions**:
   - Identifies pairs of opposite instructions (e.g., `a + 1` followed by `a - 1`) and removes the redundant instruction.
   - Replaces all uses of the redundant instruction with the appropriate operand.

### Example

Before applying the pass:  
`%1 = add i32 %0, 1`  
`%2 = sub i32 %1, 1`  

After applying the pass:  
`%1 = add i32 %0, 1`  
`%2 = %0`

## How to run
Remember after make changes to pass "MultiInstructionOptimization.cpp" go to './build' directory and call command 'make' 

### Make
1. Create into current directory a sub-directory 'build':  
    `mkdir build`

2. Go to 'build' and run cmake:  
    `cmake -DLT_LLVM_INSTALL_DIR=$LLVM_DIR ../`

3. In to 'build' generate 'libMultiInstructionOptimization.so':  
    `make`

### Apply pass
Command should be executed in the main directory 'Multi Instruction Optimization'

1. Compile your C code into LLVM bitcode:  
    `clang -Xclang -disable-O0-optnone -S -emit-llvm -O0 ./test/multi.c -o ./test/multi.ll`

2. Apply mem2reg optimization:  
    `opt -p mem2reg ./test/multi.ll -o ./test/multi.m2r.bc`

    (Readable format after mem2reg optimization)  
    `llvm-dis ./test/multi.m2r.bc -o ./test/multi.m2r.ll`

3. Apply "Multi Instruction Optimization" pass:  
    `opt -load-pass-plugin build/libMultiInstructionOptimization.so -p multi-instruction-optimization -S ./test/multi.m2r.ll -o ./test/multi.optimized.ll`