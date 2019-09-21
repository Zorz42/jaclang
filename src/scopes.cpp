// This will detect scopes

#define LIB_ERROR
#define LIB_LEXER
#define LIB_PARSER

#include "jaclang.h"

#define current lexer::toks.at(parser::tokCount)

bool parser::e::beginScope()
{
	if(current.text == "{")
	{
		branch scope;
		scope.name = "scope";
		appendBranch(scope, *currentBranchScope);
		currentBranchScope = &(currentBranchScope->sub.back());
		scopes.push_back(currentBranchScope);
		return true;
	}
	else
		return false;
}

bool parser::e::endScope()
{
	if(current.text == "}")
	{
		if(scopes.size() < 2)
			error::syntaxError("There is no scope to end");
		scopes.pop_back();
		currentBranchScope = scopes.back();
		return true;
	}
	else
		return false;
}