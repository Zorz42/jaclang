#pragma once

struct branch
{
    std::string name;
    std::vector<branch> sub;
    unsigned int count = 0;
};

extern branch mainBranch;
extern branch* currentBranchScope;

void appendBranch(const branch& source, branch& target);
void appendBranch(std::string source, branch& target);

namespace parser
{
	void main(std::string rootName);
	branch calculation(bool nested=false);
	
	extern std::list<token>::iterator currToken;
    std::list<token>::iterator peekNextToken();
    extern unsigned long tokCount;
    extern bool breakLoop;

    std::list<token>::iterator nextToken();
    std::list<token>::iterator prevToken();


    namespace e
	{
		bool functionCall(branch& target);
		bool systemFunctionCall();
		bool variableDeclaration();
		bool beginScope();
		bool endScope();
		bool functionDeclaration();
        bool variableSetting();
        bool returnStatement();
	}
	
	extern std::vector<branch*> scopes;
}
