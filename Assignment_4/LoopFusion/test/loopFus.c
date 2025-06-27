#define N 5

void loopFusionExample(int n) {
    int a[N] = {};
    int b[N] = {1, 2, 3, 4, 5};
    int c[N] = {};


    
    // First loop
    for (int i = 0; i < n; i++) {
        a[i] = b[i] + 1;
    }
    
    // Second loop
    for (int i = 0; i < n; i++) {
        c[i] = a[i] * 2;
    }
}