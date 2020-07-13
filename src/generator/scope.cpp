#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

void generator::e::scope() { // scope is very straight forward
    Branch *prev_scope = current_branch_scope; // save current scope
    current_branch_scope = &current_branch_scope->sub.at(current_branch_scope_count); // move to new scope
    unsigned long stack_length = asm_::stack.size();  // save stack length
    unsigned long prev_scope_on_stack = asm_::current_scope_on_stack; // save scope on stack
    unsigned long prev_branch_scope_count = current_branch_scope_count; // save branch scope count
    asm_::current_scope_on_stack = stack_length;  // set scope on stack
    current_branch_scope_count = 0;
    
    generator::main();
    
    asm_::current_scope_on_stack = prev_scope_on_stack; // retrieve scope on stack
    current_branch_scope_count = prev_branch_scope_count; // retrieve branch scope count
    while(asm_::stack.size() > stack_length) // remove elements from stack that were in scope
    {
        asm_::decStackPointer(asm_::stack.at(asm_::stack.size() - 1).size());
        asm_::stack.pop_back();
    }
    current_branch_scope = prev_scope; // retrieve current branch scope
}
