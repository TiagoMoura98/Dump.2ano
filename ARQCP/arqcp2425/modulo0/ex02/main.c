#include <stdio.h>

// Function to return the sum of two integers
int sum(int a, int b) {
    return a + b;
}

int main() {
    int x = 5, y = 7;

    // Call the sum function and print the result
    printf("Sum of %d and %d is: %d\n", x, y, sum(x, y));

    return 0;
}