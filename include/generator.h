#pragma once

#define PUSHA_SIZE 112

struct Variable {
    std::string name; // name
    std::string type;   // datatype
    int position; // position on stack
    bool arg;
    
    int8_t size() const;
    std::string generateAddress() const;
};

struct Function {
    std::string name;
    std::string type;
    std::vector<Variable> args;

    int8_t size() const;
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

    DatatypeMatches(std::string datatype, std::vector<Match> matches) : datatype(std::move(datatype)),
                                                                        matches(std::move(matches)) {};

    DatatypeMatches() = default;
};

namespace generator {
    void main(bool inFunction=false);

    namespace e {
        void systemFunctionCall();

        void functionDeclaration();

        void variableDeclaration(unsigned long scope_on_stack);

        std::string expr(Branch &calculation);

        Function *functionCall(const Branch &function_branch);

        void variableSetting();

        void returnStatement();
    
        void ifStatement();
        
        void scope();
    
        void whileStatement();
    }

    inline std::vector<Function> function_vector;

    inline std::unordered_map<int8_t, std::string> size_keywords;

    Variable *getVariable(const std::string &name);
    Function *getFunction(const std::string &name, const std::vector<std::string> &args);

    inline std::vector<std::string> primitive_datatypes;
    inline std::unordered_map<std::string, int> primitive_datatype_sizes;
    inline std::vector<DatatypeMatches> operator_matches;
    inline std::unordered_map<std::string, std::vector<std::string>> implicit_conversations;

    void checkForImplicitConversion(const std::string &dest, const std::string &source);

    inline Function *current_function = nullptr;
}
