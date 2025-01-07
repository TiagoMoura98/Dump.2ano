int count_words(char *str){
int counter = 0;
    // percorrer a string ate ao fim e encontrar null
    while (*str){
        // se for uma letra e nao um espaco 
        if (*str != ' ' && *str != '\n' && *str != '\t' && *str != '\0') {
            //enquanto nao encontramos um espaco vamos "varrer" a palavra toda
            while (*str != '\n' && *str != '\0' && *str != ' ' && *str != '\t'){
                ++str;
            }
            //no fim de percorrer a palavra incrementamos o counter de palavras
            counter ++;
        }else{
            //se nao for uma letra movemos o pointer para o endereco seguinte
            ++str;
        }

    }

    return counter;
}