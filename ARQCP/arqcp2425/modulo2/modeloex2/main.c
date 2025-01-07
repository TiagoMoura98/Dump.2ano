#include <stdio.h>
#include "asm.h"

int main(void) {
    
    int vec[] = {1,3,4,6,3,8};
    int r = count_max(vec, 6);
    printf("%d\n", r);
    
    return 0;
}