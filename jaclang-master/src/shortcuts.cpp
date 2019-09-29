// there are defined functions, that are often used to make code more readable

#include "jaclang.h"

bool contains(std::string text, char character) 
{
	return text.find(character) != std::string::npos;
}

bool contains(std::vector<std::string>& list, std::string text)
{
	return find(list, text) != list.size();
}

bool contains(std::vector<char>& list, char text)
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

std::string onStack(int offset)
{
	return "[rbp-" + std::to_string(offset) + "]"; // returns position on stack: [rbp-offset]
}

int find(std::vector<std::string>& source, std::string target)
{
	for(int i = 0; i < source.size(); i++)
		if(source.at(i) == target)
			return i;
	return source.size();
}

int find(std::string source, char target)
{
	for(int i = 0; i < source.size(); i++)
		if(source.at(i) == target)
			return i;
	return source.size();
}

int find(std::vector<char>& source, char& target)
{
	for(int i = 0; i < source.size(); i++)
		if(source.at(i) == target)
			return i;
	return source.size();
}