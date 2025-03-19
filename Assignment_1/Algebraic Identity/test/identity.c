#include <stdio.h>
#include <stdlib.h>

void Identity(int x) {
   int x1 = x + 0;
   int x2 = 0 + x;
   int x3 = x * 1;
   int x4 = 1 * x;

   printf("x1: %d\n", x1);
   printf("x2: %d\n", x2);
   printf("x3: %d\n", x3);
   printf("x4: %d\n", x4);    
}  
