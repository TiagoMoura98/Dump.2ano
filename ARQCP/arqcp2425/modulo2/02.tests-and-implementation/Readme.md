# USAC02 - Implement the int get_number_binary(int n, char* bits) function.

## 2. Tests and Implementation

### 2.1. Test Cases

#### Test Case 1: Valid Token
**Input:**
- `str`: "TEMP &unit:celsius &value:20#HUM&unit:percentage&value:80"
- `token`: "TEMP"

**Expected Output:**
- `unit`: "celsius"
- `value`: 20
- Return: 1

#### Test Case 2: Invalid Token
**Input:**
- `str`: "TEMP &unit:celsius &value:20#HUM&unit:percentage&value:80"
- `token`: "AAA"

**Expected Output:**
- `unit`: ""
- `value`: 0
- Return: 0

### 2.2. Implementation

The function `extract_data` will be implemented in Assembly language. The function will:
1. Search for the token in the input string.
2. If the token is found, extract the unit and value.
3. If the token is not found, set the unit to an empty string and value to zero.

### 2.3. Assembly Code

```assembly
section .data
    str db "TEMP &unit:celsius &value:20#HUM&unit:percentage&value:80", 0
    token db "TEMP", 0
    unit db 20 dup(0)
    value dd 0

section .bss

section .text
    global extract_data

extract_data:
    ;  Function implementation here
    ; Parameters:
    ;   rdi - str
    ;   rsi - token
    ;   rdx - unit
    ;   rcx - value
    ; Return:
    ;   rax - 1 if success, 0 if failure

    ; Assembly code implementation goes here

    ret
```
### 2.4. Running the Tests
To run the tests, compile the Assembly code using an assembler like NASM and link it with a C test harness. The test harness will call the extract_data function with the test cases and verify the output.

### 2.5. Test Harness in C
```c
#include <stdio.h>
#include <string.h>

extern int extract_data(char* str, char* token, char* unit, int* value);

void run_tests() {
    char str[] = "TEMP &unit:celsius &value:20#HUM&unit:percentage&value:80";
    char token1[] = "TEMP";
    char token2[] = "AAA";
    char unit[20];
    int value;
    int res;

    // Test Case 1
    res = extract_data(str, token1, unit, &value);
    printf("%d:%s,%d\n", res, unit, value); // Expected: 1:celsius,20

    // Test Case 2
    res = extract_data(str, token2, unit, &value);
    printf("%d:%s,%d\n", res, unit, value); // Expected: 0:,0
}

int main() {
    run_tests();
    return 0;
}
```