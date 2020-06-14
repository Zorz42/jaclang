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

int8_t current_value_size;
std::string getTypeMatch(const std::string &type1, const std::string &match_operator, const std::string &type2);

#define FIRST calculation.sub->at(0)

std::string generator::e::expr(Branch &calculation) {
    std::string current_calue_type;
    std::string this_value_type;
    
    asm_::append_instruction("movq", "$0", asm_::availableRegister(8));

    if(FIRST.name == "functionCall") { // check if its function call at the beginning
        Function* curr = generator::e::functionCall(FIRST);
        asm_::append_instruction("mov", "(%rsp)", asm_::availableRegister(curr->size())); // mov return value to register
        current_calue_type = "int";
    } else if(FIRST.name.at(0) == ':') { // variables will have : at the beginning
        std::string value = FIRST.name; // variable name
        value.erase(value.begin()); // remove :
        Variable *curr = generator::getVariable(value);
        asm_::append_instruction("mov", curr->generateAddress(), asm_::availableRegister(curr->size())); // mov first value to register
        current_calue_type = curr->type;
    } else if(FIRST.name == "expr") {
        asm_::nextRegister(); // its just nested calculation
        current_calue_type = generator::e::expr(FIRST);
        asm_::append_instruction("mov", asm_::availableRegister(8), asm_::availableRegister(8, -1));
        asm_::prevRegister();
    } else { // else its just constant
        asm_::append_instruction("mov", "$" + FIRST.name, asm_::availableRegister(8));
        current_calue_type = "int";
    }

#define CURRENT_VALUE calculation.sub->at(i).name
#define CURRENT_OPERATOR calculation.sub->at(i - 1).name

    for(unsigned long i = 2; i <= calculation.sub->size(); i += 2) {
        std::string current_value_asm = CURRENT_VALUE;
        current_value_size = 4; // default - int (4 bytes)
        if(current_value_asm.at(0) == ':') { // if its variable
            std::string value = current_value_asm; // variable name
            value.erase(value.begin()); // remove :
            Variable *curr = generator::getVariable(value);
            current_value_asm = curr->generateAddress();
            current_value_size = curr->size();
            this_value_type = curr->type;
        } else if(CURRENT_VALUE == "expr") { // if value is calculation
            asm_::nextRegister();
            this_value_type = generator::e::expr(calculation.sub->at(i));
            asm_::prevRegister();

            current_value_asm = asm_::availableRegister(8, 1);
            current_value_size = 8;
        } else if(CURRENT_VALUE == "functionCall") {
            Function *this_function = generator::e::functionCall(calculation.sub->at(i));
            this_value_type = this_function->type;
            current_value_asm = "(%rsp)";
            current_value_size = this_function->size();
        } else {
            this_value_type = "int";
            current_value_asm = "$" + current_value_asm;
        }
        if(CURRENT_OPERATOR == "+") // default cases for operators
            operator_add(current_value_asm);
        else if(CURRENT_OPERATOR == "-")
            operator_sub(current_value_asm);
        else if(CURRENT_OPERATOR == "*")
            operator_mul(current_value_asm);
        else if(CURRENT_OPERATOR == "/")
            operator_div(current_value_asm);
        else if(CURRENT_OPERATOR == "==")
            operator_eq(current_value_asm);
        else if(CURRENT_OPERATOR == ">")
            operator_gt(current_value_asm);
        else if(CURRENT_OPERATOR == "<")
            operator_ls(current_value_asm);
        else
            error::treeError("unrecognized operator");
        current_calue_type = getTypeMatch(current_calue_type, CURRENT_OPERATOR, this_value_type);
    }
    return current_calue_type;
}

void operator_add(const std::string &value) {
    asm_::append_instruction("add", value, asm_::availableRegister(current_value_size), current_value_size);
}

void operator_sub(const std::string &value) {
    asm_::append_instruction("sub", value, asm_::availableRegister(current_value_size), current_value_size);
}

std::string getRegisterA(int8_t size) { // get register A of size, for multiplication
    switch(size) {
        case 1: return "%al";
        case 2: return "%ax";
        case 4: return "%eax";
        case 8: return "%rax";
    }
    return "";
}

void operator_mul(const std::string &value) { // various operators
    std::string reg = getRegisterA(current_value_size);
    asm_::append_instruction("mov", value, reg, current_value_size);
    asm_::append_instruction("imul", asm_::availableRegister(current_value_size));
    asm_::append_instruction("mov", reg, asm_::availableRegister(current_value_size), current_value_size);
}

void operator_div(const std::string &value) {
    std::string reg = getRegisterA(current_value_size);
    asm_::append_instruction("mov", asm_::availableRegister(current_value_size), reg);
    asm_::append_instruction(std::string("c") + "d" + "q");
    asm_::append_instruction("mov", value, asm_::availableRegister(current_value_size), current_value_size);
    asm_::append_instruction("idiv", asm_::availableRegister(current_value_size));
    asm_::append_instruction("mov", reg, asm_::availableRegister(current_value_size));
}

void operator_eq(const std::string &value) { // for comparison, it returns 1 as true and 0 as false
    // operator equals
    asm_::append_instruction("cmp", value, asm_::availableRegister(current_value_size));
    asm_::append_instruction("sete", asm_::availableRegister(1));
    asm_::append_instruction("movzbq", asm_::availableRegister(1), asm_::availableRegister(8));
}

void operator_gt(const std::string &value) { // greater
    asm_::append_instruction("cmp", value, asm_::availableRegister(current_value_size));
    asm_::append_instruction("setg", asm_::availableRegister(1));
    asm_::append_instruction("movzbq", asm_::availableRegister(1), asm_::availableRegister(8));
}

void operator_ls(const std::string &value) { // lesser
    asm_::append_instruction("cmp", value, asm_::availableRegister(current_value_size));
    asm_::append_instruction("setl", asm_::availableRegister(1));
    asm_::append_instruction("movzbq", asm_::availableRegister(1), asm_::availableRegister(8));
}

std::string getTypeMatch(const std::string &type1, const std::string &match_operator, const std::string &type2) { // get match for combining two values with an operator and returns result value type. For primitives it will always be: <a> + <b> = <a> example: int + long = int
    DatatypeMatches current;
    for(const DatatypeMatches &curr_datatype_matches : generator::operator_matches)
        if(curr_datatype_matches.datatype == type1 + match_operator) {
            current = curr_datatype_matches;
            break;
        }
    for(const Match &curr_match : current.matches) // its repeated, because it needs to first look at passing values without implicit conversation
        if(curr_match.type == type2)
            return curr_match.result;

    for(const Match &curr_match : current.matches)
        for(const std::string &implicitConversionResult : generator::implicit_conversations[type2])
            if(implicitConversionResult == curr_match.type)
                return curr_match.result;
    error::treeError("No match for operator '" + match_operator + "' between '" + type1 + "' and '" + type2 + "'");
    return ""; // to avoid warnings
}
