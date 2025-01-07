.section .data
.section .note.GNU-stack,"",@progbits

.section .text
    .global enqueue_value

# %rdi = int* buffer
# %rsi = int bufferSize
# %rdx = int* tail
# %rcx = int* head
# %r8  = int newValue

enqueue_value:
    # carregar os valores de head e tail
    movl (%rdx), %r9d              # %r9d = *tail (índice do tail)
    movl (%rcx), %r10d             # %r10d = *head (índice do head)

    # adicionar o novo valor no buffer na posição indicada por head
    movl %r8d, (%rdi, %r10, 4)     # buffer[head] = newValue

    # update do valor de head
    incl %r10d                     # head++
    cmpl %esi, %r10d               # if head >= bufferSize
    jl no_reset_head
    xorl %r10d, %r10d              # head = 0 (reset para o início do buffer)

no_reset_head:
    # guardar o valor atualizado de head
    movl %r10d, (%rcx)             # *head = head

    # check if buffer está cheio (head ficou = a tail após a inserção)
    cmpl %r10d, %r9d               # if head == tail (buffer cheio)
    jne not_full

    # if buffer cheio, atualizar tail para o próximo índice
    incl %r9d                      # tail++
    cmpl %esi, %r9d                # if tail >= bufferSize
    jl no_reset_tail
    xorl %r9d, %r9d                # tail = 0 (reset para o início do buffer)

no_reset_tail:
    # guardar o valor atualizado de tail
    movl %r9d, (%rdx)              # *tail = tail
    movl $1, %eax                  # 1 = buffer está cheio
    ret

not_full:
    movl $0, %eax                  # 0 = buffer não está cheio
    ret
