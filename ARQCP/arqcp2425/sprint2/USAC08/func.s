.section .data
.section .text
    .global dequeue_value
    dequeue_value:

    pushq %r9
    pushq %rsi
    pushq %r10

    movq $0, %rax 
    movl (%rdx), %r9d   #indice de tail guardado em r9d           
    cmpl (%rcx), %r9d   #comparamos o indice de head
    je end              #buffer vazio

    subl $1, %esi       #length-1  = indice mais alto 
    movl (%rdi, %r9, 4), %r10d  
    movl %r10d, (%r8)   
    addq $1, (%rdx)     #incrementa indice da tail

    movq $1, %rax

    cmpl %esi, (%rdx)   #se o indice de tail for maior que a length
    jg move_to_start
    jmp end

    move_to_start:
    movl $0, (%rdx)     #coloca o indice de tail igual a 0

    end:
    popq %r10
    popq %rsi
    popq %r9

    ret
