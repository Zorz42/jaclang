// the main loop of parser

#include <utility>

#include "jaclang.h"

unsigned long parser::tokCount = 0;
std::vector<branch*> parser::scopes;

branch* currentBranchScope;
branch mainBranch;

#define current lexer::tokens.at(parser::tokCount)

void parser::main(std::string rootName)
{
	currentBranchScope = &mainBranch;
	mainBranch.name = std::move(rootName); // root name is input file name
	
	for(;parser::tokCount < lexer::tokens.size(); parser::tokCount++) // go through all tokens
	{
		if(parser::e::systemFunctionCall()); // else execute systemFunctionCall
		else if(parser::e::functionDeclaration());
		else if(parser::e::variableDeclaration()); // else execute variableDeclaration
		else if(parser::e::beginScope());
		else if(parser::e::endScope());
        else if(parser::e::variableSetting());
		else
			appendBranch(parser::calculation(), *currentBranchScope);
	}
}

void appendBranch(const branch& source, branch& target) // function for appending branch to another branch
{
	target.sub.push_back(source);
}

void appendBranch(std::string source, branch& target) // function for appending empty branch with name to branch
{
	branch obj;
	obj.name = std::move(source);
	target.sub.push_back(obj);
}
