// the error module, which reports syntax errors

#include "jaclang.h"

void error::syntaxError(std::string error) // Print out syntax error
{
	std::cerr << "\033[1;31mSyntax error, line " << lexer::toks.at(parser::tokCount).line << ": " // Syntax error in red
	<< error << std::endl << std::endl; // Actual error
	std::string errorLine = file::getLine(lexer::toks.at(parser::tokCount).line);
	while(errorLine.at(0) == ' ' || errorLine.at(0) == '	')
		errorLine.erase(errorLine.begin());
	std::cerr << errorLine << std::endl; // find line of error
	for(int i = 0; i < lexer::toks.at(parser::tokCount).pos; i++) // point to error token
		std::cerr << " ";
	std::cerr << "^" << "\033[0m" << std::endl; // Reset to white
	
	terminate("SYNTAX ERROR", ERROR_SYNTAX_ERROR);
}

void error::treeError(std::string error) // Print out syntax error
{
	std::cerr << "\033[1;31mTree error: " // Tree error in red
	<< error << std::endl // Actual error
	<< "\033[0m"; // Reset to white
	terminate("TREE ERROR", ERROR_SYNTAX_ERROR);
}


void error::terminate(std::string reason, int exitCode) // Exit if error or something
{
	std::cerr << "Jaclang terminated with exit code " << exitCode << "." << std::endl // Terminated
	<< "Reason: " << reason << "." << std::endl; // Reason
	exit(exitCode); // Exit
}
