// the main loop of parser

#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

bool break_loop = false;

void parser::main(std::string root_name) {
    /*
     This goes through all the code tokens and each function checks if it is a recognised pattern of tokens,
     if it is, it returns true, else false
     */
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

    current_branch_scope = &main_branch;
    main_branch.name = std::move(root_name); // root name is input file name

    curr_token = lexer::tokens.begin();
    if(curr_token == lexer::tokens.end())
        return;
    
    while(true) { // go through all tokens
        bool knownBranch = false;
        for(auto i : parserFunctions) {
            if(break_loop)
                break;
            bool temp = i();
            if(temp) {
                knownBranch = true;
                break;
            }
        }
        if(break_loop)
            break;
        if(!knownBranch)
            appendBranch(parser::expr(), *current_branch_scope);
        nextToken();
    }
}

void parser::appendBranch(const Branch &source, Branch &target) { // function for appending branch to another branch
    target.sub->push_back(source);
}

void parser::appendBranch(std::string source, Branch &target) { // function for appending empty branch with name to branch
    Branch obj;
    obj.name = std::move(source);
    target.sub->push_back(obj);
}

std::list<Token>::iterator parser::peekNextToken() { // see the next token without moving to it
    auto result = nextToken();
    prevToken();
    return result;
}

std::list<Token>::iterator parser::nextToken() { // move to the next token
    if(curr_token == lexer::tokens.end())
        break_loop = true;
    parser::tok_count++;
    return ++parser::curr_token;
}

std::list<Token>::iterator parser::prevToken() { // move to previous token
    tok_count--;
    return --curr_token;
}

Branch::Branch(const Branch &input) {
    if(input.sub != nullptr) {
        alloc();
        *sub = *input.sub;
    }
    name = input.name;
    count = input.count;
}

void Branch::alloc() { // allocate vector of sub branches
    sub = new std::vector<Branch>;
}

Branch::~Branch() {
    delete sub;
    sub = nullptr;
}
