struct variable
{
	std::string indent; // indent
	int type; // type of value
	int position; // position on stack
};

namespace generator
{
	void main();
	
	namespace e
	{
		void systemFunctionCall();
		void variableDeclaration();
		void equation(branch equation);

	}
	
	extern int stackPointer;
	extern std::vector<variable> stack;
	
	void pushToStack(variable source);

	extern int currentRegister32;
	extern std::vector<std::string> availableRegisters32;

	void nextRegister();
	std::string availableRegister32();
}
