void format_word(char *str){

    if(*str && *str >= 'a' && *str <= 'z'){
        *str = *str - 32;
    }
    ++str;
    while(*str != ' ' && *str != '\0' && *str != '\t' && *str != '\n'){
        if(*str <= 'Z' && *str >= 'A'){
            *str = *str + 32;
        }
        ++str;
    }

}