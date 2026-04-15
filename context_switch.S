// void context_switch(void **old_sp, void *new_sp)
//   rcx = old_sp, rdx = new_sp  (Windows x64)
//
// Windows x64 callee-saved: rbx, rbp, rdi, rsi, r12~r15 (rdi, rsi 포함이 Linux 와 다른 점)

.text
.globl context_switch
context_switch:
    push %rbx
    push %rbp
    push %rdi
    push %rsi
    push %r12
    push %r13
    push %r14
    push %r15

    mov %rsp, (%rcx)
    mov %rdx, %rsp

    pop %r15
    pop %r14
    pop %r13
    pop %r12
    pop %rsi
    pop %rdi
    pop %rbp
    pop %rbx
    ret
