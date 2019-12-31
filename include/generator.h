#pragma once

#include "jaclang.h"

struct variable
{
    std::string name; // name
    std::string type;   // datatype
    int position; // position on stack
    int8_t size();
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
		std::string calculation(branch& calculation);
        void functionCall(const std::string& variableName);
        void variableSetting();
        void returnStatement();
    }
	
	extern int stackPointer;
	extern std::vector<variable> stack;
	extern std::vector<function> functionVector;
	
	void pushToStack(variable source);

	extern unsigned int currentRegister;
	extern std::vector<std::string> availableRegisters[4];

	void nextRegister();
    void prevRegister();
	std::string availableRegister(int8_t size, int8_t offset=0);

    extern std::unordered_map<int8_t, std::string> sizeKeywords;

    variable get_variable(const std::string& name);

    extern std::unordered_map<std::string, int> primitiveDatatypeSizes;
    extern std::unordered_map<std::string, std::string> operatorMatches;
    extern std::vector<std::string> primitiveDatatypes;
}
