#include "jaclang.h"

void init() // initialize global variables
{
    jaclangInput = "";
    jaclangToNasm = "";
    nasmToLinker = "";
    binaryOutput = "";

    cacheDir = ".jlcache";

    generator::availableRegisters32 = {
            "ebx",
            "ecx",
            "esi",
            "edi",
            "r7d",
            "r8d",
            "r9d",
            "r10d",
            "r11d",
            "r12d",
            "r13d",
            "r14d",
            "r15d"
    };

#if OS_TYPE == 0 // Linux
    file::outputVector = {
            "section .data", // data section
            "",
            "stdoutchar: db 0",
            "section .bss",  // bss section
            "",
            "section .text", // text section
            "   global _start", // for linker
            "_start:",
            "",
            "",
            "   mov eax, 60",  // sys exit
            "   mov edi, 0", // exit code 0 (success)
            "   syscall",
            "",
            "printchar:", // print character
            "   mov BYTE [stdoutchar], ah", // char argument
            "   mov rax, 1",
            "   mov rdi, 1",
            "   mov rsi, stdoutchar",
            "   mov rdx, 1",
            "   syscall",
            "",
            "   ret",
    };
#elif OS_TYPE == 1 // MACOS
    file::outputVector = {
                "global start",
                "section .data", // data section
                "",
		"stdoutchar: db 0",
                "section .bss",  // bss section
                "",
                "section .text", // text section
                "", // for linker
                "start:",
                "",
                "",
                "   mov     rax, 0x2000001",
                "   mov     rdi, 0",
                "   syscall",
                "",
                "printchar:", // print character
                "   mov BYTE [rel stdoutchar], ah", // char argument
                "   mov rax, 0x2000004",
                "   mov rdi, 1",
                "   mov rsi, stdoutchar",
                "   mov rdx, 1",
                "   syscall",
                "",
                "   ret",
        };
#endif


    lexer::keywords = {
            "int",
            "return"
    };

    parser::scopes = {&mainBranch};

#define find(x) find(file::outputVector, x)

    file::asm_data = find("section .data")     + 1; // locate each section
    file::asm_bss  = find("section .bss")      + 1;
    file::asm_text = find("section .text")     + 4;
    file::asm_func = file::outputVector.size();

#undef find
}

