// this generates variable declarations

#include "jaclang.h"

void generator::e::variableDeclaration()
{
	#define current mainBranch.sub.at(mainBranch.count)
	
	variable obj; // obj variable
	obj.indent = current.sub.at(0).name; // indent
	obj.type = VARIABLE_INT; // type
	
	for(variable iter : generator::stack) // go through stack
		if(iter.indent == current.sub.at(0).name) // if this variable already exists, then report error
			error::treeError("Declaration of already existing variable");
	
	
	generator::stackPointer += 4; // shift by 4 (size of int -> 4bytes)
	obj.position = generator::stackPointer; // set position
	generator::stack.push_back(obj); // push to stack
	
	if(current.sub.at(1).name == "/equation")
	{
		if(current.sub.at(1).sub.at(0).name != "int") // if equation type is not int report error
			error::treeError("Int declaration must be type int");
		
		generator::e::equation(current.sub.at(1)); // do equation
		
		file::append_instruction("mov", "DWORD " + onStack(generator::stackPointer), generator::availableRegister32()); // set variable on stack
	}
	else
	{
		if(!isInt(current.sub.at(1).name))
			error::treeError("Int declaration must be type int");
		
		file::append_instruction("mov", "DWORD " + onStack(generator::stackPointer), current.sub.at(1).name);
	}
}