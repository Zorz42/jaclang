// the main loop of parser

#include "jaclang.h"

unsigned int parser::tokCount = 0;
std::vector<branch*> parser::scopes = {&mainBranch};

branch* currentBranchScope;
branch mainBranch;

#define current lexer::toks.at(parser::tokCount)

void parser::main(std::vector<std::string> args)
{
	currentBranchScope = &mainBranch;
	mainBranch.name = args.at(0); // root name is input file name
	
	for(;parser::tokCount < lexer::toks.size(); parser::tokCount++) // go through all tokens
	{
		if(parser::e::systemFunctionCall()); // else execute systemFunctionCall
		else if(parser::e::functionDeclaration());
		else if(parser::e::variableDeclaration()); // else execute variableDeclaration
		else if(parser::e::beginScope());
		else if(parser::e::endScope());
		else
		{
			branch calculation = parser::calculation(); // else parse calculation
			appendBranch(calculation, *currentBranchScope);
		}
	}
}

void appendBranch(branch& source, branch& target) // function for appending branch to another branch
{
	target.sub.push_back(source);
}

void appendBranch(std::string source, branch& target) // function for appending empty branch with name to branch
{
	branch obj;
	obj.name = source;
	target.sub.push_back(obj);
}
