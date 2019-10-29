// this file prints out abstract syntax tree

#include "jaclang.h"

int nested = -1;

void printAST(branch& obj) // print abstract syntax tree
{
	nested++; // how deep is the program in ast
	std::string name;
	for(int i = 0; i < nested; i++) // print tabs for deep is it
		name += "	";
	name += obj.name;
	coutd << name << std::endl;
	for(branch iterator : obj.sub) // if there is another branch, print it
		printAST(iterator);
	nested--;
}