// main header, which includes everything all files need

#include "version.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <list>
#include <unordered_map>
#include <fstream>

struct Branch;

// headers/
#include "os-detection.h" // detects os during compile-time
#include "shortcuts.h"    // shortcut functions
#include "error.h"        // error module
#include "file.h"         // file module
#include "preprocessor.h" // preprocessor will preprocess file
#include "lexer.h"        // breaks it into tokens
#include "parser.h"       // parses it into tree
#include "optimizer.h"    // optimizes main tree
#include "generator.h"    // generates assembly code
#include "assembly.h"     // module for assembly generation and optimization
