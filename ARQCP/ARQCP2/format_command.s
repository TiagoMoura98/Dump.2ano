 .section .note.GNU-stack,"",@progbits

 .section .text
    .global format_command

format_command:
str_trimmer:
    movb (%rdi), %al
    cmpb $0, %al          
    je end_trimmer

    cmpb $32, %al         
    je spc_found

str_found:
    cmpb $97, %al         
    jge capitalize
                 
process_char:
    movb %al, (%rdx)      
    incq %rdx
    incq %rdi
    jmp str_trimmer

spc_found:
    incq %rdi             
    jne str_trimmer

end_trimmer:
    movb $44, (%rdx)      
    incq %rdx
    jmp get_binary_number

capitalize:
    subb $32, %al         
    jmp process_char

get_binary_number:
    movl %esi, %eax       
    cmp $32, %eax         
    jge invalid_input               
    cmp $0, %eax         
    jle invalid_input      
    
L1:
    cmp $16, %eax
    jge L2
    movb $'0', (%rdx)
    incq %rdx
    movb $44, (%rdx)      
    incq %rdx
    jmp L3
L2:
    movb $'1', (%rdx)
    incq %rdx
    movb $44, (%rdx)
    incq %rdx
    subl $16, %eax
L3:
    cmp $8, %eax
    jge L4
    movb $'0', (%rdx)
    incq %rdx
    movb $44, (%rdx)
    incq %rdx
    jmp L5
L4:
    movb $'1', (%rdx)
    incq %rdx
    movb $44, (%rdx)
    incq %rdx
    subl $8, %eax
L5:
    cmp $4, %eax
    jge L6
    movb $'0', (%rdx)
    incq %rdx
    movb $44, (%rdx)
    incq %rdx
    jmp L7
L6:
    movb $'1', (%rdx)
    incq %rdx
    movb $44, (%rdx)
    incq %rdx
    subl $4, %eax
L7:
    cmp $2, %eax
    jge L8
    movb $'0', (%rdx)
    incq %rdx
    movb $44, (%rdx)
    incq %rdx
    jmp L9
L8:
    movb $'1', (%rdx)
    incq %rdx
    movb $44, (%rdx)
    incq %rdx
    subl $2, %eax
L9:
    cmp $1, %eax
    jge L10
    movb $'0', (%rdx)
    incq %rdx
    jmp L11
L10:
    movb $'1', (%rdx)
    incq %rdx
L11:
    movb $0, (%rdx)       
    movl $1, %eax        
    jmp end
    
invalid_input:
    movl $0, %eax         
    jmp end               

end:
    ret
