// this generates ast into asm code

#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

#define current currentBranchScope->sub->at(currentBranchScope->count) // current branch

void generator::main(bool inFunction) {
    if (inFunction) {
        file::append_instruction("mov %rsp, %rbp");
        file::append_instruction("sub $");
    }
    unsigned long subRsp = 0;
    if (inFunction)
        subRsp = currentBranchScope == &mainBranch ? file::asm_text - 1 : file::asm_func - 1;
    file::append_instruction("");
    for (; currentBranchScope->count < currentBranchScope->sub->size(); currentBranchScope->count++) {
        // iterate though branches
        if (!file::outputVector.at(file::asm_text - 1).empty())
            file::append_text("");
        if (current.name == "systemFunctionCall")  // choose appropriate generator for branch
            e::systemFunctionCall();
        else if (current.name == "variableDeclaration")
            e::variableDeclaration(currentScopeOnStack);
        else if (current.name == "calc")
            e::calculation(current);
        else if (current.name == "scope") // if branch is scope
            e::scope();
        else if (current.name == "functionDeclaration" && currentFunction == nullptr)
            e::functionDeclaration();
        else if (current.name == "variableSetting")
            e::variableSetting();
        else if (current.name == "returnStatement" && currentFunction != nullptr)
            e::returnStatement();
        else if (current.name == "ifStatement")
            e::ifStatement();
        else
            error::treeError("Unknown branch: " + current.name);
    }
    if (inFunction) 
        file::outputVector.at(subRsp).append(std::to_string(biggestStackPointer) + ", %rsp");
    
}

void generator::pushToStack(variable source) // push to stack
{
    generator::incStackPointer(source.size());
    source.position = generator::stackPointer;
    generator::stack.push_back(source);
}

void generator::nextRegister() {
    generator::currentRegister++;
    if ((uint8_t) generator::currentRegister == generator::availableRegisters[0].size())
        error::treeError("register overflow");
}

void generator::prevRegister() {
    generator::currentRegister--;
    if (generator::currentRegister == -1)
        error::treeError("register overflow");
}

std::string generator::availableRegister(int8_t size, int8_t offset) {
    int index = 0;
    for (int i = size; i != 1; i /= 2)
        index++;
    return "%" + generator::availableRegisters[index].at((unsigned long) generator::currentRegister + offset);
}

int8_t getTypeSize(const std::string &type) {
    return int8_t(generator::primitiveDatatypeSizes[type]);
}

int8_t variable::size() {
    return getTypeSize(type);
}

int8_t function::size() const {
    return getTypeSize(type);
}

void generator::incStackPointer(int value) {
    stackPointer += value;
    if (stackPointer > biggestStackPointer)
        biggestStackPointer = stackPointer;
}

void generator::decStackPointer(int value) {
    stackPointer -= value;
}

void generator::e::scope() {
    branch *prevScope = currentBranchScope; // save current scope
    currentBranchScope = &(current);        // move to new scope
    unsigned long stackLength = generator::stack.size();  // save stack length
    unsigned long prevScopeOnStack = currentScopeOnStack; // save scope on stack
    currentScopeOnStack = stackLength;  // set scope on stack
    generator::main();
    currentScopeOnStack = prevScopeOnStack; // retrieve scope on stack
    while (generator::stack.size() > stackLength) // remove elements from stack that were in scope
    {
        decStackPointer(generator::stack.at(generator::stack.size() - 1).size());
        generator::stack.pop_back();
    }
    currentBranchScope = prevScope; // retrieve current branch scope
}
