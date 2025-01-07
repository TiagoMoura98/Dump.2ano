#include <stdio.h>

// Declaration of the median function implemented in assembly
extern int median(int* vec, int length, int *me);

int main() {
    int length;

    // Input and validate the length of the array
    printf("Enter the length of the array (positive integer): ");
    while (scanf("%d", &length) != 1 || length <= 0) {
        while (getchar() != '\n'); // Clear invalid input
        printf("Invalid input. Please enter a positive integer for the length: ");
    }

    int vec[length];

    // Input the elements of the array
    printf("Enter %d integers for the array:\n", length);
    for (int i = 0; i < length; i++) {
        printf("Element %d: ", i);
        while (scanf("%d", &vec[i]) != 1) {
            while (getchar() != '\n'); // Clear invalid input
            printf("Invalid input. Please enter an integer for element %d: ", i);
        }
    }

    int me = 0;
    int *ptr = &me;

    // Call the median function
    int result = median(vec, length, ptr);

    // Print the results
    printf("Success?: %d\n", result);
    printf("The median is: %d\n", me);

    return 0;
}
