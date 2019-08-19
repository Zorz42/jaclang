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
	#include "constants.h"
	#include "shortcuts.h"
	#include "error.h"
	
	#include "file.h"      // opens file
	#include "lexer.h"     // breaks it into tokens
	#include "parser.h"    // parses it into syntax tree
	#include "generator.h" // generates asm code