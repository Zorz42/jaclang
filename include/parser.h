#pragma once

struct Branch {
public:
    std::string name;
    std::vector<Branch> sub;
};

namespace parser {
    void main(std::string root_name);

    bool isSystemIndent(std::string indent);

    Branch expr(bool optional_existence=false);

    inline bool debug_show_ast;

    std::list<Token>::iterator nextToken();
    std::list<Token>::iterator prevToken();
    std::list<Token>::iterator peekNextToken();
    inline std::list<Token>::iterator curr_token;

    inline Branch main_branch;
    inline Branch *current_branch_scope;
    inline unsigned long current_branch_scope_count;

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
        bool globalVariableDeclaration();
    }

    inline std::vector<Branch*> scopes;
}
