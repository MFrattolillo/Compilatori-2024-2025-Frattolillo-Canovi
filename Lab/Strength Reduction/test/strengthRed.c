#include <stdio.h>
#include <stdlib.h>

void strengthReductionTest1 (int x) {
    int y1 = 15 * x;
    int y2 = x * 15;
    int y3 = x / 8;

    printf("y1: %d\n", y1);
    printf("y2: %d\n", y2);
    printf("y3: %d\n", y3);
}

void strengthReductionTest2 (int x) {
    int z3 = x / 8;
    int z2 = x * 15;
    int z1 = 15 * x;

    printf("z1: %d\n", z1);
    printf("z2: %d\n", z2);
    printf("z3: %d\n", z3);
}