#pragma once

bool contains(std::vector<std::string> &list, const std::string &text);

bool isSystemIndent(std::string indent);

std::string onStack(int offset);

void printAST(branch &obj);

bool isInt(const std::string &text);


unsigned long find(std::vector<std::string> &source, const std::string &target);

unsigned long find(const std::string &source, char target);
