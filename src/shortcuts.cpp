#include "jaclang.h"

bool contains(std::string text, char character) 
{
	return text.find(character) != std::string::npos;
}

bool contains(std::vector<std::string> list, std::string text)
{
	return std::find(std::begin(list), std::end(list), text) != std::end(list);
}

bool isSystemIndent(std::string indent) // check if string has __ at the beggining and at the end. 
{
	bool result =
	indent.at(0) == '_' &&
	indent.at(1) == '_' &&
	indent.at(indent.size() - 1) == '_' &&
	indent.at(indent.size() - 2) == '_';
	
	return result;
}

std::string onStack(int offset)
{
	return "[rbp-" + std::to_string(offset) + "]"; // returns position on stack: [rbp-offset]
}