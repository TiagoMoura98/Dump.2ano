#include <stdio.h>
#include "func.h"

int main(){
    //declaration and initialization of vars and pointers
    int arr [] = {1,2,1,5,6,4};
    int *min = 0;
    int *max = 0;
    float *avg = 0;
    //function call
    get_array_statistics(arr,6,min,max,avg);

    //testing
    //printf("Value of min %d\n", *min);
    printf("Value of max %d\n", *max);
    printf("Value of avg %f\n", *avg);

    return 0;
}