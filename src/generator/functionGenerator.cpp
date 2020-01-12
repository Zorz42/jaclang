// this generates system functions

#include "jaclang.h"

#define current currentBranchScope->sub.at(currentBranchScope->count) // function branch
#define currentName  currentBranchScope->sub.at(currentBranchScope->count).sub.at(0).name // name of function
#define currentName2 currentBranchScope->sub.at(currentBranchScope->count).sub.at(1).name

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
    obj.name = currentName2;
    for (const function &i : generator::functionVector)
        if (i.name == obj.name)
            error::treeError("Function already declared!");
    generator::functionVector.push_back(obj);
    currentBranchScope->count++;
    std::vector<variable> prevStack = generator::stack;
    int prevStackPointer = generator::stackPointer;
    generator::stackPointer = 0;
    generator::stack = {};
    if (current.name != "scope")
        error::treeError("Expected scope after function declaration!");
    generator::inFunction = true;
    std::string line = obj.name;
    line += ".:";
    file::append(line);

    file::append_instruction("pusha");
    file::append_instruction("mov", "rbp", "rsp");
    file::append("");

    branch *prevScope = currentBranchScope;
    currentBranchScope = &(current);
    generator::main();

    file::append("");
    file::append_instruction("popa");
    file::append_instruction("ret");
    file::append("");

    generator::inFunction = false;
    currentBranchScope = prevScope;
    generator::stack = prevStack;
    generator::stackPointer = prevStackPointer;
}

void generator::e::functionCall(const std::string &variableName) {
    bool funcExists = false; // go through existing functions and check if it exists
    for (const function &iter : generator::functionVector)
        if (iter.name == variableName) {
            funcExists = true;
            break;
        }
    if (!funcExists)
        error::treeError("Function does not exist!");
    file::append_instruction("call", variableName + "."); // call function
}

void generator::e::returnStatement() {
    file::append_instruction("pop", "rbp"); // call function
    file::append_instruction("ret");
}

std::string generateAsmText() // generate text for inline assembly [text]  ;;__asm__
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
