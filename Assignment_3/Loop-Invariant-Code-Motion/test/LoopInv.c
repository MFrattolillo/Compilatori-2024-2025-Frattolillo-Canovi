// test_loop_simple.c
int LoopInv(int a, int b) {
    int res = 0;
    int k = 3;

    for (int i = 0; i < 10; i++) {
        int tmp = a + b;  // loop-invariant
        res += tmp + i;   // dipende da i quindi non invariant
        k = k * 2;        // k cambia ogni volta quindi non invariant
    }

    return res + k;
}
