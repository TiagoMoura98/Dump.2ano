.section .data
.section .text

    .global max     #int max(int a, int b int c)
        max:
        movq $0, %rax

        cmpq %rsi, %rdi   #rdi>rsi
        jl end
        cmpq %rdx, %rdi
        jl end

        movq $1, %rax

        end:
    
    .global count_max    #int count_max(int* ptr, int num)  vi < vi+1 > vi+2
        count_max:
        
        pushq %rsi
        pushq %rdi

        subq $2, %rsi 
        movq %rsi, %rcx
        movq $0, %r8     #counter
        

        array_loop:
            movq (%rdi, %r9, 4), %r10
            movq 
            call max

        

    ret
    