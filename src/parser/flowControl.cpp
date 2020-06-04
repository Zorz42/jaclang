#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

#define current parser::currToken

bool parser::e::ifStatement() {
    if(current->text == "if") {
        branch currentBranch;
        currentBranch.alloc();
        currentBranch.name = "ifStatement";
        parser::nextToken();
        branch equationBranch = parser::calculation(false);
        appendBranch(equationBranch, currentBranch);
        appendBranch(currentBranch, *currentBranchScope);
        return true;
    }
    return false;
}

bool parser::e::whileStatement() {
    if(current->text == "while") {
        branch currentBranch;
        currentBranch.alloc();
        currentBranch.name = "whileStatement";
        parser::nextToken();
        branch equationBranch = parser::calculation(false);
        appendBranch(equationBranch, currentBranch);
        appendBranch(currentBranch, *currentBranchScope);
        return true;
    }
    return false;
}
