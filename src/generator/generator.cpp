// this generates ast into asm code

#include "jaclang.h"

void generator::main() {
    static unsigned long currentScopeOnStack = 0;
#define current currentBranchScope->sub->at(currentBranchScope->count) // current branch
    file::append_instruction("mov %rsp, %rbp");
    file::append_instruction("sub $");
    unsigned long subRsp = currentBranchScope == &mainBranch ? file::asm_text - 1 : file::asm_func - 1;
    file::append_instruction("");
    for (; currentBranchScope->count < currentBranchScope->sub->size(); currentBranchScope->count++) {
        // iterate though branches
        if (!file::outputVector.at(file::asm_text - 1).empty())
            file::append_text("");
        if (current.name == "systemFunctionCall")  // choose appropriate generator for branch
            generator::e::systemFunctionCall();
        else if (current.name == "variableDeclaration")
            generator::e::variableDeclaration(currentScopeOnStack);
        else if (current.name == "calc")
            generator::e::calculation(current);
        else if (current.name == "scope") { // if branch is scope
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
        } else if (current.name == "functionDeclaration" && generator::currentFunction == nullptr)
            generator::e::functionDeclaration();
        else if (current.name == "variableSetting")
            generator::e::variableSetting();
        else if (current.name == "returnStatement" && generator::currentFunction != nullptr)
            generator::e::returnStatement();
        else
            error::treeError("Unknown branch: " + current.name);
    }
    file::outputVector.at(subRsp).append(std::to_string(generator::biggestStackPointer) + ", %rsp");
    
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
