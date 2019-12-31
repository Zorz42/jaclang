// main header, which includes everything all files need

#pragma once

#define VERSION_STR "BETA 2.3.2"
#define VERSION_INT 10

#include <iostream>
#include <vector>
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
#include "main.h"         // main file where everything happens
