// this file parses function calls, calculation calls that file too

#include "jaclang.h"

#define current lexer::tokens.at(parser::tokCount)

bool parser::e::functionCall(branch& target)
{
    if(parser::tokCount == lexer::tokens.size())
        return false;
    if(current.type == TYPE_INDENT && lexer::tokens.at(parser::tokCount + 1).text == "(") // if its function -> indent followed by '('
	{
		branch currentBranch; // make branch for function
		if(isSystemIndent(current.text))  // check if its system function call or just function call
			return false;
		else
			currentBranch.name = "functionCall";
		appendBranch(current.text, currentBranch); // append indent to branch 
		parser::tokCount += 2;
		if(current.text != ")")
			error::syntaxError("Function call cannot have arguments (for now)");
		appendBranch(currentBranch, target); // append branch to root
		return true;
	}
    else
		return false;
}

bool parser::e::systemFunctionCall()
{
    if(parser::tokCount == lexer::tokens.size())
        return false;
    if(current.type == TYPE_INDENT && lexer::tokens.at(parser::tokCount + 1).type == TYPE_STRING) // if its systemFunction -> indent followed by string
	{
		branch currentBranch; // make branch for function
		if(isSystemIndent(current.text))  // check if its system function call
			currentBranch.name = "systemFunctionCall";
		else
			return false;
		appendBranch(current.text, currentBranch); // append indent to branch 
		parser::tokCount++; // go to argument
		if(current.type != TYPE_STRING)
			error::syntaxError("Expected one string argument on system function");
		else
			appendBranch(current.text, currentBranch);
		appendBranch(currentBranch, *currentBranchScope); // append branch to root
		return true;
	}
    else
		return false;
}

bool parser::e::functionDeclaration()
{
    if(parser::tokCount == lexer::tokens.size())
        return false;
    if(current.text == "int" && lexer::tokens.at(parser::tokCount + 1).type == TYPE_INDENT && lexer::tokens.at(parser::tokCount + 2).text == "(") // if first text is int
	{
		branch currentBranch;
		currentBranch.name = "functionDeclaration"; // set to variableDeclaration
		parser::tokCount += 3;
		if(current.text != ")") // check if everything is working out and append it to main branch
			error::syntaxError("No arguments allowed in function declaration (for now)");
		
		appendBranch("int", currentBranch);
		appendBranch(lexer::tokens.at(parser::tokCount - 2).text, currentBranch);
		
		appendBranch(currentBranch, *currentBranchScope);
		return true;
	}
    else
		return false;
}

bool parser::e::returnStatement()
{
    if(current.text == "return")
    {
        branch currentBranch;
        currentBranch.name = "returnStatement";
        appendBranch(currentBranch, *currentBranchScope);
        return true;
    }
    else
        return false;
}
