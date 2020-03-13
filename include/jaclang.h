// main header, which includes everything all files need

#pragma once

#define VERSION "BETA 2.6.1"

#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>

struct branch;

#define coutd std::cout

extern bool debug_show_tokens;
extern bool debug_show_ast;

// headers/
#include "os-detection.h" // detects os during compile-time
#include "constants.h"    // constants
#include "shortcuts.h"    // shortcut functions
#include "error.h"        // error module
#include "file.h"         // opens file
#include "preprocessor.h" // preprocessor will preprocess file
#include "lexer.h"        // breaks it into tokens
#include "parser.h"       // parses it into syntax tree
#include "generator.h"    // generates asm code
