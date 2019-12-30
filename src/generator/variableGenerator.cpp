// this generates variable declarations

#include "jaclang.h"

#define current currentBranchScope->sub.at(currentBranchScope->count)

void generator::e::variableDeclaration(unsigned long scopeOnStack)
{
	variable obj; // obj variable
    obj.type = current.sub.at(0).name;   // datatype
	obj.indent = current.sub.at(1).name; // indent
	
	unsigned int i = 0;
	
	for(const variable& iter : generator::stack) // go through stack
	{
		if(iter.indent == current.sub.at(1).name && i >= scopeOnStack) // if this variable already exists, then report error
        {
            std::string errorString = iter.indent;
            errorString += " already exists as a variable";
            error::treeError(errorString);
        }
		i++;
	}
	
	generator::pushToStack(obj); // push to stack
	
	if(current.sub.at(2).name == "calc")
	{
		if(current.sub.at(2).sub.at(0).name != "int") // if calculation type is not int report error
			error::treeError("int declaration must be type int");
		
		generator::e::calculation(current.sub.at(2)); // do calculation
		
		file::append_instruction("mov", generator::sizeKeywords[obj.size()] + " " + onStack(generator::stackPointer), generator::availableRegister(obj.size())); // set variable on stack
	}
	else
	{
		if(!isInt(current.sub.at(2).name))
			error::treeError("int declaration must be type int");
		
		file::append_instruction("mov", generator::sizeKeywords[obj.size()] + " " + onStack(generator::stackPointer), current.sub.at(2).name);
	}
}

void generator::e::variableSetting()
{
    bool variableExists = false;
    variable currentVariable;
    
    for(const variable& iter : generator::stack) // go through stack
        if(iter.indent == current.sub.at(0).name)
        {
            variableExists = true;
            currentVariable = iter;
            break;
        }
    if(!variableExists)
    {
        std::string errorString = current.sub.at(0).name;
        errorString += " does not exist as a variable";
        error::treeError(errorString);
    }
    
    if(current.sub.at(1).name == "calc")
    {
        if(current.sub.at(1).sub.at(0).name != "int") // if calculation type is not int report error
            error::treeError("int declaration must be type int");
        
        generator::e::calculation(current.sub.at(1)); // do calculation
        
        file::append_instruction("mov", generator::sizeKeywords[currentVariable.size()] + " " + onStack(currentVariable.position), generator::availableRegister(currentVariable.size())); // set variable on stack
    }
    else
    {
        if(!isInt(current.sub.at(1).name))
            error::treeError("int declaration must be type int");
        
        file::append_instruction("mov", generator::sizeKeywords[currentVariable.size()] + " " + onStack(currentVariable.position), current.sub.at(1).name);
    }
}
