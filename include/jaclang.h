// main header, which includes everything all files need

#pragma once

#define VERSION_STR "BETA 1.2.0"
#define VERSION_INT 3

#include <iostream> // cout
#include <fstream>  // open file
#include <string>   // better strings
#include <vector>   // better arrays
#include <chrono>   // time

struct branch;

#define coutd if(debug) std::cout

extern bool debug;

// headers/
#include "constants.h"
#include "shortcuts.h"
#include "error.h"
#include "file.h"         // opens file
#include "preprocessor.h" // preprocesses file
#include "lexer.h"        // breaks it into tokens
#include "parser.h"       // parses it into syntax tree
#include "generator.h"    // generates asm code