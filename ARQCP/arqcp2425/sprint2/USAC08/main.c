#include <stdio.h>
#include "asm.h"

int main(void) {
    int length;

    // Input and validate the length of the array
    printf("Enter the length of the array (positive integer): ");
    while (scanf("%d", &length) != 1 || length <= 0) {
        while (getchar() != '\n'); // Clear invalid input
        printf("Invalid input. Please enter a positive integer for the length: ");
    }

    int arr[length];

    // Input the elements of the array
    printf("Enter %d integers for the array:\n", length);
    for (int i = 0; i < length; i++) {
        printf("Element %d: ", i);
        while (scanf("%d", &arr[i]) != 1) {
            while (getchar() != '\n'); // Clear invalid input
            printf("Invalid input. Please enter an integer for element %d: ", i);
        }
    }

    // Input and validate the head index
    int head;
    printf("Enter the head index (0 to %d): ", length - 1);
    while (scanf("%d", &head) != 1 || head < 0 || head >= length) {
        while (getchar() != '\n'); // Clear invalid input
        printf("Invalid input. Please enter an integer for the head index (0 to %d): ", length - 1);
    }
    int* ptr_head = &head;

    // Input and validate the tail index
    int tail;
    printf("Enter the tail index (0 to %d): ", length - 1);
    while (scanf("%d", &tail) != 1 || tail < 0 || tail >= length) {
        while (getchar() != '\n'); // Clear invalid input
        printf("Invalid input. Please enter an integer for the tail index (0 to %d): ", length - 1);
    }
    int* ptr_tail = &tail;

    // Input and validate the value of n
    int n;
    printf("Enter the number of elements to move (n, 1 to %d): ", length);
    while (scanf("%d", &n) != 1 || n <= 0 || n > length) {
        while (getchar() != '\n'); // Clear invalid input
        printf("Invalid input. Please enter a positive integer for n (1 to %d): ", length);
    }

    int result_arr[n];

    // Call the move_n_to_array function
    int res = move_n_to_array(arr, length, ptr_tail, ptr_head, n, result_arr);

    // Print the updated array
    printf("%d: ", res);
    for (int i = 0; i < length; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    // Print the resulting array
    printf("Values moved to result array: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", result_arr[i]);
    }
    printf("\n");

    return 0;
}
