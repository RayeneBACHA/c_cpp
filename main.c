#include <stdio.h>
#include <malloc.h>

#include "math.h"
int a = 88;



int main(int argc, char const *argv[])
{   

    int g = add(5,6);
    
    float c = 55.66;
    int f = (int)c;
    int * ptr = (int*)malloc(1 * sizeof(int));
    *(ptr +1 ) = 52;
    printf("Hellow\n");
    printf("Hellow\n");

    return 0;
}
