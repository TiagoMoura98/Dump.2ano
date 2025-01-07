void frequencies(float *grades, int n, int *freq){

    // array onde vao ser colocadas as grades so com o valor inteiro
    float *ptr_init = grades;
    int counter = 0;
    //transformamos as grades todas em so valores inteiros
    for (int p = 0; p < n; p++)
    {
        *grades = (int) *grades;
        ++grades;
    }
    grades = ptr_init;

    // aproveitamos o size que conhecemos de freq para percorrer o mesmo
    for (int i = 0; i <= 20; i++)
    {
        //percorremos o array das notas que criamos para contar quantas vezes existe cada nota
        for (int j = 0; j < n; j++){   
            // aproveitamos o iterador para comparar o valor da nota
            if(*grades == i){
                counter++;
            }
            ++grades;
        }
        // depois de "contar" guardamos o seu valor na posicao do array freq
        *freq = counter;
        ++freq;
        // reset do counter para usarmos noutra nota
        counter = 0;
        grades = ptr_init;
    }
}

