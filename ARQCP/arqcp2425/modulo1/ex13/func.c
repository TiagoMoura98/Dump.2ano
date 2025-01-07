#include <stdbool.h>

void array_sort(short *vec, int n) {
    bool mexeu = true;
    // Guardar o endereço da posição inicial
    short *ptr = vec;
    short tmp, tmp2;

    while (mexeu) {
        // vec volta a apontar para o valor inicial
        vec = ptr;
        // Reset da flag se mexeu ou não na ordem
        mexeu = false;
        for (int i = 0; i < n - 1; i++) {
            // Guarda o valor da posição atual
            tmp = *vec;
            // Guarda o valor da posição seguinte
            ++vec;
            tmp2 = *vec;
            --vec;
            // Se o atual for maior que o seguinte, trocamos
            if (tmp > tmp2) {
                *vec = tmp2;
                vec++;
                *vec = tmp;
                mexeu = true;
            } else {
                vec++;
            }
        }
    }
}

int sort_without_reps(short *src, int n, short *dest){

bool repetido = false;
short *inicioDest = dest;
short *tempDest = dest;
int totalCopied = 0;

    for (int i = 0; i < n; i++, ++src)
    {
        //guardar a posicao em que estamos no arr destino
        tempDest = dest;

        dest = inicioDest;
        //vamos procurar no arr de destino se ja existe o valor que estamos a apontar atualmente com o src
        for (int j = 0; j < totalCopied; j++, ++dest)
        {
            if(*src == *dest){
                repetido = true;
            }
        }
        //se nao for repetido alteramos o valor
        if(repetido == false){
            dest = tempDest;
            *dest = *src;
            totalCopied ++;
            ++dest;
        }else{
            // reset da flag
            repetido = false;
        }
        
    }
    
    array_sort(inicioDest, totalCopied);
    return totalCopied;

}


