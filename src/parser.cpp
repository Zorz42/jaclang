// the main loop of parser

#include "jaclang.h"

int parser::tokCount = 0;
std::vector<branch*> scopes = {&mainBranch};

branch* currentBranchScope;
branch mainBranch;

#define current lexer::toks.at(parser::tokCount)

void parser::main()
{
	currentBranchScope = &mainBranch;
	mainBranch.name = file::input; // root name is input file name
	//for(token iterator : tokens)
		//coutd << iterator.type << ": " << iterator.text << std::endl;
	
	for(;parser::tokCount < lexer::toks.size(); parser::tokCount++) // go through all tokens
	{
		if(current.text != ";" && current.text != "\\n") // ignore ';' and '\n'
		{
			if(parser::e::functionCall()) 0; // execute functionCall
			else if(parser::e::systemFunctionCall()) 0; // else execute systemFunctionCall
			else if(parser::e::variableDeclaration()) 0; // else execute variableDeclaration
			else if(parser::e::beginScope()) 0;
			else if(parser::e::endScope()) 0;
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