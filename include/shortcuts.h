#pragma once

bool contains(const std::string& text, char character);
//bool contains(std::vector<std::string>& list, const std::string& text);
//bool contains(std::vector<char>& list, char text);
bool isSystemIndent(std::string indent);

std::string onStack(int offset);

void printAST(branch& obj);

//unsigned long toDec(std::string text);
bool isInt(const std::string& text);
bool isDec(const std::string& text);

unsigned long find(std::vector<std::string>& source, const std::string& target);
unsigned long find(const std::string& source, char target);
