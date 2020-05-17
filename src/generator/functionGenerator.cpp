// this generates system functions

#include "jaclang.h"

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

    std::vector<variable> prevStack = generator::stack;
    int prevStackPointer = generator::stackPointer, prevBiggestStackPointer = generator::biggestStackPointer;
    generator::stackPointer = 0;
    generator::biggestStackPointer = 0;
    generator::stack = {};
    branch *prevScope = currentBranchScope;
    currentBranchScope = &(current);
    int8_t prevCurrentRegister = currentRegister;
    currentRegister = 0;

    std::string line = obj.name;
    line += ".:";
    file::append(line);
    file::append_instruction("pusha");

    generator::main();

    file::append("");
    file::append_instruction("popa");
    file::append_instruction("ret");
    file::append("");

    currentRegister = prevCurrentRegister;
    currentBranchScope = prevScope;
    generator::stack = prevStack;
    generator::stackPointer = prevStackPointer;
    generator::biggestStackPointer = prevBiggestStackPointer;
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
    file::append_instruction("call", functionName + "."); // call function
    if(generator::stackPointer + target->size() > generator::biggestStackPointer)
        generator::biggestStackPointer = generator::stackPointer + target->size();
    return target;
}

void generator::e::returnStatement() {
    checkForImplicitConversion(currentFunction->type, generator::e::calculation(current.sub->at(0)));  // do calculation
    file::append_instruction("mov" + generator::sizeKeywords[currentFunction->size()], generator::availableRegister(currentFunction->size()), "+112(%rbp)");

    file::append_instruction("add", "$" + std::to_string(generator::biggestStackPointer), "%rsp");
    file::append_instruction("popa"); // call function
    file::append_instruction("ret");
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
    std::string text = "   mov ah, ";
    text += currentName2;
    file::append_text(text);
    file::append_text("   call printchar");

}
