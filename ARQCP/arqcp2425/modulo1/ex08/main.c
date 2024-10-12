#include <stdio.h>
#include "func.h"

int main(){

    unsigned int x = 4;
    int sum;
    sum = sum_integer_bytes(&x);

    printf("Value of sum %d\n", sum);

    return 0;
}