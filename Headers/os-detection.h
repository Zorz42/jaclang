// this is the file that recognizes compilers.

#ifdef _WIN32
    #ifdef _WIN64
        #error "Jaclang does not support Windows (Yet...)!"
    #else
        #error "Jaclang does not support 32-bit systems!"
    #endif

    #elif __APPLE__
        #include "TargetConditionals.h"
    #if TARGET_IPHONE_SIMULATOR
        #error "Jaclang does not support IOS Simulator!"
    #elif TARGET_OS_IPHONE
        #error "Jaclang does not support iPhones!"
    #elif TARGET_OS_MAC
        #define OS_TYPE 1
    #else
        #error "Unknown Apple platform"
    #endif
#elif __linux__
    #define OS_TYPE 0
#elif __unix__
    #error "Jaclang does not support this unix OS"
#elif defined(_POSIX_VERSION)
    #error "Jaclang does not support POSIX"
#else
    #error "Jaclang does not recognize this compiler!"
#endif

// the nasm compiler (macos: /usr/local/bin/nasm, linux: /usr/bin/nasm)

#if OS_TYPE == 0 // Linux
    #define OS_NASM "/usr/bin/nasm -w-all -f elf64 "
    //#define OS_LINKER "ld -m elf_x86_64 -s -o "
    #define OS_SECTION_DATA ".data"
    #define OS_SECTION_BSS ".bss"
    #define OS_SECTION_TEXT ".text"

    #elif OS_TYPE == 1 // MACOS

    #define OS_NASM "/usr/local/bin/nasm -w-all -f macho64 "
    //#define OS_LINKER "ld -macosx_version_min 10.7.0 -no_pie -o "
    #define OS_SECTION_DATA ".section __DATA, __data"
    #define OS_SECTION_BSS ".section __BSS, __bss"
    #define OS_SECTION_TEXT ".section __TEXT, __text"
#endif
