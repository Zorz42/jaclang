// the main loop of parser

#include "jaclang.h"

unsigned long parser::tokCount = 0;
std::vector<branch*> parser::scopes;

branch* currentBranchScope;
branch mainBranch;

std::vector<datatypeMatches> generator::operatorMatches;
std::vector<std::string> generator::primitiveDatatypes;

#define current lexer::tokens.at(parser::tokCount)

void parser::main(std::string rootName)
{
    auto parserFunctions = {
            &parser::e::systemFunctionCall,
            &parser::e::functionDeclaration,
            &parser::e::variableDeclaration,
            &parser::e::beginScope,
            &parser::e::endScope,
            &parser::e::variableSetting,
            &parser::e::returnStatement,
    };

    currentBranchScope = &mainBranch;
	mainBranch.name = std::move(rootName); // root name is input file name
	
	for(;parser::tokCount < lexer::tokens.size(); parser::tokCount++) // go through all tokens
	{
        bool knownBranch = false;
		for(auto i : parserFunctions)
            if(i())
            {
                knownBranch = true;
                break;
            }
		if(!knownBranch)
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
