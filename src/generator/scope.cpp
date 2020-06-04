#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

void generator::e::scope() {
    branch *prevScope = currentBranchScope; // save current scope
    currentBranchScope = &currentBranchScope->sub->at(currentBranchScope->count); // move to new scope
    unsigned long stackLength = asm_::stack.size();  // save stack length
    unsigned long prevScopeOnStack = asm_::currentScopeOnStack; // save scope on stack
    asm_::currentScopeOnStack = stackLength;  // set scope on stack
    generator::main();
    asm_::currentScopeOnStack = prevScopeOnStack; // retrieve scope on stack
    while (asm_::stack.size() > stackLength) // remove elements from stack that were in scope
    {
        asm_::decStackPointer(asm_::stack.at(asm_::stack.size() - 1).size());
        asm_::stack.pop_back();
    }
    currentBranchScope = prevScope; // retrieve current branch scope
}
