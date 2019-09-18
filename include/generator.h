#pragma once

struct variable
{
	std::string indent; // indent
	int8_t type; // type of value
	int position; // position on stack
	int8_t size;
};

namespace generator
{
	void main();
	
	namespace e
	{
		void systemFunctionCall();
		void variableDeclaration(int scopeOnStack);
		void equation(branch& equation);
	}
	
	extern int stackPointer;
	extern std::vector<variable> stack;
	
	void pushToStack(variable source);

	extern int currentRegister32;
	extern std::vector<std::string> availableRegisters32;

	void nextRegister();
	std::string availableRegister32();
}
