#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

void generator::e::scope() {
    branch *prevScope = currentBranchScope; // save current scope
    currentBranchScope = &currentBranchScope->sub->at(currentBranchScope->count); // move to new scope
    unsigned long stackLength = at::stack.size();  // save stack length
    unsigned long prevScopeOnStack = at::currentScopeOnStack; // save scope on stack
    at::currentScopeOnStack = stackLength;  // set scope on stack
    generator::main();
    at::currentScopeOnStack = prevScopeOnStack; // retrieve scope on stack
    while (at::stack.size() > stackLength) // remove elements from stack that were in scope
    {
        at::decStackPointer(at::stack.at(at::stack.size() - 1).size());
        at::stack.pop_back();
    }
    currentBranchScope = prevScope; // retrieve current branch scope
}
