# Compilatori 2024-2025 Frattolillo-Canovi
## Loop Fusion

`Loop Fusion` pass

## How to run
Remember after make changes to pass "LoopFusionPass.cpp" go to './build' directory and call command 'make' 

### Make
1. Create into current directory a sub-directory 'build':  
    `mkdir build`

2. Go to 'build' and run cmake:  
    `cmake -DLT_LLVM_INSTALL_DIR=$LLVM_DIR ../`

3. In to 'build' generate 'libLoopFusion.so':  
    `make`

### Apply pass
Command should be executed in the main directory 'Loop-Fusion'

1. Compile your C code into LLVM bitcode:  
    `clang -Xclang -disable-O0-optnone -S -emit-llvm -O0 ./test/loopFus.c -o ./test/loopFus.ll`

2. Apply mem2reg optimization:  
    `opt -p mem2reg ./test/loopFus.ll -o ./test/loopFus.m2r.bc`

    (Readable format after mem2reg optimization)    
    `llvm-dis ./test/loopFus.m2r.bc -o ./test/loopFus.m2r.ll `

3. Apply "Loop Fusion" pass:  
    `opt -load-pass-plugin build/libLoopFusion.so -p loopFusion -S ./test/loopFus.m2r.ll -o ./test/loopFus.optimized.ll`