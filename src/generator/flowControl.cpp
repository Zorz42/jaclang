#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

#define current currentBranchScope->sub->at(currentBranchScope->count)

void generator::e::ifStatement() {
    if(current.sub->at(0).name == "calc") {
        generator::e::calculation(current.sub->at(0));
        file::append_instruction("cmpq", "$0", "%rbx");
        file::append_instruction("je", "ifend");
        currentBranchScope->count++;
        
        scope();
        
        file::append_instruction("ifend:");
    }
    else {
        if(current.sub->at(0).name != "0") {
            scope();
        }
        currentBranchScope->count++;
    }
}
