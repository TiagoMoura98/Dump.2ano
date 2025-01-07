#include <stdio.h>
#include "asm.h"

int main(void) {

    int arr [] = {1,2,3,4,5};
    int* buffer = arr;
    int length = 5;
    int* head = 4+arr;
    int* tail = arr;
    int n = 3;
    int result_arr [n];

    int res = move_n_to_array(buffer, length, tail, head, n, result_arr);
    printf ("%d: %d , %d , %d , %d , %d\n" ,res , arr [0] , arr [1] , arr [2] , arr [3] , arr [4]);
    printf ("Value from buffer: %d, %d, %d\n", result_arr[0], result_arr[1], result_arr[2]);
    return 0;
}