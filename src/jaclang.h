// main header, which includes everything all files need
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <iterator>
#include <stdio.h>

extern std::streambuf* orig_buf;

struct branch;

#define coutd if(debug) std::cout.rdbuf(orig_buf); else std::cout.rdbuf(NULL); std::cout
#define coutn std::cout.rdbuf(orig_buf); std::cout

extern bool debug;

// headers/
	#include "../include/constants.h"
	#include "../include/shortcuts.h"
	#include "../include/error.h"
	
	#include "../include/file.h"         // opens file
	#include "../include/preprocessor.h" // preprocesses file
	#include "../include/lexer.h"        // breaks it into tokens
	#include "../include/parser.h"       // parses it into syntax tree
	#include "../include/generator.h"    // generates asm code