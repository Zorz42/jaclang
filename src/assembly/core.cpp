#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

void asm_::main() {
     std::unordered_map<int8_t, std::string> size_keywords;
    
    size_keywords.reserve(4);
    size_keywords[1] = "b";
    size_keywords[2] = "w";
    size_keywords[4] = "l";
    size_keywords[8] = "q";
    for(const Instruction& i : instructions) {
        std::string expr; // generates instruction: inst arg1, arg2
        if(i.instruction.size() && i.instruction.at(i.instruction.size() - 1) != ':') // if its label do not shift to the right
            expr = "   ";
        expr += i.instruction;
        if(i.size)
            expr += size_keywords[i.size];
        if(!i.arg1.empty()) {
            expr += " ";
            expr += i.arg1;
        }
        if(!i.arg2.empty()) {
            expr += ", ";
            expr += i.arg2;
        }
        switch(i.section) { // for each section defined
            case Section_Data:
                file::appendData(expr);
                break;
            case Section_Bss:
                file::appendBss(expr);
                break;
            case Section_Text:
                file::appendText(expr);
                break;
            case Section_Functions:
                file::appendFunc(expr);
                break;
            case Section_Auto: ;
        }
    }
}
