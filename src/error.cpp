// the error module, which reports syntax errors

#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

#define WHITE "\033[0m"
#define RED "\033[1;31m"

void error::syntaxError(const std::string &error) { // Print out syntax error
    if(debug_show_ast)
        printAST(parser::main_branch);
    
    std::cerr << RED << "Syntax error, line " << parser::curr_token->line << ": " << error << "\n\n"; // actual error
    std::string errorLine = file::getLine(parser::curr_token->line);
    while(errorLine.at(0) == ' ' || errorLine.at(0) == '\t')
        errorLine.erase(errorLine.begin());
    std::cerr << errorLine << std::endl;
    
    for(unsigned long _ = 0; _ < parser::curr_token->pos; _++) // point to error token
        std::cerr << " ";
    std::cerr << "^" << WHITE << std::endl; // reset to white

    terminate("SYNTAX ERROR", Err_Syntax_Error);
}

void error::semanticError(const std::string &error) { // print out syntax error
    std::cerr << RED << "Semantic error: " << error << WHITE << std::endl;
    terminate("SEMANTIC ERROR", Err_Semantic_Error);
}


void error::terminate(const std::string &reason, ErrorType error_type) { // exit if error
    std::cerr << "Jaclang terminated with exit code " << error_type << "." << std::endl // Terminated
              << "Reason: " << reason << "." << std::endl; // reason
    exit(error_type); // exit
}
