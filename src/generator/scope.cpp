#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

void generator::e::scope() {
    branch *prevScope = currentBranchScope; // save current scope
    currentBranchScope = &currentBranchScope->sub->at(currentBranchScope->count); // move to new scope
    unsigned long stackLength = generator::stack.size();  // save stack length
    unsigned long prevScopeOnStack = currentScopeOnStack; // save scope on stack
    currentScopeOnStack = stackLength;  // set scope on stack
    generator::main();
    currentScopeOnStack = prevScopeOnStack; // retrieve scope on stack
    while (generator::stack.size() > stackLength) // remove elements from stack that were in scope
    {
        decStackPointer(generator::stack.at(generator::stack.size() - 1).size());
        generator::stack.pop_back();
    }
    currentBranchScope = prevScope; // retrieve current branch scope
}
