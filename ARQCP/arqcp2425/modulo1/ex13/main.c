#include <stdio.h>
#include "func.h"


int main(){

    short arr [] = {3,5,2,5,1};
    short arr2 [5];
    short *ptr_arr2 = arr2;

    int sizeArr2 = sort_without_reps(arr,5,arr2);

    for (int i = 0; i < 5; i++, ++ptr_arr2)
    {
        printf("%d",*ptr_arr2);
    }

    printf("\n Number of values copied: %d\n", sizeArr2);
    
    return 0;
}