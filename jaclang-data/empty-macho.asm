global start
section .data

stdoutchar: db 0
section .bss

section .text
                
start:


   mov     rax, 0x2000001
   mov     rdi, 0
   syscall

printchar:
   mov BYTE [rel stdoutchar], ah
   mov rax, 0x2000004
   mov rdi, 1
   mov rsi, stdoutchar
   mov rdx, 1
   syscall

   ret
