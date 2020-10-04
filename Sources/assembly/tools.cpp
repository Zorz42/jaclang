#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

void asm_::append_instruction(const std::string &inst, const std::string &arg1, const std::string &arg2, int size, Section sect) { // append instruction token
    Instruction obj;
    obj.instruction = inst;
    obj.arg1 = arg1;
    obj.arg2 = arg2;
    obj.size = size;
    obj.section = sect == Section_Auto ? !generator::current_function ? Section_Text : Section_Functions : sect;
    instructions.emplace_back(obj);
}

std::string asm_::onStack(long offset) { // output asm value of offset on stack example: offset = -4 -> -4(%rbp)
    return std::to_string(offset) + "(%rbp)"; // returns position on stack: -offset(%rbp)
}

void asm_::pushToStack(Variable source) { // push to stack
    incStackPointer(source.size());
    source.position = -stack_pointer;
    stack.push_back(source);
}

void asm_::nextRegister() { // increment register
    if((uint8_t) ++current_register == available_registers[0].size())
        error::semanticError("register overflow");
}

void asm_::prevRegister() { // decrement register
    current_register--;
}

std::string asm_::availableRegister(int8_t size, int8_t offset) { // get available register of specific size
    int index = 0;
    for(int i = size; i != 1; i /= 2)
        index++;
    return "%" + available_registers[index].at((unsigned long) current_register + offset);
}

void asm_::incStackPointer(int value) { // increment stack pointer
    stack_pointer += value;
    if(stack_pointer > biggest_stack_pointer)
        biggest_stack_pointer = stack_pointer;
}

void asm_::decStackPointer(int value) { // decrament stack pointer
    stack_pointer -= value;
}
