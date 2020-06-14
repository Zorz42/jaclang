// there are defined functions, that are often used to make code more readable

#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

bool contains(std::vector<std::string> &list, const std::string &text) { // check if vector of strings contains string
    return std::find(list.begin(), list.end(), text) != list.end();
}

bool isSystemIndent(std::string indent) { // check if string has __ at the beginning and at the end.
    bool result =
            indent.at(0) == '_' &&
            indent.at(1) == '_' &&
            indent.at(indent.size() - 1) == '_' &&
            indent.at(indent.size() - 2) == '_';

    return result;
}

unsigned long find(std::vector<std::string> &source, const std::string &target) { // find string in vector of strings
    return std::find(source.begin(), source.end(), target) - source.begin();
}

unsigned long find(const std::string &source, char target) { // find character in string
    return source.find(target);
}
