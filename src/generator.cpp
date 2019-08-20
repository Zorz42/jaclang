// this generates ast into asm code

#include <jaclang.h>

int generator::stackPointer = 0; // top of stack
std::vector<variable> generator::stack; // the stack, not actual just for allocation

void generator::main()
{
	#define current mainBranch.sub.at(mainBranch.count) // current branch
	for(; mainBranch.count < mainBranch.sub.size(); mainBranch.count++) // iterate though branches
	{
		file::append_text("");
		std::cout << current.name << std::endl;
		if(current.name == "systemFunctionCall")  // choose apropriate generator for branch
			generator::e::systemFunctionCall();
		else if(current.name == "variableDeclaration")
			generator::e::variableDeclaration();
		else if(current.name == "/equation")
			generator::e::equation(current);
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