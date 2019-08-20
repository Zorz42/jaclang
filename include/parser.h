struct branch
{
	std::string name;
	std::vector<branch> sub;
	int count = 0;
};

extern branch mainBranch;

void appendBranch(branch& source, branch& target);
void appendBranch(std::string source, branch& target);

namespace parser
{
	void main();
	branch equation(std::string end, std::string end2="");
	
	extern int tokCount;
	
	namespace e 
	{
		bool functionCall();
		bool systemFunctionCall();
		bool variableDeclaration();
	}
}