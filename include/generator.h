#pragma once

struct variable
{
    std::string indent; // indent
    std::string type;   // datatype
    int position; // position on stack
    int8_t size()
    {
        return parser::primitiveDatatypeSizes[this->type];
    }
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

	extern unsigned int currentRegister;
	extern std::vector<std::string> availableRegisters[4];

	void nextRegister();
    void prevRegister();
	std::string availableRegister(int8_t size);

    extern std::unordered_map<int8_t, std::string> sizeKeywords;

    variable get_variable(const std::string& name);
}
