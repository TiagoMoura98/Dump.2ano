int sum_odd(int *p){
    int size = *p;
    int sum = 0;
    ++p;

    for (int i = 1; i <= size; i++ , ++p)
    {
        if(*p % 2 != 0){
            sum += *p;
        }
    }
    
    return sum;
}