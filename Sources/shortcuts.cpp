// there are defined functions, that are often used to make code more readable

#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

bool contains(std::vector<std::string> &list, const std::string &text) { // check if vector of strings contains string
    return std::find(list.begin(), list.end(), text) != list.end();
}

unsigned long find(std::vector<std::string> &source, const std::string &target) { // find string in vector of strings
    return std::find(source.begin(), source.end(), target) - source.begin();
}
