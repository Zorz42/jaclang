#pragma once

struct Branch {
    std::string name;
    std::vector<Branch> *sub = nullptr;
    unsigned int count = 0;

    Branch() = default;
    Branch(const Branch &input);
    void alloc();
    ~Branch();
};

namespace parser {
    void main(std::string root_name);

    Branch expr(bool nested=false);

    inline unsigned long tok_count;

    std::list<Token>::iterator nextToken();
    std::list<Token>::iterator prevToken();
    std::list<Token>::iterator peekNextToken();
    inline std::list<Token>::iterator curr_token;

    inline Branch main_branch;
    inline Branch *current_branch_scope;

    void appendBranch(const Branch &source, Branch &target);

    void appendBranch(std::string source, Branch &target);


    namespace e {
        bool functionCall(Branch &target);
        bool systemFunctionCall();
        bool variableDeclaration();
        bool beginScope();
        bool endScope();
        bool functionDeclaration();
        bool variableSetting();
        bool returnStatement();
        bool ifStatement();
        bool whileStatement();
    }

    inline std::vector<Branch*> scopes;
}
