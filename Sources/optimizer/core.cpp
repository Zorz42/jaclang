#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

void optimizer::optimize(Branch* target) {
    if(target->name == "expr")
        expr(target, false);
    else
        for(Branch& iter : target->sub)
            optimize(&iter);
}

#define CURRENT(x) current_branch->sub.at(i + x)

void optimizer::expr(Branch* current_branch, const bool nested) {
    // first optimize sub expressions
    for(Branch& iter : current_branch->sub)
        if(iter.name == "expr")
            expr(&iter, true);

    // precalculate constants together
    for(unsigned int i = 0; i < current_branch->sub.size() - 1; i += 2) {
        if(isInt(CURRENT(0).name) && isInt(CURRENT(2).name)) {
            if(CURRENT(1).name == "+")
                CURRENT(0).name = std::to_string(std::stoi(CURRENT(0).name) + std::stoi(CURRENT(2).name));
            else if(CURRENT(1).name == "-")
                CURRENT(0).name = std::to_string(std::stoi(CURRENT(0).name) - std::stoi(CURRENT(2).name));
            else if(CURRENT(1).name == "*")
                CURRENT(0).name = std::to_string(std::stoi(CURRENT(0).name) * std::stoi(CURRENT(2).name));
            else if(CURRENT(1).name == "/")
                CURRENT(0).name = std::to_string(std::stoi(CURRENT(0).name) / std::stoi(CURRENT(2).name));
            else if(CURRENT(1).name == "==")
                CURRENT(0).name = std::to_string(std::stoi(CURRENT(0).name) == std::stoi(CURRENT(2).name));
            else if(CURRENT(1).name == ">")
                CURRENT(0).name = std::to_string(std::stoi(CURRENT(0).name) > std::stoi(CURRENT(2).name));
            else if(CURRENT(1).name == "<")
                CURRENT(0).name = std::to_string(std::stoi(CURRENT(0).name) < std::stoi(CURRENT(2).name));
            current_branch->sub.erase(current_branch->sub.begin() + i + 1);
            current_branch->sub.erase(current_branch->sub.begin() + i + 1);
            i -= 2;
        }
    }
    // replace the equation with its value if it has one value
    if(nested && current_branch->sub.size() == 1 && isInt(current_branch->sub.at(0).name))
        *current_branch = current_branch->sub.at(0);
}

