// this generates variable declarations

#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

#define CURRENT current_branch_scope->sub.at(current_branch_scope_count)

void generator::e::variableDeclaration() {
    Variable obj; // obj variable
    obj.type = CURRENT.sub.at(0).name; // datatype
    obj.name = CURRENT.sub.at(1).name; // name
    
    unsigned int i = 0;

    if(!obj.size())
        error::semanticError("Cannot declare a variable with size 0!");
    for(const Variable &iter : asm_::stack) { // go through stack
        if(iter.name == CURRENT.sub.at(1).name && i >= asm_::current_scope_on_stack) // if this variable already exists, then report error
            error::semanticError(iter.name + " already exists as a variable!");
        i++;
    }
    
    asm_::pushToStack(obj); // push to stack
    if(CURRENT.sub.at(2).name != "none") { // if it's definition
        checkForImplicitConversion(obj.type, generator::e::expr(CURRENT.sub.at(2)));  // do calculation
        asm_::append_instruction("mov", asm_::availableRegister(obj.size()), asm_::onStack(-asm_::stack_pointer), obj.size()); // set variable on stack
    }
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
    for(Variable &iter : asm_::global_variables) // go through global variables
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

void generator::e::globalVariableDeclaration() {
    Variable obj; // obj variable
    obj.type = CURRENT.sub.at(0).name; // datatype
    obj.name = CURRENT.sub.at(1).name; // name
    obj.position = 0;
    
    unsigned int i = 0;

    if(!obj.size())
        error::semanticError("Cannot declare a variable with size 0!");
    for(const Variable &iter : asm_::global_variables) { // go through stack
        if(iter.name == CURRENT.sub.at(1).name && i >= asm_::current_scope_on_stack) // if this variable already exists, then report error
            error::semanticError(iter.name + " already exists as a variable!");
        i++;
    }
    
    asm_::global_variables.emplace_back(obj);
    asm_::append_instruction(".globl", "g" + obj.name); // set variable
    
    if(CURRENT.sub.at(2).name != "none") { // if it's definition
        checkForImplicitConversion(obj.type, generator::e::expr(CURRENT.sub.at(2)));  // do calculation
        asm_::append_instruction("mov", asm_::availableRegister(obj.size()), obj.generateAddress(), obj.size()); // set variable
        asm_::append_instruction("g" + obj.name + ":", "", "", 0, Section_Data);
        asm_::append_instruction("." + asm_::size_keywords[obj.size()], "0", "", 0, Section_Data);
    }
}
