// this generates variable declarations

#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

#define CURRENT current_branch_scope->sub.at(current_branch_scope_count)

void generator::e::variableDeclaration(unsigned long scope_on_stack) {
    Variable obj; // obj variable
    obj.type = CURRENT.sub.at(0).name; // datatype
    obj.name = CURRENT.sub.at(1).name; // name
    obj.arg = false;
    
    unsigned int i = 0;

    if(!obj.size())
        error::semanticError("Cannot declare a variable with size 0!");
    for(const Variable &iter : asm_::stack) { // go through stack
        if(iter.name == CURRENT.sub.at(1).name && i >= scope_on_stack) // if this variable already exists, then report error
            error::semanticError(iter.name + " already exists as a variable!");
        i++;
    }
    
    asm_::pushToStack(obj); // push to stack
    checkForImplicitConversion(obj.type, generator::e::expr(CURRENT.sub.at(2)));  // do calculation
    asm_::append_instruction("mov", asm_::availableRegister(obj.size()), asm_::onStack(asm_::stack_pointer), obj.size()); // set variable on stack
}

void generator::e::variableSetting() {
    Variable *current_variable = getVariable(CURRENT.sub.at(0).name);
    checkForImplicitConversion(current_variable->type, generator::e::expr(CURRENT.sub.at(1))); // do calculation
    asm_::append_instruction("mov", asm_::availableRegister(current_variable->size()), current_variable->generateAddress(), current_variable->size()); // set variable on stack
}

Variable *generator::getVariable(const std::string &name) {
    for(Variable &iter : asm_::stack) // go through stack and check if variable exists
        if(iter.name == name)
            return &iter;
    if(current_function) // go through function args
        for(Variable& iter : current_function->args)
            if(iter.name == name)
                return &iter;
    error::semanticError(name + " does not exist!");
    return nullptr;
}

void generator::checkForImplicitConversion(const std::string &dest, const std::string &source) {
    bool success = dest == source;
    if(!success)
        for(const std::string &conversation : generator::implicit_conversations[source])
            if(dest == conversation)
                success = true;
    if(!success)
        error::semanticError("Could not convert '" + source + "' to '" + dest + "'!");
}
