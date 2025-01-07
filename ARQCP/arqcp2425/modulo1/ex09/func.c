void get_array_statistics(int* vec, int n, int* min, int* max, float* avg){

    if(n > 0){
    int min_temp = *vec;
    int max_temp = *vec;
    float avg_temp = *vec;

    ++vec;

        for (int i = 1; i < n; i++)
        {
            if(*vec < min_temp){
                min_temp = *vec;
            }
            
            if(*vec > max_temp){
                max_temp = *vec;
            }
            
            //increments the avg with the current pointed value
            avg_temp += *vec;
            //increments the pointer
            ++vec;
        }
        
        avg_temp = avg_temp / n;

        *min = min_temp;
        *max = max_temp;
        *avg = avg_temp;
        }
        else{
        *min = 0;
        *max = 0;
        *avg = 0;
        }
        
} 