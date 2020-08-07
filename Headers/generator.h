#define PUSHA_SIZE 112

struct Variable {
    std::string name; // name
    std::string type;   // datatype
    long position; // position on stack
    
    int8_t size() const;
    std::string generateAddress() const;
};

struct Function {
    std::string name;
    std::string type;
    std::vector<Variable> args;
    
    bool defined;
    
    int size() const;
    std::string generateName() const;
};

struct Match {
    std::string type;
    std::string result;

    explicit Match(std::vector<std::string> input) : type(input.at(0)), result(input.at(1)) {};
};

struct DatatypeMatches {
    std::string datatype;
    std::vector<Match> matches;

    DatatypeMatches(std::string datatype_, std::vector<Match> matches_) : datatype(std::move(datatype_)),
                                                                        matches(std::move(matches_)) {};
    DatatypeMatches() = default;
};

namespace generator {
    void main(bool inFunction=false);

    namespace e {
        void systemFunctionCall();
        void functionDeclaration();
        void variableDeclaration();
        std::string expr(Branch &calculation);
        Function *functionCall(const Branch &function_branch);
        void variableSetting();
        void returnStatement();
        void ifStatement();
        void scope();
        void whileStatement();
        void globalVariableDeclaration();
        void localVariableDeclaration();
    }

    inline std::vector<Function> function_vector;

    Variable *getVariable(const std::string &name);
    Function *getFunction(const std::string &name, const std::vector<std::string> &args);

    inline std::vector<std::string> primitive_datatypes;
    inline std::unordered_map<std::string, int> primitive_datatype_sizes;
    inline std::vector<DatatypeMatches> operator_matches;
    inline std::unordered_map<std::string, std::vector<std::string>> implicit_conversations;

    void checkForImplicitConversion(const std::string &dest, const std::string &source);

    inline Function *current_function = nullptr;
    inline Branch *current_branch_scope;
    inline unsigned long current_branch_scope_count;
}
