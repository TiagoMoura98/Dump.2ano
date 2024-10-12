#ifndef ASM_H 
#define ASM_H 
void init_data(int* data, int n);
void print_data(int* data, int n);
void init_data_ptrs(int* data, int n, int** ptrs);
void print_data_ptrs(int** ptrs, int n);
int * find_max_data_ptrs(int** ptrs, int n);
void sort_data_ptrs(int** ptrs, int n);
#endif 

