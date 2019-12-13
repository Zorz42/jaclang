#pragma once

struct variable
{
    std::string indent; // indent
    //int8_t type; // type of value
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
		void variableDeclaration(unsigned long scopeOnStack);
		void calculation(branch& calculation);
        void functionCall(const std::string& variableName);
        void variableSetting();
        void returnStatement();
    }
	
	extern int stackPointer;
	extern std::vector<variable> stack;
	extern std::vector<function> functionVector;
	
	void pushToStack(variable source);

	extern unsigned int currentRegister32;
	extern std::vector<std::string> availableRegisters32;

	void nextRegister();
    void prevRegister();
	std::string availableRegister32();
}
