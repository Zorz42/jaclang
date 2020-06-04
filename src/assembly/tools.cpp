#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

void asm_::append_instruction(const std::string &inst, const std::string &arg1, const std::string &arg2, int size, section sect) {
    instruction obj;
    obj.inst = inst;
    obj.arg1 = arg1;
    obj.arg2 = arg2;
    obj.size = size;
    obj.sect = sect == auto_ ? generator::currentFunction == nullptr ? section_text : section_functions : sect;
    instructions.push_back(obj);
}

std::string asm_::onStack(int offset) { // output asm value of offset on stack example: offset = 4 -> -4(%rbp)
    return "-" + std::to_string(offset) + "(%rbp)"; // returns position on stack: -offset(%rbp)
}

void asm_::pushToStack(variable source) // push to stack
{
    incStackPointer(source.size());
    source.position = stackPointer;
    stack.push_back(source);
}

void asm_::nextRegister() {
    currentRegister++;
    if ((uint8_t) currentRegister == availableRegisters[0].size())
        error::treeError("register overflow");
}

void asm_::prevRegister() {
    currentRegister--;
    if (currentRegister == -1)
        error::treeError("register overflow");
}

std::string asm_::availableRegister(int8_t size, int8_t offset) {
    int index = 0;
    for (int i = size; i != 1; i /= 2)
        index++;
    return "%" + availableRegisters[index].at((unsigned long) currentRegister + offset);
}

void asm_::incStackPointer(int value) {
    stackPointer += value;
    if (stackPointer > biggestStackPointer)
        biggestStackPointer = stackPointer;
}

void asm_::decStackPointer(int value) {
    stackPointer -= value;
}
