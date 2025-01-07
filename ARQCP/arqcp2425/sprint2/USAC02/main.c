#include <stdio.h>
#include "asm.h"

int main(void) {
    int value;
    char bits[5];
    int res;

    // Input and validation loop
    printf("Enter a positive integer (0-31): ");
    while (scanf("%d", &value) != 1 || value < 0 || value > 31) {
        // Clear the input buffer in case of invalid input
        while (getchar() != '\n');
        printf("Invalid input. Please enter a positive integer (0-31): ");
    }

    res = get_number_binary(value, bits);

    printf("%d: %d, %d, %d, %d, %d\n", res, bits[4], bits[3], bits[2], bits[1], bits[0]);

    return 0;
}
