#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

#define CURRENT parser::curr_token

bool parser::e::ifStatement() { // simple if statement
    if(CURRENT->text == "if") {
        Branch current_branch;
        current_branch.name = "ifStatement";
        parser::nextToken();
        appendBranch(parser::expr(), current_branch);
        appendBranch(current_branch, *current_branch_scope);
        return true;
    }
    return false;
}

bool parser::e::whileStatement() { // very similar while statement
    if(CURRENT->text == "while") {
        Branch current_branch;
        current_branch.name = "whileStatement";
        parser::nextToken();
        appendBranch(parser::expr(), current_branch);
        appendBranch(current_branch, *current_branch_scope);
        return true;
    }
    return false;
}
