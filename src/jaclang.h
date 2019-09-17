// main header, which includes everything all files need

#pragma once

#ifdef LIB_IOSTREAM
#include "../include/iostream.h" // cout
#endif
#ifdef LIB_FSTREAM
#include <fstream>  // open file
#endif
#ifdef LIB_STRING
#include <string>   // better strings
#endif
#ifdef LIB_VECTOR
#include <vector>   // better arrays
#endif
#ifdef LIB_CHRONO
#include <chrono>   // time
#endif

#ifdef LIB_IOSTREAM
extern std::streambuf* orig_buf;
#endif

struct branch;

#ifdef LIB_IOSTREAM
#define coutd if(debug) std::cout.rdbuf(orig_buf); else std::cout.rdbuf(NULL); std::cout
#define coutn std::cout.rdbuf(orig_buf); std::cout
#endif

extern bool debug;

// headers/
#ifdef LIB_CONSTANTS
	#include "../include/constants.h"
#endif
#ifdef LIB_SHORTCUTS
	#include "../include/shortcuts.h"
#endif
#ifdef LIB_ERROR
	#include "../include/error.h"
#endif
	
#ifdef LIB_FILE
	#include "../include/file.h"         // opens file
#endif
#ifdef LIB_PREPROCESSOR
	#include "../include/preprocessor.h" // preprocesses file
#endif
#ifdef LIB_LEXER
	#include "../include/lexer.h"        // breaks it into tokens
#endif
#ifdef LIB_PARSER
	#include "../include/parser.h"       // parses it into syntax tree
#endif
#ifdef LIB_GENERATOR
	#include "../include/generator.h"    // generates asm code
#endif