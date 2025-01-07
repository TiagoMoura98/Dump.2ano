.section .data
.section .note.GNU-stack,"",@progbits

.section .text
    .global get_n_element

get_n_element:
    # int get_n_element(int* buffer, int length, int* tail, int* head)
    # rdi - buffer (base address of the array)
    # esi - length (length of the array)
    # rdx - tail (pointer to the index of the oldest element)
    # rcx - head (pointer to the index of the next insertion position)
    
    cmp $0, %esi            # Check if length == 0
    je empty_buffer
    
    movl (%rcx), %eax
    cmp (%rdx), %eax          # Check for empty buffer: head == tail
    je empty_buffer
    
    movl (%rcx), %eax
    cmp (%rdi), %eax        # Check if head < buffer or head >= buffer + length
    jl out_of_bounds
    movl (%rcx), %eax
    cmp %esi, %eax          # or head >= buffer + length   
    jge out_of_bounds
    
    movl (%rdx), %eax
    cmp (%rdi), %eax        # Check if tail < buffer or tail >= buffer + length * 4
    jl out_of_bounds
    cmp %esi, %eax
    jge out_of_bounds

    # Calculate number of elements in the buffer
    movl (%rcx), %eax
    cmp (%rdx), %eax
    jl wrap_around          # If head < tail, handle wrap-around case

    movl (%rcx), %eax       # Moving the pointed by the address in rcx to eax - Normal case: head > tail   
    subl (%rdx), %eax       # Return result in %eax  (result = head - tail)
      
    jmp done

wrap_around:
    # Wrap-around case: head < tail
    movl (%rcx), %eax
    addl %esi, %eax         # result = head + length
    subl (%rdx), %eax       # result = result - tail
    
done:
    # Epilogue
    ret

out_of_bounds:
    mov $-1, %eax           # Return -1 for out-of-bounds error
    jmp done

empty_buffer:
    mov $0, %eax            # Return 0 for empty buffer
    jmp done
