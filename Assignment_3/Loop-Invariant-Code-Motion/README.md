# Compilatori 2024-2025 Frattolillo-Canovi
## Loop Invariant Pass

This `Loop Invariant Code Motion` pass identifies and moves computations that are invariant within a loop to a location outside the loop. This optimization reduces redundant calculations and improves runtime performance.

## How to run
Remember after make changes to pass "LoopInvariantPass.cpp" go to './build' directory and call command 'make' 

### Make
1. Create into current directory a sub-directory 'build':  
    `mkdir build`

2. Go to 'build' and run cmake:  
    `cmake -DLT_LLVM_INSTALL_DIR=$LLVM_DIR ../`

3. In to 'build' generate 'libLoopInvariantPass.so':  
    `make`

### Apply pass
Command should be executed in the main directory 'Loop-Invariant-Code-Motion'

1. Compile your C code into LLVM bitcode:  
    `clang -Xclang -disable-O0-optnone -S -emit-llvm -O0 ./test/LoopInv.c -o ./test/LoopInv.ll`

2. Apply mem2reg optimization:  
    `opt -p mem2reg ./test/LoopInv.ll -o ./test/LoopInv.m2r.bc`

    (Readable format after mem2reg optimization)    
    `llvm-dis ./test/LoopInv.m2r.bc -o ./test/LoopInv.m2r.ll `

3. Apply "Loop Invariant Code Motion" pass:  
    `opt -load-pass-plugin build/libLoopInvariantPass.so -p LoopInvariantPass -S ./test/LoopInv.m2r.ll -o ./test/LoopInv.optimized.ll`