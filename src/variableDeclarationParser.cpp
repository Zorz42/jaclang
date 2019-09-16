// this parses variable declaration

#define LIB_ERROR
#define LIB_LEXER
#define LIB_PARSER
#define LIB_GENERATOR

#include "jaclang.h"

#define current lexer::toks.at(parser::tokCount)

int stackPointer = 4;
std::vector<variable> stack;

bool parser::e::variableDeclaration()
{
	if(current.text == "int") // if first text is int - keyword
	{
		branch currentBranch;
		currentBranch.name = "variableDeclaration"; // set to variableDeclaration
		parser::tokCount++;
		if(current.type != TYPE_INDENT) // check if everything is working out and append it to main branch
			error::syntaxError("Expected indent after int keyword in variable declaration");
		appendBranch(current.text, currentBranch);
		parser::tokCount++;
		if(current.text != "=")
			error::syntaxError("Expected '=' after keyword in variable declaration");
		parser::tokCount++;
		branch equationBranch = parser::equation(";", "\\n");
		appendBranch(equationBranch, currentBranch);
		
		appendBranch(currentBranch, *currentBranchScope);
		return true;
	}
	else
		return false;
}