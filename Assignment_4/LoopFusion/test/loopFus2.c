#define N 5

void loopFusionExample(int n) {
    int a[N] = {};
    int b[N] = {1, 2, 3, 4, 5};
    int c[N] = {};
    int d[N] = {};

    for (int i = 0; i < n; i++) {
        a[i] = b[i]+2;
      
    }

    for (int i = 0; i < n; i++) {
        c[i] = b[i+3] * 2;
    }
  
    
    for (int i = 0; i < n; i++) {
       d[i] = b[i];
    }
}
