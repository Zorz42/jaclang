// this file parses equations

#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

#define current parser::currToken

void optimize(branch &currentBranch, const bool &nested, const bool &ignoreIsInt=false);

void putInBraces(branch& currentBranch, std::vector<std::string> operators);

branch parser::calculation(bool nested) { // parse calculation
    std::vector<std::string> equationSymbols = {"+", "-", "*", "/", "==", "<", ">"}; // valid operators in calculation
    branch currentBranch; // current branch in operation
    currentBranch.alloc();
    currentBranch.name = "calc"; // sets current branch to calculation
    if (current->text == "-") {
        appendBranch("0", currentBranch);
        appendBranch("-", currentBranch);
        parser::nextToken();
    }
    while (parser::currToken != lexer::tokens.end()) {
        if (parser::e::functionCall(currentBranch));
        else if (current->type == tt_constant) // if its constant
            appendBranch(current->text, currentBranch);
        else if (current->type == tt_indent)
            appendBranch(":" + current->text, currentBranch);
        else if (current->text == "(") { // then its nested within the operation
            parser::nextToken();
            branch obj = parser::calculation(true); // make equation until ')'
            appendBranch(obj, currentBranch);
        } else
            error::syntaxError("Value expected");
        parser::nextToken();
        if (parser::currToken == lexer::tokens.end())
            break;
        if (find(equationSymbols, current->text) != equationSymbols.size()) { // if there is valid operator
            if (parser::currToken == --lexer::tokens.end())
                error::syntaxError("File ends with a operator");
            appendBranch(current->text, currentBranch);
        } else
            break;
        parser::nextToken();
    }

    if (parser::currToken != lexer::tokens.end())
        if (current->text != ")" && parser::peekNextToken() != lexer::tokens.end())
            parser::prevToken();

#define curr(x) (currentBranch.sub->at(x).name)
#define currentObj(x) (currentBranch.sub->at(x))
#define eraseEl(x) (currentBranch.sub->erase(currentBranch.sub->begin() + x))
    putInBraces(currentBranch, {"*", "/"});
    putInBraces(currentBranch, {"+", "-"});
    
    optimize(currentBranch, nested);

    return currentBranch;
}

void putInBraces(branch& currentBranch, std::vector<std::string> operators) {
    for (unsigned long i = 1; i < currentBranch.sub->size(); i += 2)
        if (contains(operators, curr(i))) {
            branch obj;
            obj.alloc();
            obj.name = "calc";
            i--;
            appendBranch(currentObj(i), obj);
            eraseEl(i);
            while (i < currentBranch.sub->size() - 2 && contains(operators, curr(i))) {
                appendBranch(currentObj(i), obj);
                appendBranch(currentObj(i + 1), obj);
                eraseEl(i);
                eraseEl(i);
            }
            optimize(obj, true, true);
            
            currentBranch.sub->insert(currentBranch.sub->begin() + i, branch());
            currentObj(i).name = obj.name;
            currentObj(i).count = obj.count;
            if(obj.sub != nullptr) {
                currentObj(i).alloc();
                *currentObj(i).sub = *obj.sub;
            }
            i++;
        }
}

void optimize(branch &currentBranch, const bool &nested, const bool &ignoreIsInt) {
    for (unsigned int i = 0; i < currentBranch.sub->size() - 1; i += 2) {
        if (isInt(currentBranch.sub->at(i).name) && isInt(currentBranch.sub->at(i + 2).name)) {
            if (currentBranch.sub->at(i + 1).name == "+")
                currentBranch.sub->at(i).name = std::to_string(
                        std::stoi(currentBranch.sub->at(i).name) + std::stoi(currentBranch.sub->at(i + 2).name));
            else if (currentBranch.sub->at(i + 1).name == "-")
                currentBranch.sub->at(i).name = std::to_string(
                        std::stoi(currentBranch.sub->at(i).name) - std::stoi(currentBranch.sub->at(i + 2).name));
            else if (currentBranch.sub->at(i + 1).name == "*")
                currentBranch.sub->at(i).name = std::to_string(
                        std::stoi(currentBranch.sub->at(i).name) * std::stoi(currentBranch.sub->at(i + 2).name));
            else if (currentBranch.sub->at(i + 1).name == "/")
                currentBranch.sub->at(i).name = std::to_string(
                        std::stoi(currentBranch.sub->at(i).name) / std::stoi(currentBranch.sub->at(i + 2).name));
            else if (currentBranch.sub->at(i + 1).name == "==")
            currentBranch.sub->at(i).name = std::to_string(
                    std::stoi(currentBranch.sub->at(i).name) == std::stoi(currentBranch.sub->at(i + 2).name));
            currentBranch.sub->erase(currentBranch.sub->begin() + i + 1);
            currentBranch.sub->erase(currentBranch.sub->begin() + i + 1);
            i -= 2;
        }
    }
    if (nested && currentBranch.sub->size() == 1 && (isInt(currentBranch.sub->at(0).name) || ignoreIsInt)) {
        branch obj = currentBranch.sub->at(0);
        currentBranch = obj;
    }
}

