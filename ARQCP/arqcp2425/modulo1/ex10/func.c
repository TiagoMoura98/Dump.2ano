char* where_is(char *str, char c){

    while (*str != '\0')
    {
        if( *str == c){
            return str;
        }
        else{
            ++str;
        }
    }
    
    return;

}