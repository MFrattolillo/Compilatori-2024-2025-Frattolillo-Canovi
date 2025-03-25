# Compilatori 2024-2025 Frattolillo-Canovi
## Strength Reduction Pass

The `StrengthReduction` pass is an LLVM optimization pass that replaces expensive arithmetic operations with cheaper ones, such as replacing multiplication or division by constants with equivalent bitwise operations. This optimization improves the performance of the generated code.


The `StrengthReduction` pass performs the following optimizations:

1. **Multiplication by 15**:
   - Replaces `x * 15` with `(x << 4) - x` (a left shift followed by a subtraction).

2. **Division by 8**:
   - Replaces `x / 8` with `x >> 3` (an arithmetic right shift).

### Example

Before applying the pass:  
`%1 = mul i32 %0, 15`  
`%2 = sdiv i32 %3, 8`

After applying the pass:  
`%1 = shl i32 %0, 4`  
`%2 = sub i32 %1, %0`  
`%4 = ashr i32 %3, 3`

## How to run
Remember after make changes to pass "StrengthReduction.cpp" go to './build' directory and call command 'make' 

### Make
1. Create into current directory a sub-directory 'build':  
    `mkdir build`

2. Go to 'build' and run cmake:  
    `cmake -DLT_LLVM_INSTALL_DIR=$LLVM_DIR ../`

3. In to 'build' generate 'libStrengthReduction.so':  
    `make`

### Apply pass
Command should be executed in the main directory 'Strength Reduction'

1. Compile your C code into LLVM bitcode:  
    `clang -Xclang -disable-O0-optnone -S -emit-llvm -O0 ./test/strengthRed.c -o ./test/strengthRed.ll`

2. Apply mem2reg optimization:  
    `opt -p mem2reg ./test/strengthRed.ll -o ./test/strengthRed.m2r.bc`

    (Readable format after mem2reg optimization)  
    `llvm-dis ./test/strengthRed.m2r.bc -o ./test/strengthRed.m2r.ll`

3. Apply "Strength Reduction" pass:  
    `opt -load-pass-plugin build/libStrengthReduction.so -p strength-reduction -S ./test/strengthRed.m2r.ll -o ./test/strengthRed.optimized.ll`