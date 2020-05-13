.section __BSS, __bss
.section __DATA, __data

stdoutchar:
   .asciz "0"

.section __TEXT, __text

.globl _main
_main:
 
   mov $0, %ebx
   mov $0x2000001, %eax           # exit 0
   syscall

printchar: 
   mov stdoutchar@GOTPCREL(%rip), %rsi
   mov %al, (%rsi)
   mov $0x2000004, %eax                  
   mov $1, %edi                          
   mov stdoutchar@GOTPCREL(%rip), %rsi   
   mov $100, %rdx                        
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
