// This will detect scopes

#include "jaclang.h"

#define current lexer::toks.at(parser::tokCount)

bool parser::e::beginScope()
{
	if(current.text == "{")
		return true;
	else
		return false;
}

bool parser::e::endScope()
{
	if(current.text == "}")
		return true;
	else
		return false;
}