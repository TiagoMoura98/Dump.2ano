#include <stdio.h>
#include "func.h"


int main(){

    int vec1 [] = {0,1,2,3,4};
    int vec2 [] = {5,6,7,8,9};
    int *ptr_vec2 = vec2;

    copy_vec(vec1,3,vec2);

    while(*ptr_vec2 != 9){
        printf("%d",*ptr_vec2);
        ++ptr_vec2;
    }
    
    return 0;
}