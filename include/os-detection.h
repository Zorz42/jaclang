#define OS_TYPE unknown
// this is the file that recognizes compilers.

// the nasm compiler (macos: /usr/local/bin/nasm, linux: /usr/bin/nasm)
#define OS_NASM

#ifdef _WIN32
	#ifdef _WIN64
		#error "Jaclang does not support Windows!"
	#else
		#error "Jaclang does not support 32-bit systems!"
	#endif
#elif __APPLE__
	#include "TargetConditionals.h"
	#if TARGET_IPHONE_SIMULATOR
		#error "Jaclang does not support ios simulator!"
	#elif TARGET_OS_IPHONE
		#error "Jaclang does not support iPhones!"
	#elif TARGET_OS_MAC
		#define OS_TYPE OSX
	#else
		#error "Unknown Apple platform"
	#endif
#elif __linux__
    #define OS_TYPE Linux
#elif __unix__ 
    #error "Jaclang does not support this unix OS"
#elif defined(_POSIX_VERSION)
    #error "Jaclang does not support POSIX"
#else
	#error "Jaclang does not recognize this compiler!"
#endif

#if OS_TYPE == Linux
	#define OS_NASM "/usr/bin/nasm"
#elif OS_TYPE == OSX
	#define OS_NASM "/usr/local/bin/nasm"
#endif