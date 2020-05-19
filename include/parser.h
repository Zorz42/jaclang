#pragma once

struct branch {
    std::string name;
    std::vector<branch> *sub = nullptr;
    unsigned int count = 0;

    branch() = default;

    branch(const branch &input) {
        if (input.sub != nullptr) {
            sub = new std::vector<branch>;
            *sub = *input.sub;
        }
        name = input.name;
        count = input.count;
    }

    void alloc() {
        sub = new std::vector<branch>;
    }

    ~branch() {
        delete sub;
        sub = nullptr;
    }
};

inline branch mainBranch;
inline branch *currentBranchScope;

void appendBranch(const branch &source, branch &target);

void appendBranch(std::string source, branch &target);

namespace parser {
    void main(std::string rootName);

    branch calculation(bool nested = false);

    inline std::list<token>::iterator currToken;

    std::list<token>::iterator peekNextToken();

    inline unsigned long tokCount;
    inline bool breakLoop = false;

    std::list<token>::iterator nextToken();

    std::list<token>::iterator prevToken();


    namespace e {
        bool functionCall(branch &target);

        bool systemFunctionCall();

        bool variableDeclaration();

        bool beginScope();

        bool endScope();

        bool functionDeclaration();

        bool variableSetting();

        bool returnStatement();
    
        bool ifStatement();
    }

    inline std::vector<branch *> scopes;
}
