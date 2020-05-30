#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

void at::append_instruction(const std::string &instruction, const std::string &arg1, const std::string &arg2) {
    std::string expr; // returns asm instruction: instruction arg1, arg2
    expr = "   ";
    expr += instruction;
    if(!arg1.empty())
        expr += " ";
    expr += arg1;
    if (!arg2.empty())
        expr += ", ";
    expr += arg2;
    file::append(expr);
}

std::string at::onStack(int offset) { // output asm value of offset on stack example: offset = 4 -> -4(%rbp)
    return "-" + std::to_string(offset) + "(%rbp)"; // returns position on stack: -offset(%rbp)
}

void at::pushToStack(variable source) // push to stack
{
    incStackPointer(source.size());
    source.position = stackPointer;
    stack.push_back(source);
}

void at::nextRegister() {
    currentRegister++;
    if ((uint8_t) currentRegister == availableRegisters[0].size())
        error::treeError("register overflow");
}

void at::prevRegister() {
    currentRegister--;
    if (currentRegister == -1)
        error::treeError("register overflow");
}

std::string at::availableRegister(int8_t size, int8_t offset) {
    int index = 0;
    for (int i = size; i != 1; i /= 2)
        index++;
    return "%" + availableRegisters[index].at((unsigned long) currentRegister + offset);
}

void at::incStackPointer(int value) {
    stackPointer += value;
    if (stackPointer > biggestStackPointer)
        biggestStackPointer = stackPointer;
}

void at::decStackPointer(int value) {
    stackPointer -= value;
}
