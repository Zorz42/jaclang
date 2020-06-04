// this generates tree calculation into asm code

#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

void operator_add(const std::string &value);
void operator_sub(const std::string &value);
void operator_mul(const std::string &value);
void operator_div(const std::string &value);
void operator_eq(const std::string &value);
void operator_gt(const std::string &value);
void operator_ls(const std::string &value);

int8_t currentValueAsmSize;

std::string getTypeMatch(const std::string &type1, const std::string &matchOperator, const std::string &type2);

std::string generator::e::calc(branch &calculation) {
    std::string currentValueType;
    std::string thisValueType;
    
    asm_::append_instruction("movq", "$0", asm_::availableRegister(8));

    if (calculation.sub->at(0).name == "functionCall") { // check if its function call at the beginning
        function* curr = generator::e::functionCall(calculation.sub->at(0).sub->at(0).name);
        asm_::append_instruction("mov", "(%rsp)", asm_::availableRegister(curr->size())); // mov return value to register
        currentValueType = "int";
    } else if (calculation.sub->at(0).name.at(0) == ':') { // variables will have : at the beginning
        std::string value = calculation.sub->at(0).name; // variable name
        value.erase(value.begin()); // remove :
        variable curr = generator::get_variable(value);
        asm_::append_instruction("mov", asm_::onStack(curr.position), asm_::availableRegister(curr.size())); // mov first value to register
        currentValueType = curr.type;
    } else if (calculation.sub->at(0).name == "calc") {
        asm_::nextRegister(); // its just nested calculation
        currentValueType = generator::e::calc(calculation.sub->at(0));
        asm_::append_instruction("mov", asm_::availableRegister(8), asm_::availableRegister(8, -1));
        asm_::prevRegister();
    } else { // else its just constant
        asm_::append_instruction("mov", "$" + calculation.sub->at(0).name, asm_::availableRegister(8));
        currentValueType = "int";
    }

#define currentValue calculation.sub->at(i).name
#define currentOperator calculation.sub->at(i - 1).name

    for (unsigned long i = 2; i <= calculation.sub->size(); i += 2) {
        std::string currentValueAsm = currentValue;
        currentValueAsmSize = 4; // default - int (4 bytes)
        if (currentValueAsm.at(0) == ':') { // if its variable
            std::string value = currentValueAsm; // variable name
            value.erase(value.begin()); // remove :
            variable curr = generator::get_variable(value);
            currentValueAsm = asm_::onStack(curr.position);
            currentValueAsmSize = curr.size();
            thisValueType = curr.type;
        } else if (currentValue == "calc") { // if value is calculation
            asm_::nextRegister();
            thisValueType = generator::e::calc(calculation.sub->at(i));
            asm_::prevRegister();

            currentValueAsm = asm_::availableRegister(8, 1);
            currentValueAsmSize = 8;
        } else if (currentValue == "functionCall") {
            function *thisFunction = generator::e::functionCall(calculation.sub->at(i).sub->at(0).name);
            thisValueType = thisFunction->type;
            currentValueAsm = "(%rsp)";
            currentValueAsmSize = thisFunction->size();
        } else {
            thisValueType = "int";
            currentValueAsm = "$" + currentValueAsm;
        }
        if (currentOperator == "+") // default cases for operators
            operator_add(currentValueAsm);
        else if (currentOperator == "-")
            operator_sub(currentValueAsm);
        else if (currentOperator == "*")
            operator_mul(currentValueAsm);
        else if (currentOperator == "/")
            operator_div(currentValueAsm);
        else if (currentOperator == "==")
            operator_eq(currentValueAsm);
        else if (currentOperator == ">")
            operator_gt(currentValueAsm);
        else if (currentOperator == "<")
            operator_ls(currentValueAsm);
        else
            error::treeError("unrecognized operator");
        currentValueType = getTypeMatch(currentValueType, currentOperator, thisValueType);
    }
    return currentValueType;
}

void operator_add(const std::string &value) {
    asm_::append_instruction("add", value, asm_::availableRegister(currentValueAsmSize), currentValueAsmSize);
}

void operator_sub(const std::string &value) {
    asm_::append_instruction("sub", value, asm_::availableRegister(currentValueAsmSize), currentValueAsmSize);
}

std::string getRegister(int8_t size) {
    switch (size) {
        case 1: return "%al";
        case 2: return "%ax";
        case 4: return "%eax";
        case 8: return "%rax";
    }
    return "";
}

void operator_mul(const std::string &value) {
    std::string reg = getRegister(currentValueAsmSize);
    asm_::append_instruction("mov", value, reg, currentValueAsmSize);
    asm_::append_instruction("imul", asm_::availableRegister(currentValueAsmSize));
    asm_::append_instruction("mov", reg, asm_::availableRegister(currentValueAsmSize), currentValueAsmSize);
}

void operator_div(const std::string &value) {
    std::string reg = getRegister(currentValueAsmSize);
    asm_::append_instruction("mov", asm_::availableRegister(currentValueAsmSize), reg);
    asm_::append_instruction(std::string("c") + "d" + "q");
    asm_::append_instruction("mov", value, asm_::availableRegister(currentValueAsmSize), currentValueAsmSize);
    asm_::append_instruction("idiv", asm_::availableRegister(currentValueAsmSize));
    asm_::append_instruction("mov", reg, asm_::availableRegister(currentValueAsmSize));
}

void operator_eq(const std::string &value) {
    asm_::append_instruction("cmp", value, asm_::availableRegister(currentValueAsmSize));
    asm_::append_instruction("sete", asm_::availableRegister(1));
    asm_::append_instruction("movzbq", asm_::availableRegister(1), asm_::availableRegister(8));
}

void operator_gt(const std::string &value) {
    asm_::append_instruction("cmp", value, asm_::availableRegister(currentValueAsmSize));
    asm_::append_instruction("setg", asm_::availableRegister(1));
    asm_::append_instruction("movzbq", asm_::availableRegister(1), asm_::availableRegister(8));
}

void operator_ls(const std::string &value) {
    asm_::append_instruction("cmp", value, asm_::availableRegister(currentValueAsmSize));
    asm_::append_instruction("setl", asm_::availableRegister(1));
    asm_::append_instruction("movzbq", asm_::availableRegister(1), asm_::availableRegister(8));
}

std::string getTypeMatch(const std::string &type1, const std::string &matchOperator, const std::string &type2) {
    datatypeMatches current;
    for (const datatypeMatches &currDatatypeMatches : generator::operatorMatches)
        if (currDatatypeMatches.datatype == type1 + matchOperator) {
            current = currDatatypeMatches;
            break;
        }
    for (const match &currMatch : current.matches) // its repeated, because it needs to first look at passing values without implicit conversation
        if (currMatch.type == type2)
            return currMatch.result;

    for (const match &currMatch : current.matches)
        for (const std::string &implicitConversionResult : generator::implicitConversations[type2])
            if (implicitConversionResult == currMatch.type)
                return currMatch.result;
    error::treeError("No match for operator '" + matchOperator + "' between '" + type1 + "' and '" + type2 + "'");
    return ""; // to avoid warnings
}
