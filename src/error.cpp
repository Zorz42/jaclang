// the error module, which reports syntax errors

#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

void error::syntaxError(const std::string &error) { // Print out syntax error
    std::cerr << "\033[1;31mSyntax error, line " << parser::currToken->line << ": " // syntax error in red
              << error << std::endl << std::endl; // actual error
    std::string errorLine = file::getLine(parser::currToken->line);
    while (errorLine.at(0) == ' ' || errorLine.at(0) == '	')
        errorLine.erase(errorLine.begin());
    std::cerr << errorLine << std::endl; // find line of error
    for (unsigned long i = 0; i < parser::currToken->pos; i++) // point to error token
        std::cerr << " ";
    std::cerr << "^" << "\033[0m" << std::endl; // reset to white

    terminate("SYNTAX ERROR", et_syntax_err);
}

void error::treeError(const std::string &error) { // print out syntax error
    std::cerr << "\033[1;31mTree error: " // tree error in red
              << error << std::endl // actual error
              << "\033[0m"; // reset to white
    terminate("TREE ERROR", et_syntax_err);
}


void error::terminate(const std::string &reason, errorType errorType_) { // exit if error
    std::cerr << "Jaclang terminated with exit code " << errorType_ << "." << std::endl // Terminated
              << "Reason: " << reason << "." << std::endl; // reason
    exit(errorType_); // exit
}
