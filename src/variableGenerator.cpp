// this generates variable declarations

#include "jaclang.h"

void generator::e::variableDeclaration(unsigned long scopeOnStack)
{
	#define current currentBranchScope->sub.at(currentBranchScope->count)
	
	variable obj; // obj variable
	obj.indent = current.sub.at(0).name; // indent
	obj.type = VARIABLE_INT; // type
	obj.size = 4;
	
	int i = 0;
	
	for(variable iter : generator::stack) // go through stack
	{
		if(iter.indent == current.sub.at(0).name && i >= scopeOnStack) // if this variable already exists, then report error
			error::treeError("Declaration of already existing variable");
		i++;
	}
	
	generator::pushToStack(obj); // push to stack
	
	if(current.sub.at(1).name == "calc")
	{
		if(current.sub.at(1).sub.at(0).name != "int") // if calculation type is not int report error
			error::treeError("Int declaration must be type int");
		
		generator::e::calculation(current.sub.at(1)); // do calculation
		
		file::append_instruction("mov", "DWORD " + onStack(generator::stackPointer), generator::availableRegister32()); // set variable on stack
	}
	else
	{
		if(!isInt(current.sub.at(1).name))
			error::treeError("Int declaration must be type int");
		
		file::append_instruction("mov", "DWORD " + onStack(generator::stackPointer), current.sub.at(1).name);
	}
}
