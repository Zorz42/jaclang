// this file parses function calls, calculation calls that file too

#include "jaclang.h"

#define current parser::currToken

bool parser::e::functionCall(branch& target) {
    if(parser::currToken == --lexer::tokens.end())
        return false;
    if(current->type == TYPE_INDENT && parser::peekNextToken()->text == "(") // if its function -> name followed by '('
	{
		branch currentBranch; // make branch for function
		if(isSystemIndent(current->text))  // check if its system function call or just function call
			return false;
		else
			currentBranch.name = "functionCall";
		appendBranch(current->text, currentBranch); // append name to branch
		parser::nextToken();
        parser::nextToken();
		if(current->text != ")")
			error::syntaxError("Function call cannot have arguments (for now)");
		appendBranch(currentBranch, target); // append branch to root
		return true;
	}
    else
        return false;
}

bool parser::e::systemFunctionCall() {
    if(parser::currToken == --lexer::tokens.end())
        return false;
    parser::peekNextToken();
    if(current->type == TYPE_INDENT && parser::peekNextToken()->type == TYPE_STRING) // if its systemFunction -> name followed by string
    {
        branch currentBranch; // make branch for function
        if (isSystemIndent(current->text))  // check if its system function call
            currentBranch.name = "systemFunctionCall";
        else
            return false;
        appendBranch(current->text, currentBranch); // append name to branch
        parser::nextToken(); // go to argument
        if (current->type != TYPE_STRING)
            error::syntaxError("Expected one string argument on system function");
        else
            appendBranch(current->text, currentBranch);
        appendBranch(currentBranch, *currentBranchScope); // append branch to root
        return true;
    }
    else
        return false;
}

bool parser::e::functionDeclaration() {
    if(parser::currToken == --lexer::tokens.end())
        return false;
    parser::nextToken();
    parser::nextToken();
    auto nextNextToken = parser::currToken;
    parser::prevToken();
    parser::prevToken();
    if(current->text == "int" && parser::peekNextToken()->type == TYPE_INDENT && nextNextToken->text == "(") // if first text is int
	{
		branch currentBranch;
		currentBranch.name = "functionDeclaration"; // set to variableDeclaration
		parser::nextToken();
        parser::nextToken();
        parser::nextToken();
		if(current->text != ")") // check if everything is working out and append it to main branch
			error::syntaxError("No arguments allowed in function declaration (for now)");
		
		appendBranch("int", currentBranch);
        parser::prevToken();
        parser::prevToken();
        auto prevPrevToken = parser::currToken;
        parser::nextToken();
        parser::nextToken();
		appendBranch(prevPrevToken->text, currentBranch);
		
		appendBranch(currentBranch, *currentBranchScope);
		return true;
	}
    else
		return false;
}

bool parser::e::returnStatement() {
    if(current->text == "return")
    {
        branch currentBranch;
        currentBranch.name = "returnStatement";
        appendBranch(currentBranch, *currentBranchScope);
        return true;
    }
    else
        return false;
}
