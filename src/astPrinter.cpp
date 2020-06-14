// this file prints out abstract syntax tree

#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

void printAST(Branch &obj) { // print abstract syntax tree
    static int nested = 0;
    for(int i = 0; i < nested; i++) // print tabs for how deep is it nested
        std::cout << "\t";
    std::cout << obj.name << std::endl;
    if(obj.sub)
        for(Branch &iterator : *obj.sub) { // if there is another branch, print it
            nested++; // how deep is the program in ast
            printAST(iterator);
            nested--;
        }
}
