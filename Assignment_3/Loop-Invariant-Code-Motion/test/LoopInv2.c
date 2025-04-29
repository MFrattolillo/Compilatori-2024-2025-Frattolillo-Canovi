int LoopInv2(int a, int b) {
    int res = 0;
    int k = 3;
    int tmp = a + b;  // spostato fuori dal loop: loop-invariant

    for (int i = 0; i < 10; i++) {
        int z = i * 2;     // nuova definizione (non ridefinizione)
        res = res + z;     // ridefinizione di res
        k = k * 2;         // ridefinizione di k
    }

    return res + k + tmp;
}
