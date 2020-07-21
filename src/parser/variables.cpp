// this parses variable declaration

#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

#define CURRENT parser::curr_token

bool parser::e::variableDeclaration() {
    if(CURRENT == --lexer::tokens.end())
        return false;
    if(contains(generator::primitive_datatypes, CURRENT->text)) { // if first text is a primitive datatype
        Branch current_branch;
        current_branch.name = "variableDeclaration"; // set to variableDeclaration
        appendBranch(CURRENT->text, current_branch);
        parser::nextToken();
        if(CURRENT->type != Indent) // check if everything is working out and append it to main branch
            error::syntaxError("Expected name after value type name in variable declaration!");
        appendBranch(CURRENT->text, current_branch);
        parser::nextToken();
        if(CURRENT->text == "=") {
            parser::nextToken();
            appendBranch(parser::expr(), current_branch);
        }
        else {
            parser::prevToken();
            appendBranch("none", current_branch);
        }
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

bool parser::e::globalVariableDeclaration() {
    if(CURRENT->text == "global") {
        parser::nextToken();
        if(!variableDeclaration())
            error::syntaxError("Invalid variable declaration after keyword global!");
        current_branch_scope->sub.at(current_branch_scope->sub.size() - 1).name = "globalVariableDeclaration";
        return true;
    }
    else
        return false;
}

bool parser::e::localVariableDeclaration() {
    if(CURRENT->text == "local") {
        parser::nextToken();
        if(!variableDeclaration())
            error::syntaxError("Invalid variable declaration after keyword local!");
        current_branch_scope->sub.at(current_branch_scope->sub.size() - 1).name = "localVariableDeclaration";
        return true;
    }
    else
        return false;
}
