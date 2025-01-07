.section .data
.section .note.GNU-stack,"",@progbits

.section .text
    .global move_n_to_array
    move_n_to_array:

    pushq %rdi
    pushq %rsi
    pushq %rdx
    pushq %rcx
    pushq %r8
    pushq %r9
    pushq %r10

    movq $0, %rax 
    cmpl %esi, %r8d     #confirmar se o numero de elementos a fazer deqeueu é maior que o numero de elementos
    jg end
    cmpl $0, %r8d       
    je end

    movl %r8d, %r10d    #numero de elementos para fazer dequeue
    movq %r9, %r8       #endereço do array onde vamos colocar os elementos

    loop_to_move:
        call dequeue_value

        addq $4, %r8    #increment pointer do array novo
        cmpq $0, %rax   #buffer esta vazio       
        je end

        subl $1, %r10d
        movq $1, %rax
        
        cmpl $0, %r10d
        je end
        jmp loop_to_move

    end:
        popq %r10
        popq %r9
        popq %r8
        popq %rcx
        popq %rdx
        popq %rsi
        popq %rdi

    ret    
    