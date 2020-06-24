// main header, which includes everything all files need

#pragma once

#include "version.h"

#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>

struct Branch;

namespace parameters {
    inline bool optimize = true;
}

// headers/
#include "os-detection.h" // detects os during compile-time
#include "shortcuts.h"    // shortcut functions
#include "error.h"        // error module
#include "file.h"         // file module
#include "preprocessor.h" // preprocessor will preprocess file
#include "lexer.h"        // breaks it into tokens
#include "parser.h"       // parses it into tree
#include "optimiser.h"    // optimises main tree
#include "generator.h"    // generates assembly code
#include "assembly.h"     // module for assembly generation and optimization
