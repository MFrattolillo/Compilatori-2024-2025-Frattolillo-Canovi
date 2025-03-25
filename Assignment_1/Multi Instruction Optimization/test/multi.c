#include <stdio.h>
#include <stdlib.h>

void Multi(int b){
    int a = b+1;
    int c = a+2;
    int d = a-1;
    // After the optimization should be e = b + 2, beacuse d will be removed and all of its uses will be replaced with b
    int e = d+2;

    printf("a: %d\n", a);
    printf("c: %d\n", c);
    printf("d: %d\n", d);
    printf("e: %d\n", e);
}
