#include "jaclang.h"

// This file prints out abstract syntax tree

int nested = -1;

void printAST(branch obj)
{
	nested++;
	std::string name;
	for(int i = 0; i < nested; i++)
		name += "	";
	name += obj.name;
	std::cout << name << std::endl;
	for(branch iterator : obj.sub)
	{
		printAST(iterator);
	}
	nested--;
}
