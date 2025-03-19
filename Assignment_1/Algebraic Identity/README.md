# Compilatori 2024-2025 Frattolillo-Canovi
## Algebraic Identity Optimization Pass

The pass simplifies arithmetic operations in LLVM IR by applying algebraic identities, such as removing redundant additions and multiplications.

The `AlgebraicIdentity` pass performs the following optimizations:

1. **Addition with Zero**:
   - Replaces `x + 0` or `0 + x` with `x`.
   
2. **Multiplication by One**:
   - Replaces `x * 1` or `1 * x` with `x`.

## Example
Before applying the pass:  
`%1 = add i32 %a, 0`  
`%2 = mul i32 %b, 1`

After applying the pass:  
`%1 = %a`  
`%2 = %b`

## How to run
Remember after make changes to pass "AlgebraicIdentity.cpp" go to './build' directory and call command 'make' 

### Make
1. Create into current directory a sub-directory 'build':  
    `mkdir build`

2. Go to 'build' and run cmake:  
    `cmake -DLT_LLVM_INSTALL_DIR=$LLVM_DIR ../`

3. In to 'build' generate 'libAlgebraicIdentity.so':  
    `make`

### Apply pass
Command should be executed in the main directory 'Algebraic Identity'

1. Compile your C code into LLVM bitcode:  
    `clang -Xclang -disable-O0-optnone -S -emit-llvm -O0 ./test/identity.c -o ./test/identity.ll`

2. Apply mem2reg optimization:  
    `opt -p mem2reg ./test/identity.ll -o ./test/identity.m2r.bc`

    (Readable format after mem2reg optimization)    
    `llvm-dis ./test/identity.m2r.bc -o ./test/identity.m2r.ll `

3. Apply "Algebraic Identity" pass:  
    `opt -load-pass-plugin build/libAlgebraicIdentity.so -p algebric-identity -S ./test/identity.m2r.ll -o ./test/identity.optimized.ll`