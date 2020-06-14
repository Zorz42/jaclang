// this file parses function calls, calculation calls that file too

#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

#define CURRENT parser::curr_token
#define ARGS current_branch.sub->at(1)

bool parser::e::functionCall(Branch &target) {
    if(parser::curr_token == --lexer::tokens.end())
        return false;
    if(CURRENT->type == Indent && parser::peekNextToken()->text == "(" && !isSystemIndent(CURRENT->text)) { // if its function -> name followed by '(' and it's not system function call
        Branch current_branch; // make branch for function
        current_branch.alloc();
        current_branch.name = "functionCall";
        appendBranch(CURRENT->text, current_branch); // append name to branch
        appendBranch("args", current_branch); // append arguments to branch
        ARGS.alloc();
        parser::nextToken();
        parser::nextToken();
        while(CURRENT->text != ")") {
            appendBranch(parser::expr(), ARGS); // append expressions on branch
            if(CURRENT->text == ")")
                break;
            parser::nextToken();
            parser::nextToken();
        }
        appendBranch(current_branch, target); // append branch to root
        return true;
    } else
        return false;
}

bool parser::e::systemFunctionCall() {
    if(parser::curr_token == --lexer::tokens.end())
        return false;
    parser::peekNextToken();
    if(CURRENT->type == Indent &&
        parser::peekNextToken()->type == String) { // if its systemFunction -> name followed by string
        Branch current_branch; // make branch for function
        current_branch.alloc();
        if(isSystemIndent(CURRENT->text))  // check if its system function call
            current_branch.name = "systemFunctionCall";
        else
            return false;
        appendBranch(CURRENT->text, current_branch); // append name to branch
        parser::nextToken(); // go to argument
        if(CURRENT->type != String)
            error::syntaxError("Expected one string argument on system function");
        else
            appendBranch(CURRENT->text, current_branch);
        appendBranch(current_branch, *current_branch_scope); // append branch to root
        return true;
    } else
        return false;
}
    
#define args current_branch.sub->at(2)

bool parser::e::functionDeclaration() {
    if(parser::curr_token == --lexer::tokens.end())
        return false;
    if(contains(generator::primitive_datatypes, CURRENT->text)) {
        Branch current_branch;
        current_branch.alloc();
        current_branch.name = "functionDeclaration"; // set to functionDeclaration
        appendBranch(CURRENT->text, current_branch);
        parser::nextToken();
        appendBranch(parser::curr_token->text, current_branch);
        parser::nextToken();
        const std::string &parenthesis = parser::curr_token->text;
        parser::prevToken();
        if(parser::curr_token->type == Indent && parenthesis == "(") {
            if(current_branch_scope != &parser::main_branch)
                error::syntaxError("Function declaration not allowed outside of main scope!");
            parser::nextToken();
            parser::nextToken();
            appendBranch("args", current_branch);
            args.alloc();
            while(CURRENT->text != ")") { // scan parameters until ')'
                if(!contains(generator::primitive_datatypes, CURRENT->text))
                    error::syntaxError("Parameter value type must be valid!");
                appendBranch(CURRENT->text, args);
                parser::nextToken();
                if(CURRENT->type != Indent)
                    error::syntaxError("Parameter name must be indent!");
                appendBranch(CURRENT->text, args);
                parser::nextToken();
                
                if(CURRENT->text == ")")
                    break;
                else if(CURRENT->text != ",")
                    error::syntaxError("Expected ',' after argument definition!");
                parser::nextToken();
            }
            appendBranch(current_branch, *current_branch_scope);
            return true;
        }
        parser::prevToken();
    }
    return false;
}

bool parser::e::returnStatement() { // a simple return statement
    if(CURRENT->text == "return") {
        Branch current_branch;
        current_branch.alloc();
        current_branch.name = "returnStatement";
        nextToken();
        appendBranch(expr(), current_branch);
        appendBranch(current_branch, *current_branch_scope);
        return true;
    } else
        return false;
}
