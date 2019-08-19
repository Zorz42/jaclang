#include "jaclang.h"

#define current lexer::toks.at(parser::tokCount)

bool parser::e::systemFunctionCall()
{
	if(current.type == TYPE_INDENT && lexer::toks.at(parser::tokCount + 1).type == TYPE_STRING) // if its systemFunction -> indent followed by string
	{
		branch currentBranch; // make branch for function
		if(isSystemIndent(current.text))  // check if its system fucntion call
			currentBranch.name = "systemFunctionCall";
		else
			return false;
		appendBranch(current.text, currentBranch); // append indent to branch 
		parser::tokCount++; // go to argument
		if(current.type != TYPE_STRING)
			error::syntaxError("expected string argment on system function");
		else
			appendBranch(current.text, currentBranch);
		parser::tokCount++;
		if(current.text != "\\n" && current.text != ";") // function end
			error::syntaxError("Expected newline or ';' at the end of function");
		appendBranch(currentBranch, mainBranch); // append branch to root
		return true;
	}
	else
		return false;
}
