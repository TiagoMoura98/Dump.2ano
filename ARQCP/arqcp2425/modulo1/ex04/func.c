

void capitalize (char *x){

    while(*x != '\0'){

        if (*x >= 97){
            *x = *x - 32;
        }
        
        ++x;
    }
}