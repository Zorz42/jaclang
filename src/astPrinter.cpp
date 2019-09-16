// this file prints out abstract syntax tree

#define LIB_IOSTREAM
#define LIB_PARSER

#include "jaclang.h"

int nested = -1;

void printAST(branch& obj)
{
	nested++;
	std::string name;
	for(int i = 0; i < nested; i++)
		name += "	";
	name += obj.name;
	coutd << name << std::endl;
	for(branch iterator : obj.sub)
	{
		printAST(iterator);
	}
	nested--;
}