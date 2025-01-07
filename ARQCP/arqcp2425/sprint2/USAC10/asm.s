.section .data
.section .text
    .global median  
median:
    # Save registers
    pushq %rdi
    pushq %rsi
    pushq %rdx
    pushq %rcx
    pushq %r9
    pushq %r10
    movq %r8, %r10

    # Initialize return value to 0
    movq $0, %rax

    # Check if length is <= 0
    cmpl $0, %esi
    jle end

    # Save the output address (me)
    movq %rdx, %r8
    pushq %r8

    # Sort the array (input pointer in %rdi, length in %esi)
    movb $'1', %cl   # Order flag for sorting
    call sort_array
    popq %r8

    # Prepare to calculate the median
    addl $1, %esi    # Increment length (for integer division logic)
    movl %esi, %eax  # Copy length to %eax
    xorl %edx, %edx  # Clear %edx (upper half of dividend)
    movl $2, %ecx    # Divisor = 2
    divl %ecx        # %eax = length / 2, %edx = remainder

    # Calculate median based on odd/even length
    cmpl $1, %edx    # Check remainder (%edx)
    je impar         # Odd case
    jne par          # Even case (explicit for clarity)

impar:
    # Odd length: Median is the middle element
    movl (%rdi, %rax, 4), %r9d  # Load median value
    movq $1, %rax               # Return success
    jmp end

par:
    # Even length: Average two middle elements
    movl (%rdi, %rax, 4), %r9d  # Load first middle value
    subq $1, %rax               # Decrement index
    addl (%rdi, %rax, 4), %r9d  # Add second middle value
    sarl $1, %r9d               # Divide by 2 (right shift)
    movq $1, %rax               # Return success
    jmp end

end:
    # Store the calculated median in the output address
    movl %r9d, (%r8)

    movq %r10, %r8
    # Restore registers
    popq %r10
    popq %r9
    popq %rcx
    popq %rdx
    popq %rsi
    popq %rdi

    # Return
    ret
