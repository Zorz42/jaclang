// this file parses function calls, equation calls that file too

#include "jaclang.h"

#define current lexer::toks.at(parser::tokCount)

bool parser::e::functionCall()
{
	if(current.type == TYPE_INDENT && lexer::toks.at(parser::tokCount + 1).text == "(") // if its function -> indent followed by '('
	{
		branch currentBranch; // make branch for function
		if(isSystemIndent(current.text))  // check if its system fucntion call or just function call
			return false;
		else
			currentBranch.name = "functionCall";
		appendBranch(current.text, currentBranch); // append indent to branch 
		parser::tokCount += 2; // go to arguments
		for(; lexer::toks.at(parser::tokCount - 1).text != ")"; parser::tokCount++)
		{
			branch obj = parser::equation(",", ")"); // ends with ',' or ')'
			appendBranch(obj, currentBranch); // append branch
		}
		if(current.text != "\\n" && current.text != ";") // function end
			error::syntaxError("Expected newline or ';' at the end of function");
		appendBranch(currentBranch, *currentBranchScope); // append branch to root
		return true;
	}
	else
		return false;
}