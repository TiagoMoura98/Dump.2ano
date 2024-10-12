

void swap_nums(int *x, int *y){
    int tmp;
    tmp = *x;
    *x = *y;
    *y = tmp;
}

void swap_pointers(char **x, char **y){
    char * tmp;
    tmp = *x;
    *x = *y;
    *y = tmp;
}