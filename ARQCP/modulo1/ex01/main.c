#include <stdio.h>

int main()
{

    int x = 5;
    int *ptr_x = &x;
    float y = *ptr_x + 3;

    printf("Value of x is %d\n", x);
    printf("Value of y is %f\n", y);

    printf("Address of x is %p\n", &x);
    printf("Address of y is %p\n", &y);
    printf("Address of ptr_x is %p\n", &ptr_x);

    printf("Value of ptr_x is %p\n", ptr_x);
    printf("Value pointed by ptr_x is %p\n", *ptr_x);

    int vec[] = {10, 20, 30, 40};
    int *ptr_vec = vec;
    int z = *ptr_vec;
    int h = *(ptr_vec + 3);

    printf("Value of z is %d\n", z);
    printf("Value of h is %d\n", h);
    printf("Address of vec is %p\n", &vec);
    printf("Address of ptr_vec is %p\n", &ptr_vec);
    printf("Value of ptr_vec is %p\n", ptr_vec);
    printf("Value of vec is %p\n", vec);
    printf("Value pointed by ptr_vec is %d\n", *ptr_vec);

    // address of vec and value of ptr_vec
    // the address of vec is the lowest address of it's indexes in this case is where is stored the value 10
    // ptr_vec stores the value that identifies vec, which is the address of the index [0]
    // so in this case they are the same

    int i;
    for (i = 0; i < 4; i++)
    {
        printf("1: %p,%d\t", &vec[i], vec[i]);
    }
    // the for cycle above prints the memory address of the index in the array we are iterating followed by the value stored in that
    // memory address

    printf("\n");
    for (ptr_vec = vec; ptr_vec < vec + 4; ptr_vec++)
    {
        printf("2: %p,%d\t", ptr_vec, *ptr_vec);
    }
    // we start by storing the address of vec in the ptr_vec, then we will run this cycle while the ptr_vec doesn't reach
    // the index 4 in the array (which doesnt exist, so we will get trough the whole array), then we increment the pointer,
    // which makes him "point" to the next address where is stored the next index position of the array
    // for each cycle it prints the address where is pointing to and the value that its stored there

    printf("\n");
    for (ptr_vec = vec + 3; ptr_vec >= vec; ptr_vec--)
    {
        printf("3: %p,%d\t", ptr_vec, *ptr_vec);
    }
    // does the same as the cycle above, but in reverse order. From last to first

    // What does ptr_vec++ or ptr_vec–-?
    // ptr_vec++ increments the pointer making it "point" to the next address, this depends on the type of pointer
    // if its integer increments 4 memory addresses, while if its char goes 1 by 1
    // the ptr_vec-- does the same but subtracting

    int a;
    printf("\n");
    ptr_vec = vec;
    // stores the index 0 address in the pointer
    printf("4: %p,%d\n", ptr_vec, *ptr_vec);
    // prints the address pointed by the pointer and the value stored there
    a = *ptr_vec++;
    // stores in var a the value pointed by the pointer but we incremented the pointer and since is an integer moves 4 bytes to
    // the next index of the array, in this case is 20
    printf("5: %p,%d,%d\n", ptr_vec, *ptr_vec, a);
    // prints the new address pointed by the pointer which is the index 1 of the array, then the value pointed by it which is 20
    // then the value of a which is 20
    ptr_vec = vec;
    // makes the pointer "point" again to the start of the array
    a = (*ptr_vec)++;
    // stores in the var a the value pointed by ptr_vec which is 10 + 1, making a equal to 11
    printf("6: %p,%d,%d\n", ptr_vec, *ptr_vec, a);
    // prints the address of the first index, the value stored there which is 10 , and then the value of a which is 11
    ptr_vec = vec;
    // unnacessary instruction because the pointer was already pointing to the address of vec
    a = *++ptr_vec;
    // 
    printf("7: %p,%d,%d\n", ptr_vec, *ptr_vec, a);
    ptr_vec = vec;
    a = ++*ptr_vec;
    printf("8: %p,%d,%d\n", ptr_vec, *ptr_vec, a);
    printf("\n");
    for (ptr_vec = vec; ptr_vec < vec + 4; ptr_vec++)
    {
        printf("9: %p,%d\t", ptr_vec, *ptr_vec);
    }

    //

    return 0;
}