// the main loop of parser

#include "jaclang.h"

int parser::tokCount = 0;
std::vector<branch*> parser::scopes = {&mainBranch};

branch* currentBranchScope;
branch mainBranch;

#define current lexer::toks.at(parser::tokCount)

void parser::main()
{
	currentBranchScope = &mainBranch;
	mainBranch.name = file::input; // root name is input file name
	
	for(;parser::tokCount < lexer::toks.size(); parser::tokCount++) // go through all tokens
	{
		if(current.text != ";" && current.text != "\\n") // ignore ';' and '\n'
		{
			if(parser::e::systemFunctionCall()); // else execute systemFunctionCall
			else if(parser::e::functionDeclaration());
			else if(parser::e::variableDeclaration()); // else execute variableDeclaration
			else if(parser::e::beginScope());
			else if(parser::e::endScope());
			else
			{
				branch equation = parser::equation(";", "\\n"); // else parse equation
				appendBranch(equation, *currentBranchScope);
			}
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