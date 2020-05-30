// the main loop of parser

#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

#define current lexer::tokens.at(parser::tokCount)

bool breakLoop = false;

void parser::main(std::string rootName) {
    auto parserFunctions = {
        &e::systemFunctionCall,
        &e::ifStatement,
        &e::whileStatement,
        &e::functionDeclaration,
        &e::variableDeclaration,
        &e::beginScope,
        &e::endScope,
        &e::variableSetting,
        &e::returnStatement,
    };

    currentBranchScope = &mainBranch;
    mainBranch.name = std::move(rootName); // root name is input file name

    currToken = lexer::tokens.begin();
    if(currToken == lexer::tokens.end())
        return;
    
    while (true) { // go through all tokens
        bool knownBranch = false;
        for (auto i : parserFunctions) {
            if (breakLoop)
                break;
            if (i()) {
                knownBranch = true;
                break;
            }
        }
        if (breakLoop)
            break;
        if (!knownBranch)
            appendBranch(parser::calculation(), *currentBranchScope);
        nextToken();
    }
}

void appendBranch(const branch &source, branch &target) { // function for appending branch to another branch
    target.sub->push_back(source);
}

void appendBranch(std::string source, branch &target) { // function for appending empty branch with name to branch
    branch obj;
    obj.name = std::move(source);
    target.sub->push_back(obj);
}

std::list<token>::iterator parser::peekNextToken() {
    auto result = nextToken();
    prevToken();
    return result;
}

std::list<token>::iterator parser::nextToken() {
    if (currToken == lexer::tokens.end())
        breakLoop = true;
    parser::tokCount++;
    return ++parser::currToken;
}

std::list<token>::iterator parser::prevToken() {
    tokCount--;
    return --currToken;
}
