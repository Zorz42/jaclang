// this generates system functions

#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

#define current currentBranchScope->sub->at(currentBranchScope->count) // function branch
#define currentName  currentBranchScope->sub->at(currentBranchScope->count).sub->at(0).name // name of function
#define currentName2 currentBranchScope->sub->at(currentBranchScope->count).sub->at(1).name

void f_asmtext();
void f_asmfunc();
void f_asmdata();
void f_asmbss();
void f_printchar();

void generator::e::systemFunctionCall() // system function: __test__
{
    if (currentName == "__asm__") {
        if(generator::currentFunction == nullptr)
            f_asmtext();
        else
            f_asmfunc();
    }
    else if (currentName == "__asmdata__")
        f_asmdata();
    else if (currentName == "__asmbss__")
        f_asmbss();
    else if (currentName == "__printchar__")
        f_printchar();
    else
        error::treeError("Unknown system function call: " + currentName);
}

void generator::e::functionDeclaration() {
    function obj;
    currentFunction = &obj;
    obj.type = currentName;
    obj.name = currentName2;
    for (const function &i : generator::functionVector)
        if (i.name == obj.name)
            error::treeError("Function already declared!");

    generator::functionVector.push_back(obj);
    currentBranchScope->count++;

    if (current.name != "scope")
        error::treeError("Expected scope after function declaration!");

    std::vector<variable> prevStack = asm_::stack;
    int prevStackPointer = asm_::stackPointer, prevBiggestStackPointer = asm_::biggestStackPointer;
    asm_::stackPointer = 0;
    asm_::biggestStackPointer = 0;
    asm_::stack = {};
    branch *prevScope = currentBranchScope;
    currentBranchScope = &(current);
    int8_t prevCurrentRegister = asm_::currentRegister;
    asm_::currentRegister = 0;

    std::string line = obj.name;
    line += ".:";
    asm_::append_instruction(line);
    asm_::append_instruction("pusha");

    generator::main(true);

    asm_::append_instruction("");
    asm_::append_instruction("popa");
    asm_::append_instruction("ret");
    asm_::append_instruction("");

    asm_::currentRegister = prevCurrentRegister;
    currentBranchScope = prevScope;
    asm_::stack = prevStack;
    asm_::stackPointer = prevStackPointer;
    asm_::biggestStackPointer = prevBiggestStackPointer;
    currentFunction = nullptr;
}

function *generator::e::functionCall(const std::string &functionName) {
    bool funcExists = false; // go through existing functions and check if it exists
    function *target = nullptr;
    for (const function &iter : generator::functionVector)
        if (iter.name == functionName) {
            funcExists = true;
            target = (function *) &iter;
            break;
        }
    if (!funcExists)
        error::treeError("Function does not exist!");
    asm_::append_instruction("call", functionName + "."); // call function
    if(asm_::stackPointer + target->size() > asm_::biggestStackPointer)
        asm_::biggestStackPointer = asm_::stackPointer + target->size();
    return target;
}

void generator::e::returnStatement() {
    checkForImplicitConversion(currentFunction->type, generator::e::calc(current.sub->at(0)));  // do calculation
    asm_::append_instruction("mov", asm_::availableRegister(currentFunction->size()), "+112(%rbp)", currentFunction->size());

    asm_::append_instruction("add", "$" + std::to_string(asm_::biggestStackPointer), "%rsp");
    asm_::append_instruction("popa"); // call function
    asm_::append_instruction("ret");
}

void f_asmtext() // append to text section
{
    asm_::append_instruction(currentName2, "", "", 0, section_text);
}

void f_asmfunc() // append to text section
{
    asm_::append_instruction(currentName2, "", "", 0, section_functions);
}

void f_asmdata() // append to data section
{
    asm_::append_instruction(currentName2, "", "", 0, section_data);
}

void f_asmbss() // append to bss section
{
    asm_::append_instruction(currentName2, "", "", 0, section_bss);
}

void f_printchar() {
    asm_::append_instruction("mov", currentName2, "%al");
    asm_::append_instruction("call", "printchar");
}
