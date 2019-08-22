// this generates ast into asm code

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
			generator::main();
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