// main header, which includes everything all files need

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <iterator>

struct branch;

// headers/
	#include "../include/constants.h"
	#include "../include/shortcuts.h"
	#include "../include/error.h"
	
	#include "../include/file.h"      // opens file
	#include "../include/lexer.h"     // breaks it into tokens
	#include "../include/parser.h"    // parses it into syntax tree
	#include "../include/generator.h" // generates asm code