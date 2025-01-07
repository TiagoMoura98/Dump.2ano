.section .data
.section .text
    .global dequeue_value
    dequeue_value:

    pushq %r9
    pushq %rsi
    pushq %r10

    movq $0, %rax 
    movl (%rdx), %r9d
    cmpl (%rcx), %r9d
    je end              #buffer vazio

    subq $1, %rsi       #length-1  = indice mais alto 
    movl (%rdi, %r9, 4), %r10d
    movl %r10d, (%r8)
    addq $1, (%rdx)     #incrementa tail

    movq $1, %rax
    cmpq %rsi, (%rdx)
    jl move_to_start
    jmp end

    move_to_start:
    movl $0, (%rdx)

    end:
    popq %r10
    popq %rsi
    popq %r9

    ret
