.data

stdoutchar:
   .asciz "0"

.text

.globl main
main:
   ret

.globl printchar
printchar:
   mov stdoutchar@GOTPCREL(%rip), %rsi
   mov %al, (%rsi)
   mov $1, %rax
   mov $1, %rdi
   mov $stdoutchar, %rsi
   mov $1, %rdx
   syscall

   ret
