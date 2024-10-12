
int sum_integer_bytes(unsigned int *p){

    unsigned int sum = 0;
    unsigned char *ptr_p = (unsigned char *) p;

    for (int i = 0; i < 4; i++)
    {
        sum += *ptr_p;
        ++ptr_p;
    }
    

    return sum;
}