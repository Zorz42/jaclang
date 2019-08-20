// this file prints out abstract syntax tree

#include "jaclang.h"

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