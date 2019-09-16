// this generates ast into asm code

#define LIB_FILE
#define LIB_ERROR
#define LIB_PARSER
#define LIB_GENERATOR

#include "jaclang.h"

int generator::stackPointer = 0; // top of stack
std::vector<variable> generator::stack; // the stack, not actual just for allocation

void generator::main()
{
	#define current currentBranchScope->sub.at(currentBranchScope->count) // current branch
	if(currentBranchScope == &mainBranch)
		file::append_text("	mov rbp, rsp");
	for(; currentBranchScope->count < currentBranchScope->sub.size(); currentBranchScope->count++) // iterate though branches
	{
		file::append_text("");
		if(current.name == "systemFunctionCall")  // choose apropriate generator for branch
			generator::e::systemFunctionCall();
		else if(current.name == "variableDeclaration")
			generator::e::variableDeclaration();
		else if(current.name == "/equation")
			generator::e::equation(current);
		else if(current.name == "scope")
		{
			branch* prevScope = currentBranchScope;
			currentBranchScope = &(current);
			int stackLength = generator::stack.size();
			generator::main();
			while(generator::stack.size() > stackLength)
				generator::stack.pop_back();
			currentBranchScope = prevScope;
		}
		else
			error::treeError("Unknown branch: " + current.name);
	}
}

void generator::pushToStack(variable source) // push to stack
{
	generator::stackPointer += 4;
	source.position = generator::stackPointer;
	generator::stack.push_back(source);
}