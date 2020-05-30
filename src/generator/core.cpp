// this generates ast into asm code

#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

#define current currentBranchScope->sub->at(currentBranchScope->count) // current branch

#define _CASE_1(_CASE) else if (current.name == #_CASE) e::_CASE();
#define _CASE_2(_CASE, _COND) else if (current.name == #_CASE && (_COND)) e::_CASE();
#define _CASE_3(_CASE, _COND, _ARGS) else if (current.name == #_CASE && (_COND)) e::_CASE(_ARGS);

#define _GET_2ND_ARG(_ARG1, _ARG2, _ARG3, _ARG4, ...) _ARG4
#define _CASE_MACRO_CHOOSER(...) _GET_2ND_ARG(__VA_ARGS__, _CASE_3, _CASE_2, _CASE_1)

#define case_(...) _CASE_MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

void generator::main(bool inFunction) {
    if (inFunction) {
        at::append_instruction("mov %rsp, %rbp");
        at::append_instruction("sub $");
    }
    unsigned long subRsp = 0;
    if (inFunction)
        subRsp = currentBranchScope == &mainBranch ? file::asm_text - 1 : file::asm_func - 1;
    at::append_instruction("");
    for (; currentBranchScope->count < currentBranchScope->sub->size(); currentBranchScope->count++) {
        // iterate though branches
        if (!file::outputVector.at(file::asm_text - 1).empty())
            file::append_text("");
        
        if (current.name == "systemFunctionCall")  // choose appropriate generator for branch
            e::systemFunctionCall();
        case_(variableDeclaration, true, at::currentScopeOnStack)
        case_(calc, true, current)
        case_(scope)
        case_(functionDeclaration, currentFunction == nullptr)
        case_(variableSetting)
        case_(returnStatement, currentFunction != nullptr)
        case_(ifStatement)
        case_(whileStatement)
        else
            error::treeError("Unknown branch: " + current.name);
    }
    if (inFunction) 
        file::outputVector.at(subRsp).append(std::to_string(at::biggestStackPointer) + ", %rsp");
    
}

int8_t getTypeSize(const std::string &type) {
    return int8_t(generator::primitiveDatatypeSizes[type]);
}

int8_t function::size() const {
    return getTypeSize(type);
}

int8_t variable::size() {
    return getTypeSize(type);
}
