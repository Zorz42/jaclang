// this parses variable declaration

#include "jaclang.h"

#define current lexer::tokens.at(parser::tokCount)

bool parser::e::variableDeclaration()
{
	if(current.text == "int") // if first text is int
	{
		branch currentBranch;
		currentBranch.name = "variableDeclaration"; // set to variableDeclaration
		parser::tokCount++;
		if(current.type != TYPE_INDENT) // check if everything is working out and append it to main branch
			error::syntaxError("Expected indent after int value type indent in variable declaration");
		appendBranch(current.text, currentBranch);
		parser::tokCount++;
		if(current.text != "=")
			error::syntaxError("Expected '=' after value type indent in variable declaration");
		parser::tokCount++;
		branch equationBranch = parser::calculation(true);
		appendBranch(equationBranch, currentBranch);
		
		appendBranch(currentBranch, *currentBranchScope);
		return true;
	}
	else
		return false;
}

bool parser::e::variableSetting()
{
    if(parser::tokCount == lexer::tokens.size())
        return false;
    if(current.type == TYPE_INDENT && lexer::tokens.at(parser::tokCount + 1).text == "=")
    {
        branch currentBranch;
        currentBranch.name = "variableSetting";
        appendBranch(current.text, currentBranch);
        parser::tokCount += 2;
        branch equationBranch = parser::calculation(true);
        appendBranch(equationBranch, currentBranch);
        appendBranch(currentBranch, *currentBranchScope);
        return true;
    }
    else
        return false;
}
