global start
section .data

returnvalue: dq 0
stdoutchar: db 0
section .bss

section .text
                
start:

   mov rax, 0x2000001
   mov rdi, 0
   syscall

printchar:
   mov BYTE [rel stdoutchar], ah
   mov rax, 0x2000004
   mov rdi, 1
   mov rsi, stdoutchar
   mov rdx, 1
   syscall

   ret

%macro pusha 0
   push rbx
   push rcx
   push rsi
   push rdi
   push r8
   push r9
   push r10
   push r11
   push r12
   push r13
   push r14
   push r15
   push rbp
%endmacro

%macro popa 0
   pop rbp
   pop r15
   pop r14
   pop r13
   pop r12
   pop r11
   pop r10
   pop r9
   pop r8
   pop rdi
   pop rsi
   pop rcx
   pop rbx
%endmacro
