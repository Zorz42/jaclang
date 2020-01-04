// this parses variable declaration

#include "jaclang.h"

#define current parser::tokCount

bool parser::e::variableDeclaration()
{
    if(parser::peekNextToken() == lexer::tokens.end())
        return false;
	if(contains(generator::primitiveDatatypes, current->text)) // if first text is a primitive datatype
	{
		branch currentBranch;
		currentBranch.name = "variableDeclaration"; // set to variableDeclaration
        appendBranch(current->text, currentBranch);
		parser::tokCount++;
		if(current->type != TYPE_INDENT) // check if everything is working out and append it to main branch
			error::syntaxError("Expected name after value type name in variable declaration");
		appendBranch(current->text, currentBranch);
		parser::tokCount++;
		if(current->text != "=")
			error::syntaxError("Expected '=' after value type name in variable declaration");
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
    if(parser::peekNextToken() == lexer::tokens.end())
        return false;
    if(current->type == TYPE_INDENT && parser::peekNextToken()->text == "=")
    {
        branch currentBranch;
        currentBranch.name = "variableSetting";
        appendBranch(current->text, currentBranch);
        parser::tokCount++;
        parser::tokCount++;
        branch equationBranch = parser::calculation(true);
        appendBranch(equationBranch, currentBranch);
        appendBranch(currentBranch, *currentBranchScope);
        return true;
    }
    else
        return false;
}
