#pragma once

struct branch
{
	std::string name;
	std::vector<branch> sub;
	int count = 0;
};

extern branch mainBranch;
extern branch* currentBranchScope;

void appendBranch(branch& source, branch& target);
void appendBranch(std::string source, branch& target);

namespace parser
{
	void main(std::vector<std::string> args);
	branch equation(bool nested=false);
	
	extern int tokCount;
	
	namespace e 
	{
		bool functionCall(branch& target);
		bool systemFunctionCall();
		bool variableDeclaration();
		bool beginScope();
		bool endScope();
		bool functionDeclaration();
	}
	
	extern std::vector<branch*> scopes;
}