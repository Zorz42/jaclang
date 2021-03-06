// this generates ast into asm code

#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

#define CURRENT current_branch_scope->sub.at(current_branch_scope_count) // current branch

#define _CASE_1(_CASE) else if(CURRENT.name == #_CASE) e::_CASE();
#define _CASE_2(_CASE, _COND) else if(CURRENT.name == #_CASE && (_COND)) e::_CASE();
#define _CASE_3(_CASE, _COND, _ARGS) else if(CURRENT.name == #_CASE && (_COND)) e::_CASE(_ARGS);

#define _GET_4TH_ARG(_ARG1, _ARG2, _ARG3, _ARG4, ...) _ARG4
#define _CASE_MACRO_CHOOSER(...) _GET_4TH_ARG(__VA_ARGS__, _CASE_3, _CASE_2, _CASE_1)

#define CASE(...) _CASE_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

void generator::initialMain() {
    std::string main_func_name = "s" + file::input_file;
    for(unsigned int i = 0; i < main_func_name.size(); i++) // replace all '/' with '.'
        if(main_func_name.at(i) == '/') {
            main_func_name.at(i) = '.';
        } else if((main_func_name.at(i) < 'A' || main_func_name.at(i) > 'Z') &&
                  (main_func_name.at(i) < 'a' || main_func_name.at(i) > 'z'))
            main_func_name.erase(i, 1);
    asm_::append_instruction(".globl", main_func_name);
    asm_::append_instruction(main_func_name + ":");
    
    generator::main(true); // generate assembly tokens out of syntax tree

    asm_::append_instruction("ret");
}

void generator::main(bool in_function) {
    if(in_function) {
        asm_::append_instruction("mov", "%rsp", "%rbp");
        asm_::append_instruction("sub", "$", "%rsp");
    }
    unsigned long sub_rsp = 0;
    if(in_function)
        sub_rsp = asm_::instructions.size() - 1;
    asm_::append_instruction("");
    
    unsigned long prev_branch_scope_count = current_branch_scope_count;
    for(current_branch_scope_count = 0; current_branch_scope_count < current_branch_scope->sub.size(); current_branch_scope_count++) {
        // iterate though branches
        asm_::append_instruction("");
        
        if(CURRENT.name == "systemFunctionCall")  // choose appropriate generator for branch
            e::systemFunctionCall();
        CASE(globalVariableDeclaration)
        CASE(localVariableDeclaration)
        CASE(variableDeclaration)
        CASE(expr, true, CURRENT)
        CASE(scope)
        CASE(functionDeclaration, !current_function)
        CASE(variableSetting)
        CASE(returnStatement, current_function)
        CASE(ifStatement)
        CASE(whileStatement)
        else
            error::semanticError("Unknown branch: " + CURRENT.name);
    }
    current_branch_scope_count = prev_branch_scope_count;
    if(in_function) {
        if(asm_::biggest_stack_pointer % 16)
            asm_::biggest_stack_pointer += 16 - asm_::biggest_stack_pointer % 16; // round to ceil of base 16
        asm_::instructions.at(sub_rsp).arg1 += std::to_string(asm_::biggest_stack_pointer);
        asm_::append_instruction("add", "$" + std::to_string(asm_::biggest_stack_pointer), "%rsp");
    }
}

int8_t getTypeSize(const std::string &type) {
    return int8_t(generator::primitive_datatype_sizes[type]);
}

int8_t Variable::size() const {
    return getTypeSize(type);
}

std::string Variable::generateAddress() const {
    return position >= 0 && position <= 2 ? (position == 2 ? "l" : "g") + name + "(%rip)" : asm_::onStack(position);
}

int Function::size() const {
    return getTypeSize(type);
}

std::string Function::generateName() const {
    std::string result = "f" + name;
    for(const Variable& iter : args)
        result += "." + iter.type;
    return result;
}

bool generator::checkForImplicitConversion(const std::string &dest, const std::string &source) {
    if(dest == source)
        return true;
    for(const std::string &conversation : generator::implicit_conversations[source])
        if(dest == conversation)
            return true;
    return false;
}
