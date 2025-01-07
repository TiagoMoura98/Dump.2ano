.section .note.GNU-stack,"",@progbits

.section .text
    .global median
    .global sort_array

median:
    # int median(int* vec, int length, int* me)
    # rdi - vec (base address of the array)
    # esi - length (length of the array)
    # rdx - me (pointer of the median value)
    
    # Prologue
    pushq %rbx
    pushq %r12

    # Verificar se length <= 0
    cmpq $0, %rsi           # Comparar length com 0
    jle fail                # Se length <= 0, saltar para fail
    
    # Verificar se array tem 1 elemento
	cmpq $1, %rsi
	je single_element

    # Chamar sort_array(vec, length, 1)
    pushq %rdx
    pushq %rdi
    pushq %rsi
    movq $1, %rdx			# Terceiro argumento (order = 1, ordem ascendente)
    call sort_array         # Chamar a função externa sort_array
    popq %rsi
    popq %rdi
    popq %rdx
    
    testl %eax, %eax        # Verificar o valor de retorno (0 = falha)
    jz fail                 # Se falhar, saltar para fail

    # Calcular a mediana
    movl %esi, %eax         # Copiar length para %eax
    shrl $1, %eax           # length / 2
    movq %rdi, %rbx         # Guardar o ponteiro vec em %rbx

	testl $1, %esi          # Verifica se length é par ou ímpar
    jnz odd                 # Se ímpar, saltar para o caso odd

even:
    # Mediana para length par: (vec[length/2 - 1] + vec[length/2]) / 2
    movl (%rbx, %rax, 4), %r12d  # vec[length / 2]
    subl $1, %eax                # length / 2 - 1
    addl (%rbx, %rax, 4), %r12d  # Adicionar vec[length / 2 - 1]
    sarl $1, %r12d               # Dividir por 2
    jmp store_result

odd:
    # Mediana para length ímpar: vec[length/2]
    movl (%rbx, %rax, 4), %r12d  # vec[length / 2]

store_result:
    movl %r12d, (%rdx)     # Guardar o valor da mediana em *me
	jmp success
	
single_element:
	movl (%rdi), %r12d            # Carrega o único elemento
	movl %r12d, (%rdx)            # Armazena em *me

success:
	movl $1, %eax		   # Retornar 1 (sucesso)
	jmp end
	
fail:
    movl $0, %eax          # Retornar 0 (falha)

end:
    # Epilogue
    popq %r12
    popq %rbx
    ret
