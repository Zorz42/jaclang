// the error module, which reports syntax errors

#include "jaclang.h"

void error::syntaxError(std::string error) // Print out syntax error
{
	coutn << "\033[1;31mSyntax error, line " << lexer::toks.at(parser::tokCount).line << ": "; // Syntax error in red
	coutn << error << std::endl << std::endl; // Actual error
	coutn << file::getLine(lexer::toks.at(parser::tokCount).line) << std::endl; // find line of error
	for(int i = 0; i < lexer::toks.at(parser::tokCount).pos; i++) // point to error token
		coutd << " ";
	coutn << "^" << std::endl;
	coutn << "\033[0m"; // Reset to whte
	
	terminate("SYNTAX ERROR", ERROR_SYNTAX_ERROR);
}

void error::treeError(std::string error) // Print out syntax error
{
	coutn << "\033[1;31mTree error: "; // Tree error in red
	coutn << error << std::endl; // Actual error
	coutn << "\033[0m"; // Reset to white
	terminate("TREE ERROR", ERROR_SYNTAX_ERROR);
}


void error::terminate(std::string reason, int exitCode) // Exit if error or something
{
	coutn << "Jaclang terminated with exit code " << exitCode << "." << std::endl; // Terminated
	coutn << "Reason: " << reason << "." << std::endl; // Reason
	exit(exitCode); // Exit
}
