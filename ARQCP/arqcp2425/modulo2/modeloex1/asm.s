.section .data
.section .text

    .global max # int max(int a, int b int c)
        max:
        movq $0, %rax

        cmpq %rsi, %rdi   #rdi>rsi
        jl end
        cmpq %rdx, %rdi
        jl end

        movq $1, %rax

        end:
    ret
    