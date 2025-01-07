.section .data

.section .text
    .global get_number_binary
    get_number_binary:

    movq $0, %rax  #returns 0 if its not possible to place the binary representation on the destination array
    cmpq $31, %rdi  #compares if int value is greater than 31
    jg end
    cmpq $0, %rdi  #compares if int value is below 0
    jl end

    movq $5, %rcx   #counter for loop, number of bits for the integer values we are working with 

    int_bytes:
        movq %rdi, %rbx  
        andq $1, %rbx %isolamos o bit menos significativo em rbx
        movb %bl, (%rsi, %rax, 1)  #move byte from %bl to destination array 
        shrq $1, %rdi #right shift by 1 bit so we dont use the one that we just copied
        addq $1, %rax #incrementar index
        loop int_bytes

    movl $1, %eax  #operation success

    end:
ret
















