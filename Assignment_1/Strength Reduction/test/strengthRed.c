#include <stdio.h>
#include <stdlib.h>

void strengthReductionTest (int x) {
    int y1 = 15 * x;
    int y2 = x * 15;
    int y3 = x / 8;

    printf("y1: %d\n", y1);
    printf("y2: %d\n", y2);
    printf("y3: %d\n", y3);
}