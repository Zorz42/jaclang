#pragma once

struct variable
{
	std::string indent; // indent
	int8_t type; // type of value
	int position; // position on stack
	int8_t size;
};

struct function
{
	std::string name;
};

namespace generator
{
	void main();
	extern bool inFunction;
	namespace e
	{
		void systemFunctionCall();
		void functionDeclaration();
		void variableDeclaration(int scopeOnStack);
		void equation(branch& equation);
	}
	
	extern int stackPointer;
	extern std::vector<variable> stack;
	extern std::vector<function> functionVector;
	
	void pushToStack(variable source);

	extern int currentRegister32;
	extern std::vector<std::string> availableRegisters32;

	void nextRegister();
	std::string availableRegister32();
}
