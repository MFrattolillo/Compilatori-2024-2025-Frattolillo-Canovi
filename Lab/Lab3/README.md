# Compilatori 2024-2025 Frattolillo-Canovi
## Loop Pass

## How to run
Remember after make changes to pass "LoopPass.cpp" go to './build' directory and call command 'make' 

### Make
1. Create into current directory a sub-directory 'build':  
    `mkdir build`

2. Go to 'build' and run cmake:  
    `cmake -DLT_LLVM_INSTALL_DIR=$LLVM_DIR ../`

3. In to 'build' generate 'libStrengthReduction.so':  
    `make`

### Apply pass
Command should be executed in the main directory 'Lab3'

1. Compile your C code into LLVM bitcode:  
    `clang -Xclang -disable-O0-optnone -S -emit-llvm -O0 ./test/loop.c -o ./test/loop.ll`

2. Apply mem2reg optimization:  
    `opt -p mem2reg ./test/loop.ll -o ./test/loop.m2r.bc`

    (Readable format after mem2reg optimization)  
    `llvm-dis ./test/loop.m2r.bc -o ./test/loop.m2r.ll`

3. Apply "Loop Pass" pass:  
    `opt -load-pass-plugin build/libLoopPass.so -p loop-pass -S ./test/loop.m2r.ll -o ./test/loop.optimized.ll`