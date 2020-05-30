// this generates system functions

#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

#define current currentBranchScope->sub->at(currentBranchScope->count) // function branch
#define currentName  currentBranchScope->sub->at(currentBranchScope->count).sub->at(0).name // name of function
#define currentName2 currentBranchScope->sub->at(currentBranchScope->count).sub->at(1).name

void f_asmtext();

void f_asmdata();

void f_asmbss();

void f_printchar();

void generator::e::systemFunctionCall() // system function: __test__
{
    if (currentName == "__asm__" ||
        currentName == "__asmtext__") // asm is alternative to asm text, because asmtext is the most common asm call
        f_asmtext();
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

    std::vector<variable> prevStack = at::stack;
    int prevStackPointer = at::stackPointer, prevBiggestStackPointer = at::biggestStackPointer;
    at::stackPointer = 0;
    at::biggestStackPointer = 0;
    at::stack = {};
    branch *prevScope = currentBranchScope;
    currentBranchScope = &(current);
    int8_t prevCurrentRegister = at::currentRegister;
    at::currentRegister = 0;

    std::string line = obj.name;
    line += ".:";
    file::append(line);
    at::append_instruction("pusha");

    generator::main(true);

    file::append("");
    at::append_instruction("popa");
    at::append_instruction("ret");
    file::append("");

    at::currentRegister = prevCurrentRegister;
    currentBranchScope = prevScope;
    at::stack = prevStack;
    at::stackPointer = prevStackPointer;
    at::biggestStackPointer = prevBiggestStackPointer;
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
    at::append_instruction("call", functionName + "."); // call function
    if(at::stackPointer + target->size() > at::biggestStackPointer)
        at::biggestStackPointer = at::stackPointer + target->size();
    return target;
}

void generator::e::returnStatement() {
    checkForImplicitConversion(currentFunction->type, generator::e::calc(current.sub->at(0)));  // do calculation
    at::append_instruction("mov" + generator::sizeKeywords[currentFunction->size()], at::availableRegister(currentFunction->size()), "+112(%rbp)");

    at::append_instruction("add", "$" + std::to_string(at::biggestStackPointer), "%rsp");
    at::append_instruction("popa"); // call function
    at::append_instruction("ret");
}

std::string generateAsmText() // generate text for inline assembly
{
    std::string text = "   ";
    text += currentName2;
    return text;
}

void f_asmtext() // append to text section
{
    file::append_text(generateAsmText());
}

void f_asmdata() // append to data section
{
    file::append_data(generateAsmText());
}

void f_asmbss() // append to bss section
{
    file::append_bss(generateAsmText());
}

void f_printchar() {
    std::string text = "   mov ";
    text += currentName2;
    text += ", %al";
    file::append_text(text);
    file::append_text("   call printchar");

}
