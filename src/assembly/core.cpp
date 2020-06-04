#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

void asm_::main() {
    for(const instruction& i : instructions) {
        std::string expr; // generates instruction: inst arg1, arg2
        expr = "   ";
        expr += i.inst;
        if(i.size)
            expr += generator::sizeKeywords[i.size];
        if(!i.arg1.empty()) {
            expr += " ";
            expr += i.arg1;
        }
        if (!i.arg2.empty()) {
            expr += ", ";
            expr += i.arg2;
        }
        switch (i.sect) {
            case section_data:
                file::append_data(expr);
                break;
            case section_bss:
                file::append_bss(expr);
                break;
            case section_text:
                file::append_text(expr);
                break;
            case section_functions:
                file::append_func(expr);
                break;
            case auto_: ;
        }
    }
}
