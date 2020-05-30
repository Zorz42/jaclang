#pragma once

struct variable {
    std::string name; // name
    std::string type;   // datatype
    int position; // position on stack
    
    int8_t size();
};

struct function {
    std::string name;
    std::string type;

    int8_t size() const;
};

struct match {
    std::string type;
    std::string result;

    explicit match(std::vector<std::string> input) : type(input.at(0)), result(input.at(1)) {};
};

struct datatypeMatches {
    std::string datatype;
    std::vector<match> matches;

    datatypeMatches(std::string datatype, std::vector<match> matches) : datatype(std::move(datatype)),
                                                                        matches(std::move(matches)) {};

    datatypeMatches() = default;
};

namespace generator {
    void main(bool inFunction=false);

    namespace e {
        void systemFunctionCall();

        void functionDeclaration();

        void variableDeclaration(unsigned long scopeOnStack);

        std::string calc(branch &calculation);

        function *functionCall(const std::string &functionName);

        void variableSetting();

        void returnStatement();
    
        void ifStatement();
        
        void scope();
    
        void whileStatement();
    }

    inline std::vector<function> functionVector;

    inline std::unordered_map<int8_t, std::string> sizeKeywords;

    variable get_variable(const std::string &name);

    inline std::vector<std::string> primitiveDatatypes;
    inline std::unordered_map<std::string, int> primitiveDatatypeSizes;
    inline std::vector<datatypeMatches> operatorMatches;
    inline std::unordered_map<std::string, std::vector<std::string>> implicitConversations;

    void checkForImplicitConversion(const std::string &dest, const std::string &source);

    inline function *currentFunction = nullptr;
}
