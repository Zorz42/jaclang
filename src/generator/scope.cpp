#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

void generator::e::scope() { // scope is very straight forward
    Branch *prev_scope = parser::current_branch_scope; // save current scope
    parser::current_branch_scope = &parser::current_branch_scope->sub->at(parser::current_branch_scope->count); // move to new scope
    unsigned long stackLength = asm_::stack.size();  // save stack length
    unsigned long prevScopeOnStack = asm_::current_scope_on_stack; // save scope on stack
    asm_::current_scope_on_stack = stackLength;  // set scope on stack
    generator::main();
    asm_::current_scope_on_stack = prevScopeOnStack; // retrieve scope on stack
    while(asm_::stack.size() > stackLength) // remove elements from stack that were in scope
    {
        asm_::decStackPointer(asm_::stack.at(asm_::stack.size() - 1).size());
        asm_::stack.pop_back();
    }
    parser::current_branch_scope = prev_scope; // retrieve current branch scope
}
