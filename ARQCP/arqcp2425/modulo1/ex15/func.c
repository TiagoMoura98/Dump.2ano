void trim_string(char *str) {
    char temp_array[100];
    char *dest = temp_array;
    char *src = str;

    // Retirar todos os espaços do início e do fim 
    // Deixar só 1 espaço entre palavras

    while (*str) {
        // Ignorar espaços no início
        while (*str == ' ') {
            ++str;
        }

        // Copiar caracteres até encontrar um espaço ou fim da string
        while (*str != ' ' && *str != '\n' && *str != '\0' && *str != '\t') {
            *dest = *str;
            ++dest;
            ++str;
        }

        // Ignorar múltiplos espaços entre palavras
        while (*str == ' ') {
            ++str;
        }

        // Adicionar um único espaço entre palavras, se não for o fim da string
        if (*str) {
            *dest = ' ';
            ++dest;
        }
    }

    // Terminar a string temporária
    *dest = '\0';

    // Copiar de volta para o array original
    char *copy = temp_array;
    while (*copy) {
        *src = *copy;
        ++src;
        ++copy;
    }
    // Para finalizar o array
    *src = '\0';
}
