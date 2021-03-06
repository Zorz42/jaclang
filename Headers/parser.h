struct Branch {
    std::string name;
    std::vector<Branch> sub;
    
    Branch() = default;
    Branch(std::string name_) : name(name_) {}
};

namespace parser {
    void main();

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
        bool localVariableDeclaration();
    }

    inline std::stack<Branch*> scopes;
}
