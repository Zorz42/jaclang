// this generates variable declarations

#include "jaclang.h"

#define current currentBranchScope->sub->at(currentBranchScope->count)

void generator::e::variableDeclaration(unsigned long scopeOnStack) {
    variable obj; // obj variable
    obj.type = current.sub->at(0).name; // datatype
    obj.name = current.sub->at(1).name; // name

    unsigned int i = 0;

    for (const variable &iter : generator::stack) { // go through stack
        if (iter.name == current.sub->at(1).name &&
            i >= scopeOnStack) { // if this variable already exists, then report error
            std::string errorString = iter.name;
            errorString += " already exists as a variable";
            error::treeError(errorString);
        }
        i++;
    }

    generator::pushToStack(obj); // push to stack

    if (current.sub->at(2).name == "calc") {
        checkForImplicitConversion(obj.type, generator::e::calculation(current.sub->at(2)));  // do calculation

        file::append_instruction("mov" + generator::sizeKeywords[obj.size()],
                                 generator::availableRegister(obj.size()), onStack(generator::stackPointer)); // set variable on stack
    } else {
        if (!isInt(current.sub->at(2).name))
            error::treeError("variable declaration must be type int");

        file::append_instruction("mov" + generator::sizeKeywords[obj.size()],
                                 "$" + current.sub->at(2).name, onStack(generator::stackPointer));
    }
}

void generator::e::variableSetting() {
    bool variableExists = false;
    variable currentVariable;

    for (const variable &iter : generator::stack) // go through stack
        if (iter.name == current.sub->at(0).name) {
            variableExists = true;
            currentVariable = iter;
            break;
        }
    if (!variableExists) {
        std::string errorString = current.sub->at(0).name;
        errorString += " does not exist as a variable";
        error::treeError(errorString);
    }

    if (current.sub->at(1).name == "calc") {
        checkForImplicitConversion(currentVariable.type,
                                   generator::e::calculation(current.sub->at(1))); // do calculation

        file::append_instruction("mov", generator::sizeKeywords[currentVariable.size()] + " " +
                                        onStack(currentVariable.position),
                                 generator::availableRegister(currentVariable.size())); // set variable on stack
    } else {
        if (!isInt(current.sub->at(1).name))
            error::treeError("int setting must be type int");

        file::append_instruction("mov", generator::sizeKeywords[currentVariable.size()] + " " +
                                        onStack(currentVariable.position), current.sub->at(1).name);
    }
}

variable generator::get_variable(const std::string &name) {
    variable obj;
    bool varExists = false; // go through stack and check if variable exists
    for (const variable &iter : generator::stack)
        if (iter.name == name) {
            varExists = true;
            obj = iter;
            break;
        }
    if (!varExists) {
        std::string errorString = name;
        errorString += " does not exist";
        error::treeError(errorString);
    }
    return obj;
}

void generator::checkForImplicitConversion(const std::string &dest, const std::string &source) {
    bool success = dest == source;
    if (!success)
        for (const std::string &conversation : generator::implicitConversations[source])
            if (dest == conversation)
                success = true;
    if (!success)
        error::treeError("Could not convert '" + source + "' to '" + dest + "'");
}
