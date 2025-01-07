.section .data
    .global op1
    .global op2
    .equ CONST, 20  

.section .text

    .global sum # int sum(void)
        sum:
        movl op1(%rip), %ecx #place op1 in ecx
        movl op2(%rip), %eax #place op2 in eax
        addl %ecx, %eax      #add ecx to eax. Result is in eax

    .global another_sum # long another_sum() 
        another_sum:
        movq $CONST, %rcx 
        movq $CONST, %rax 
        movslq op1(%rip), %r9 
        movslq op2(%rip), %r10 

        
        subq %r9, %rcx
        subq %r10, %rax
        addq %rcx, %rax
        addq $CONST, %rax
    ret
    