// this generates ast into asm code

#include "jaclang.h"

int generator::stackPointer = 0; // top of stack
std::vector<variable> generator::stack; // the stack, not actual just for allocation
std::vector<function> generator::functionVector;
bool generator::inFunction = false;

std::unordered_map<std::string, int> generator::primitiveDatatypeSizes;

unsigned long currentScopeOnStack = 0;

std::unordered_map<int8_t, std::string> generator::sizeKeywords;

void generator::main()
{
	#define current currentBranchScope->sub.at(currentBranchScope->count) // current branch
    if(currentBranchScope == &mainBranch)
		file::append_text("   mov rbp, rsp");
	for(; currentBranchScope->count < currentBranchScope->sub.size(); currentBranchScope->count++) // iterate though branches
    {
		if(!file::outputVector.at(file::asm_text - 1).empty())
			file::append_text("");
		if(current.name == "systemFunctionCall")  // choose appropriate generator for branch
			generator::e::systemFunctionCall();
		else if(current.name == "variableDeclaration")
			generator::e::variableDeclaration(currentScopeOnStack);
		else if(current.name == "calc")
			generator::e::calculation(current);
		else if(current.name == "scope") // if branch is scope
        {
			branch* prevScope = currentBranchScope; // save current scope
			currentBranchScope = &(current);        // move to new scope
			unsigned long stackLength = generator::stack.size();  // save stack length
			unsigned long prevScopeOnStack = currentScopeOnStack; // save scope on stack
			currentScopeOnStack = stackLength;  // set scope on stack
			generator::main();
			currentScopeOnStack = prevScopeOnStack; // retrieve scope on stack
			while(generator::stack.size() > stackLength) // remove elements from stack that were in scope
			{
				stackPointer -= generator::stack.at(generator::stack.size() - 1).size();
				generator::stack.pop_back();
			}
			currentBranchScope = prevScope; // retrieve current branch scope
		}
		else if(current.name == "functionDeclaration" && !generator::inFunction)
			generator::e::functionDeclaration();
		else if(current.name == "variableSetting")
            generator::e::variableSetting();
		else if(current.name == "returnStatement" && generator::inFunction)
            generator::e::returnStatement();
		else
			error::treeError("Unknown branch: " + current.name);
    }
}

void generator::pushToStack(variable source) // push to stack
{
	generator::stackPointer += source.size();
	source.position = generator::stackPointer;
	generator::stack.push_back(source);
}

void generator::nextRegister()
{
    generator::currentRegister++;
    if(generator::currentRegister == generator::availableRegisters[2].size())
        error::treeError("register overflow");
}

void generator::prevRegister()
{
    generator::currentRegister--;
    if(generator::currentRegister == -1)
        error::treeError("register overflow");
}

std::string generator::availableRegister(int8_t size)
{
    int index = 0;
    for (int i = size; i != 1; i /= 2)
        index++;
    return generator::availableRegisters[index].at(generator::currentRegister);
}

int8_t variable::size()
{
    return generator::primitiveDatatypeSizes[this->type];
}
