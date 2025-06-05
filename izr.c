#include <stdio.h>

int main() {
   int array[] = {21, 3, 7};
   int *p = array;
   printf("%p\n", p);
   printf("%p\n", p + 1);
   printf("%p\n", p + 2);
   // printf("%p\n", p + 3);
   return 0;
}