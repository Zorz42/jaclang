// the error module, which reports syntax errors

#include <jaclang.h>

void error::syntaxError(std::string error) // Print out syntax error
{
	std::cout << "\033[1;31mSyntax error, line " << lexer::toks.at(parser::tokCount).line << ": "; // Syntax error in red
	std::cout << error << std::endl << std::endl; // Actual error
	std::cout << file::getLine(lexer::toks.at(parser::tokCount).line) << std::endl; // find line of error
	for(int i = 0; i < lexer::toks.at(parser::tokCount).pos; i++) // point to error token
		std::cout << " ";
	std::cout << "^" << std::endl;
	std::cout << "\033[0m"; // Reset to whte
	
	terminate("SYNTAX ERROR", ERROR_SYNTAX_ERROR);
}

void error::treeError(std::string error) // Print out syntax error
{
	std::cout << "\033[1;31mTree error: "; // Tree error in red
	std::cout << error << std::endl; // Actual error
	std::cout << "\033[0m"; // Reset to white
	terminate("TREE ERROR", ERROR_SYNTAX_ERROR);
}


void error::terminate(std::string reason, int exitCode) // Exit if error or something
{
	std::cout << "Jaclang terminated with exit code " << exitCode << "." << std::endl; // Terminated
	std::cout << "Reason: " << reason << "." << std::endl; // Reason
	exit(exitCode); // Exit
}
