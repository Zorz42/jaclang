// the main loop of parser

#include "jaclang.h"

std::list<token>::iterator parser::currToken;
std::vector<branch*> parser::scopes;

branch* currentBranchScope;
branch mainBranch;

unsigned long parser::tokCount;

std::vector<datatypeMatches> generator::operatorMatches;
std::vector<std::string> generator::primitiveDatatypes;

#define current lexer::tokens.at(parser::tokCount)

bool parser::breakLoop = false;

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

	parser::currToken = lexer::tokens.begin();

	while(true) // go through all tokens
	{
	    std::cout << currToken->text << std::endl;
        bool knownBranch = false;
		for(auto i : parserFunctions)
		{
		    if(breakLoop)
		        break;
		    if(i())
            {
                knownBranch = true;
                break;
            }
        }
		if(breakLoop)
		    break;
		if(!knownBranch)
			appendBranch(parser::calculation(), *currentBranchScope);
        parser::nextToken();
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

std::_List_iterator<token> parser::peekNextToken()
{
    auto result = parser::nextToken();
    parser::prevToken();
    return result;
}

std::list<token>::iterator parser::nextToken()
{
    if(parser::currToken == lexer::tokens.end())
        parser::breakLoop = true;
    parser::tokCount++;
    return ++parser::currToken;
}

std::list<token>::iterator parser::prevToken()
{
    parser::tokCount--;
    return --parser::currToken;
}