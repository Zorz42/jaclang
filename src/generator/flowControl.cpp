#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

#define CURRENT current_branch_scope->sub.at(current_branch_scope_count)

void generator::e::ifStatement() { // simple if statement generator
    static unsigned int if_counter = 0;
    if(CURRENT.sub.at(0).name == "expr") { // IFE<index>: is label, which stands for IF END, to skip if statement if it's false
        generator::e::expr(CURRENT.sub.at(0));
        asm_::append_instruction("cmpq", "$0", "%rbx");
        asm_::append_instruction("je", "IFE" + std::to_string(if_counter));
        current_branch_scope_count++;
        scope();
        
        asm_::append_instruction("IFE" + std::to_string(if_counter) + ":");
        if_counter++;
    }
    else {
        if(CURRENT.sub.at(0).name != "0") {
            scope();
        }
        current_branch_scope_count++;
    }
}

void generator::e::whileStatement() {
    static unsigned int while_counter = 0;
    if(CURRENT.sub.at(0).name == "expr") { // WHS = WHILE START, WHE = WHILE END
        asm_::append_instruction("WHS" + std::to_string(while_counter) + ":");
        generator::e::expr(CURRENT.sub.at(0));
        asm_::append_instruction("cmpq", "$0", "%rbx");
        asm_::append_instruction("je", "WHE" + std::to_string(while_counter));
        current_branch_scope_count++;
        scope();
        asm_::append_instruction("jmp", "WHS" + std::to_string(while_counter));
        asm_::append_instruction("WHE" + std::to_string(while_counter) + ":");
        while_counter++;
    }
    else {
        if(CURRENT.sub.at(0).name != "0") {
            scope();
        }
        current_branch_scope_count++;
    }
}
