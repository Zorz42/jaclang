// this parses variable declaration

#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

#define CURRENT parser::curr_token

bool parser::e::variableDeclaration() {
    if(parser::curr_token == --lexer::tokens.end())
        return false;
    if(contains(generator::primitive_datatypes, CURRENT->text)) { // if first text is a primitive datatype
        Branch current_branch;
        current_branch.alloc();
        current_branch.name = "variableDeclaration"; // set to variableDeclaration
        appendBranch(CURRENT->text, current_branch);
        parser::nextToken();
        if(CURRENT->type != Indent) // check if everything is working out and append it to main branch
            error::syntaxError("Expected name after value type name in variable declaration");
        appendBranch(CURRENT->text, current_branch);
        parser::nextToken();
        if(CURRENT->text != "=")
            error::syntaxError("Expected '=' after value type name in variable declaration");
        parser::nextToken();
        appendBranch(parser::expr(), current_branch);
        appendBranch(current_branch, *current_branch_scope);
        return true;
    } else
        return false;
}

bool parser::e::variableSetting() {
    if(CURRENT == --lexer::tokens.end())
        return false;
    if(CURRENT->type == Indent && parser::peekNextToken()->text == "=") {
        Branch current_branch;
        current_branch.alloc();
        current_branch.name = "variableSetting";
        appendBranch(CURRENT->text, current_branch);
        parser::nextToken();
        parser::nextToken();
        appendBranch(parser::expr(), current_branch); // parse equation from variable setting
        appendBranch(current_branch, *current_branch_scope);
        return true;
    } else
        return false;
}
