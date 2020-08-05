// this generates system functions

#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

#define CURRENT generator::current_branch_scope->sub.at(generator::current_branch_scope_count) // function branch
#define CURRENT_NAME  CURRENT.sub.at(0).name // name of function
#define CURRENT_NAME_2 CURRENT.sub.at(1).name

void f_asmtext();
void f_asmfunc();
void f_asmdata();
void f_asmbss();
void f_printchar();
std::string generateReadableFunctionName(const std::string &name, const std::vector<std::string> &args);

void generator::e::systemFunctionCall() // system function: __test__
{
    if(CURRENT_NAME == "__asm__") {
        if(generator::current_function == nullptr)
            f_asmtext();
        else
            f_asmfunc();
    }
    else if(CURRENT_NAME == "__asmdata__")
        f_asmdata();
    else if(CURRENT_NAME == "__asmbss__")
        f_asmbss();
    else if(CURRENT_NAME == "__printchar__")
        f_printchar();
    else
        error::semanticError("Unknown system function call: " + CURRENT_NAME);
}

#define ARGS CURRENT.sub.at(2).sub

void generator::e::functionDeclaration() { // declaring a function
    // parse all branches and turn them into an object
    Function obj;
    obj.type = CURRENT_NAME;
    obj.name = CURRENT_NAME_2;
    int arg_stack_pos = primitive_datatype_sizes[obj.type];
    std::vector<std::string> arguments_types;
    
    for(unsigned int i = 0; i < ARGS.size(); i += 2) {
        Variable var_obj;
        var_obj.name = ARGS.at(i + 1).name;
        var_obj.type = ARGS.at(i).name;
        arguments_types.push_back(ARGS.at(i).name);
        var_obj.position = PUSHA_SIZE + arg_stack_pos;
        if(!var_obj.size())
            error::semanticError("Cannot declare a variable argument with size 0!");
        arg_stack_pos += primitive_datatype_sizes[var_obj.type];
        obj.args.push_back(var_obj);
    }
    
    Function *target = getFunction(obj.name, arguments_types);
    
    current_branch_scope_count++;
    
    obj.defined = generator::current_branch_scope_count != generator::current_branch_scope->sub.size() && CURRENT.name == "scope";
    if(!target)
        generator::function_vector.push_back(obj);
    asm_::append_instruction(".globl", obj.generateName());
    
    if(obj.defined) { // if there isn't scope, then it's just declaration instead of definition
        if(target && target->defined)
            error::semanticError("Function '" + generateReadableFunctionName(obj.name, arguments_types) + "' already defined!");
        // save all variables and then retrieve them
        
        current_function = &obj;
        std::vector<Variable> prev_stack = asm_::stack;
        unsigned long prev_stack_pointer = asm_::stack_pointer, prev_biggest_stack_pointer = asm_::biggest_stack_pointer;
        asm_::stack_pointer = 0;
        asm_::biggest_stack_pointer = 0;
        asm_::stack.clear();
        current_branch_scope = &(CURRENT);
        int8_t prev_current_register = asm_::current_register;
        asm_::current_register = 0;

        asm_::append_instruction(obj.generateName() + ":");
        asm_::append_instruction("pusha");

        generator::main(true);

        asm_::append_instruction("popa");
        asm_::append_instruction("ret");

        asm_::current_register = prev_current_register;
        current_branch_scope = &parser::main_branch;
        asm_::stack = prev_stack;
        asm_::stack_pointer = prev_stack_pointer;
        asm_::biggest_stack_pointer = prev_biggest_stack_pointer;
        current_function = nullptr;
    }
    else
        current_branch_scope_count--;
}

#undef ARGS

#define FUNCTION_NAME function_branch.sub.at(0).name
#define FUNCTION_ARGS function_branch.sub.at(1).sub

Function *generator::e::functionCall(const Branch &function_branch) {
    // function call needs to have evaluated arguments first, to determine return value
    std::vector<std::string> argument_types;
    std::vector<unsigned long> argument_instruction_positions;
    
    asm_::nextRegister();
    for(Branch iter : FUNCTION_ARGS) {
        argument_types.push_back(expr(iter));
        int argumentSize = generator::primitive_datatype_sizes[argument_types.at(argument_types.size() - 1)];
        argument_instruction_positions.push_back(asm_::instructions.size());
        asm_::append_instruction("mov", asm_::availableRegister(argumentSize), "", argumentSize);
    }
    asm_::prevRegister();
    
    Function *target = getFunction(FUNCTION_NAME, argument_types); // go through existing functions and check if it exists
    
    if(!target)
        error::semanticError("Function '" + generateReadableFunctionName(FUNCTION_NAME, argument_types) + "' does not exist!");
    
    for(unsigned long i = 0; i < argument_instruction_positions.size(); i++)
        asm_::instructions.at(argument_instruction_positions.at(i)).arg2 = std::to_string(target->args.at(i).position - PUSHA_SIZE) + "(%rsp)";
    
    asm_::append_instruction("call", target->generateName()); // call function
    unsigned int target_size = target->size();
    for(const Variable &iter : target->args)
        target_size += iter.size();
    if(asm_::stack_pointer + target_size > asm_::biggest_stack_pointer)
        asm_::biggest_stack_pointer = asm_::stack_pointer + target_size;
    return target;
}

#undef FUNCTION_NAME
#undef FUNCTION_ARGS

void generator::e::returnStatement() { // a simple return statement
    if(primitive_datatype_sizes[current_function->type]) {
        checkForImplicitConversion(current_function->type, generator::e::expr(CURRENT.sub.at(0)));  // do calculation
        asm_::append_instruction("mov", asm_::availableRegister(current_function->size()), "+112(%rbp)", current_function->size());
        
        if(asm_::biggest_stack_pointer % 16)
            asm_::biggest_stack_pointer += 16 - asm_::biggest_stack_pointer % 16; // round to ceil of base 16
        asm_::append_instruction("add", "$" + std::to_string(asm_::biggest_stack_pointer), "%rsp");
    }
    else
        // move expression into anoher branch, because it will be ignored
        current_branch_scope->sub.insert(current_branch_scope->sub.begin() + current_branch_scope_count + 1, CURRENT.sub.at(0));
    asm_::append_instruction("popa"); // call function
    asm_::append_instruction("ret");
}

std::string generateReadableFunctionName(const std::string &name, const std::vector<std::string> &args) {
    // generate function name for errors
    std::string function_name = name + "(";
    for(unsigned long i = 0; i < args.size(); i++) {
        if(i)
            function_name.push_back(',');
        function_name += args.at(i);
    }
    function_name.push_back(')');
    return function_name;
}

Function *generator::getFunction(const std::string &name, const std::vector<std::string> &args) {
    // go through existing functions and check if it exists
    for(Function &iter : generator::function_vector)
        if(iter.name == name) {
            if(args.size() != iter.args.size())
                goto loop_end;
            for(unsigned long i = 0; i < args.size(); i++)
                if(args.at(i) != iter.args.at(i).type)
                    goto loop_end;
            return &iter;
            loop_end:;
        }
    return nullptr;
}

void f_asmtext() { // append to text section
    asm_::append_instruction(CURRENT_NAME_2, "", "", 0, Section_Text);
}

void f_asmfunc() { // append to text section
    asm_::append_instruction(CURRENT_NAME_2, "", "", 0, Section_Functions);
}

void f_asmdata() { // append to data section
    asm_::append_instruction(CURRENT_NAME_2, "", "", 0, Section_Data);
}

void f_asmbss() { // append to bss section
    asm_::append_instruction(CURRENT_NAME_2, "", "", 0, Section_Bss);
}

void f_printchar() {
    asm_::append_instruction("mov", CURRENT_NAME_2, "%al");
    asm_::append_instruction("call", "printchar");
}
