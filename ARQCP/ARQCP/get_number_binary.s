.section .data

.section .note.GNU-stack,"",@progbits

.section .text
    .global get_number_binary
    get_number_binary:

    pushq %rdi
    pushq %rsi

    movq $0, %rax  #returns 0 if its not possible to place the binary representation on the destination array
    cmpl $31, %edi  #compares if int value is greater than 31
    jg end
    cmpl $0, %edi  #compares if int value is below 0
    jl end

    movq $5, %rdx   #counter for loop, number of bits for the integer values we are working with 
    movq %rsi, %rcx
    jmp reset_string

    int_bytes:
        movq $1, %rax  #operation success
        cmpq $0, %rdx         
        je end
        
        subq $1, %rdx
        shr $1, %dil    #right shift for 1 bit
        jc carry_set    # Jump to 'carry_set' label if the carry flag is set
        jnc carry_clear # Jump to 'carry_clear' label if the carry flag is not
        jmp int_bytes

    jmp end

    reset_string:
        movb $0, (%rsi)
        addq $1, %rsi
        addq $1, %rax
        cmpq $5, %rax
        jl reset_string
        movq %rcx, %rsi
        je int_bytes

    carry_set:
        movb $1, (%rsi)    #moves LSB that we shifted "out" to the destination string
        addq $1, %rsi
        jmp int_bytes

    carry_clear:
        movb $0, (%rsi)    #moves LSB that we shifted "out" to the destination string
        addq $1, %rsi
        jmp int_bytes

    end:
    popq %rsi
    popq %rdi
ret


