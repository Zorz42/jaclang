#pragma once

bool contains(std::string text, char character);
bool contains(std::vector<std::string>& list, std::string text);
bool contains(std::vector<char>& list, char text);
bool isSystemIndent(std::string indent);

std::string onStack(int offset);

void printAST(branch& obj);

double toDec(std::string text);
bool isInt(std::string text);
bool isDec(std::string text);

int find(std::vector<std::string>& source, std::string target);
int find(std::string source, char target);