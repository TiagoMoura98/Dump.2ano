.section .data

.section .text
    .global sort_array

# Input:
#   %rdi: (int* vec)
#   %esi: (int length)
#   %dl: order (char order), 1 para ascendente, 0 para descendente
# Output:
#   %eax: 1 se for sucesso, 0 se não for

sort_array:
    # if length <= 0, retorna 0
    cmpl $0, %esi
    jle end0

    # if length == 1, retorna 1 (nada para ordenar)
    cmpl $1, %esi
    je end1

    # prologue
    pushq %rbx                    # guardar %rbx

    # inicializar variáveis
    movl %esi, %r8d               # salvar length em %r8d
    decl %r8d                     # ajustar length para usar como limite (length - 1)
    movb %dl, %bl                 # salvar ordem em %bl (1 = crescente, 0 = decrescente)

    movl $1, %r9d                 # swapped = 1 para entrar no loop

outer_loop:
    cmpl $0, %r9d                 # verificar se houve troca
    je sorted                     # se não houve troca, array está ordenado

    movl $0, %r9d                 # reset swapped flag
    movl $0, %ecx                 # reset índice

inner_loop:
    # comparar index com (length - 1)
    cmpl %r8d, %ecx               # if index >= length - 1, fim
    jge outer_loop                # se alcançou o fim, sair do loop

    # carregar vec[index] em %eax
    movl (%rdi, %rcx, 4), %eax    # %eax = vec[index]

    # carregar vec[index + 1] em %edx
    movl 4(%rdi, %rcx, 4), %edx   # %edx = vec[index + 1]

    # comparar vi e vi+1
    cmpb $1, %bl                  # verificar ordem
    je ascending_order            # se ordem = crescente

    # ordem descendente
    cmpl %edx, %eax               # comparar vec[index] e vec[index + 1]
    jge no_swap                   # se vec[index] >= vec[index + 1], está na ordem correta

    # trocar vec[index] e vec[index + 1]
    movl %edx, (%rdi, %rcx, 4)    # vec[index] = vec[index + 1]
    movl %eax, 4(%rdi, %rcx, 4)   # vec[index + 1] = vec[index]
    movl $1, %r9d                 # flag a 1
    jmp continue_loop

no_swap:
    # não trocar vec[index] e vec[index + 1]
    jmp continue_loop

ascending_order:
    # ordem ascendente
    cmpl %edx, %eax               # comparar vec[index] e vec[index + 1]
    jle no_swap                   # se vec[index] <= vec[index + 1], está na ordem correta

    # trocar vec[index] e vec[index + 1]
    movl %edx, (%rdi, %rcx, 4)    # vec[index] = vec[index + 1]
    movl %eax, 4(%rdi, %rcx, 4)   # vec[index + 1] = vec[index]
    movl $1, %r9d                 # flag a 1

continue_loop:
    incl %ecx                     # incrementar índice
    jmp inner_loop

sorted:
    # epilogue
    popq %rbx

    jmp end1

end1:
    movl $1, %eax                 # retornar 1 (sucesso)
    ret

end0:
    movl $0, %eax                 # retornar 0 (insucesso)
    ret
