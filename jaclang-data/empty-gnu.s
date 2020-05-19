.bss
.data

stdoutchar:
   .asciz "0"

.text

.globl main
main:

   movq $60, %rax
   movq $0, %rdi
   syscall

printchar:
   mov stdoutchar@GOTPCREL(%rip), %rsi
   mov %al, (%rsi)
   mov $1, %rax
   mov $1, %rdi
   mov $stdoutchar, %rsi
   mov $1, %rdx
   syscall

   ret

.macro pusha
   push %rbx
   push %rcx
   push %rsi
   push %rdi
   push %r8
   push %r9
   push %r10
   push %r11
   push %r12
   push %r13
   push %r14
   push %r15
   push %rbp
.endm

.macro popa
   pop %rbp
   pop %r15
   pop %r14
   pop %r13
   pop %r12
   pop %r11
   pop %r10
   pop %r9
   pop %r8
   pop %rdi
   pop %rsi
   pop %rcx
   pop %rbx
.endm

