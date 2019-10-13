// there are defined functions, that are often used to make code more readable

#include "jaclang.h"

bool contains(std::string text, char character) // check if string contains character
{
	return text.find(character) != std::string::npos;
}

bool contains(std::vector<std::string>& list, std::string text) // check if vector of strings contains string
{
	return find(list, text) != list.size();
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

std::string onStack(int offset) // output asm value of offset on stack example: offset = 4 -> [rbp-4]
{
	return "[rbp-" + std::to_string(offset) + "]"; // returns position on stack: [rbp-offset]
}

int find(std::vector<std::string>& source, std::string target) // find string in vector of strings
{
	for(int i = 0; i < source.size(); i++)
		if(source.at(i) == target)
			return i;
	return source.size();
}

int find(std::string source, char target) // find character in string
{
	for(int i = 0; i < source.size(); i++)
		if(source.at(i) == target)
			return i;
	return source.size();
}