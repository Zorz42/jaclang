#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

#define current currentBranchScope->sub->at(currentBranchScope->count)

void generator::e::ifStatement() {
    static unsigned int ifCounter = 0;
    if(current.sub->at(0).name == "calc") {
        generator::e::calc(current.sub->at(0));
        file::append_instruction("cmpq", "$0", "%rbx");
        file::append_instruction("je", "IFE" + std::to_string(ifCounter));
        currentBranchScope->count++;
        scope();
        
        file::append_instruction("IFE" + std::to_string(ifCounter) + ":");
        ifCounter++;
    }
    else {
        if(current.sub->at(0).name != "0") {
            scope();
        }
        currentBranchScope->count++;
    }
}

void generator::e::whileStatement() {
    static unsigned int whileCounter = 0;
    if(current.sub->at(0).name == "calc") {
        file::append_instruction("WHS" + std::to_string(whileCounter) + ":");
        generator::e::calc(current.sub->at(0));
        file::append_instruction("cmpq", "$0", "%rbx");
        file::append_instruction("je", "WHE" + std::to_string(whileCounter));
        currentBranchScope->count++;
        scope();
        file::append_instruction("jmp", "WHS" + std::to_string(whileCounter));
        file::append_instruction("WHE" + std::to_string(whileCounter) + ":");
        whileCounter++;
    }
    else {
        if(current.sub->at(0).name != "0") {
            scope();
        }
        currentBranchScope->count++;
    }
}
