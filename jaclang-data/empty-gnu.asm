section .data

stdoutchar: db 0
section .bss

section .text

    global _start
_start:

   mov eax, 60
   mov edi, 0
   syscall

printchar:
   mov BYTE [stdoutchar], ah
   mov rax, 1
   mov rdi, 1
   mov rsi, stdoutchar
   mov rdx, 1
   syscall

   ret

