#define N 5

void loopFusionExample(int n) {
    int a[N] = {};
    int b[N] = {1, 2, 3, 4, 5};
    int c[N] = {};


    // First loop
    for (int i = 0; i < n; i++) {
        a[i] = b[i] + 1;
    }

    
    /*int i=0;
    do
    {
        i++;
        a[i] = b[i] + 1;
    } while (i<n);
    */

   // Second loop
    for (int i = 0; i < 22; i++) {
        c[i] = b[i] * 2;
    }

    /*int j=0;
    do
    {
        j++;
        c[j] = b[j] * 2;
    } while (j<n);
    */

}