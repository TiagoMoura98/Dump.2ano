#include <stdbool.h>

void array_sort(int *vec, int n){

bool mexeu = true;
//guardar o endereco da posicao inicial
int *ptr = vec;
int tmp, tmp2;

   while (mexeu)
   {
    //vec volta a apontar para o valor inicial
    vec = ptr;
    //reset da flag se mexeu ou nao na ordem
    mexeu = false;
    for (int i = 0; i < n - 1; i++)
    {
        //guarda o valor da posicao atual
        tmp = *vec;
        //guarda o valor da posicao seguinte
        ++vec;
        tmp2 = *vec;
        --vec;
        //se o atual for maior que o seguinte trocamos
        if(tmp > tmp2){
            *vec = tmp2;
            vec++;
            *vec = tmp;
            mexeu = true;
        }else{
            vec++;
        } 
    }
   }
}

