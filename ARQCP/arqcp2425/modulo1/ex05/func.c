
void copy_vec(int *vec1, int n, int *vec2){

    for (int i = 0; i < n; i++)
    {
        *vec2 = *vec1;
        ++vec2;
        ++vec1;
    }
    

}