#include <stdio.h>

int main() {
    // Step 1: Declare variables
    int x = 5;
    int* ptr_x = &x;
    float y = *ptr_x + 3;

    // Print values and addresses
    printf("x = %d, y = %.2f\n", x, y);
    printf("Address of x: %p\n", (void*)&x);
    printf("Address of y: %p\n", (void*)&y);
    printf("Address of ptr_x: %p\n", (void*)&ptr_x);
    printf("Value of ptr_x (address it points to): %p\n", (void*)ptr_x);
    printf("Value pointed by ptr_x: %d\n", *ptr_x);

    // Step 2: Declare an array and pointers
    int vec[] = {10, 20, 30, 40};
    int* ptr_vec = vec;
    int z = *ptr_vec;      // First element of vec (10)
    int h = *(ptr_vec + 3); // Fourth element of vec (40)

    // Print values and addresses
    printf("z = %d, h = %d\n", z, h);
    printf("Address of vec: %p\n", (void*)vec);
    printf("Address of ptr_vec: %p\n", (void*)&ptr_vec);
    printf("Value of ptr_vec (address it points to): %p\n", (void*)ptr_vec);
    printf("Value pointed by ptr_vec: %d\n", *ptr_vec);

    // Step 3: Print elements and addresses
    int i;
    for (i = 0; i < 4; i++) {
        printf("1: %p, %d\t", (void*)&vec[i], vec[i]);
    }
    printf("\n");
    
    for (ptr_vec = vec; ptr_vec < vec + 4; ptr_vec++) {
        printf("2: %p, %d\t", (void*)ptr_vec, *ptr_vec);
    }
    printf("\n");

    for (ptr_vec = vec + 3; ptr_vec >= vec; ptr_vec--) {
        printf("3: %p, %d\t", (void*)ptr_vec, *ptr_vec);
    }
    printf("\n");

    // Step 4: Pointer arithmetic
    ptr_vec = vec;
    printf("4: %p, %d\n", (void*)ptr_vec, *ptr_vec);

    int a = *ptr_vec++;
    printf("5: %p, %d, %d\n", (void*)ptr_vec, *ptr_vec, a);

    ptr_vec = vec;
    a = (*ptr_vec)++;
    printf("6: %p, %d, %d\n", (void*)ptr_vec, *ptr_vec, a);

    ptr_vec = vec;
    a = *++ptr_vec;
    printf("7: %p, %d, %d\n", (void*)ptr_vec, *ptr_vec, a);

    ptr_vec = vec;
    a = ++*ptr_vec;
    printf("8: %p, %d, %d\n", (void*)ptr_vec, *ptr_vec, a);

    for (ptr_vec = vec; ptr_vec < vec + 4; ptr_vec++) {
        printf("9: %p, %d\t", (void*)ptr_vec, *ptr_vec);
    }
    printf("\n");

    // Step 5: Inspect memory layout
    unsigned int d = 0xAABBCCDD;
    printf("10: %p, %x\n", (void*)&d, d);

    unsigned char* ptr_d = (unsigned char*)&d;
    unsigned char* p;
    for (p = ptr_d; p < ptr_d + sizeof(unsigned int); p++) {
        printf("11: %p, %x\t", (void*)p, (unsigned char)*p);
    }
    printf("\n");

    return 0;
}
