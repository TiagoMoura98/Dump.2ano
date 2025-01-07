.section .data
.section .text
    .global move_n_to_array
    move_n_to_array:
    movq $0, %rax  #returns 0 if its not possible to place the binary representation on the destination array

        push %rdi
        push %rcx

        movq %rsi, %rcx #counter for loop to find max address
        subq $1, %rcx

        max_calc:
            addq $4, %rdi       # increments pointer to the next int in the buffer
            loop max_calc

        movq %r8, %rcx  #counter for loop to move n elements

        move_to_other_arr:
            movl (%rdx), %r10d  #moves the oldest element to the
            movl %r10d ,(%r9)   #new array
            movl $0, (%rdx)     #makes the oldest element equal to 0
            addq $4, %r9        #increments pointer of new array
            subq $1, %rcx
            cmpq $0, %rcx
            je  end
            jmp tail_incrementer

        tail_incrementer:
            cmpq %rdx, %rdi  #confirms if the tail is at the limit of the buffer
            jle move_to_start
            addq $4, %rdx   #if its not in the limit we just increase the pointer to the next int
            jmp move_to_other_arr

        move_to_start:
            movq %rdi, %rdx
            jmp move_to_other_arr

        end:
        popq %rcx
        popq %rdi
        movq $1, %rax  #operation success
ret
