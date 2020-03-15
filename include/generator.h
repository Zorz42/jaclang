#pragma once

#include "jaclang.h"

struct variable {
    std::string name; // name
    std::string type;   // datatype
    int position; // position on stack
    int8_t size();
};

struct function {
    std::string name;
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
    void main();

    extern bool inFunction;
    namespace e {
        void systemFunctionCall();

        void functionDeclaration();

        void variableDeclaration(unsigned long scopeOnStack);

        std::string calculation(branch &calculation);

        void functionCall(const std::string &variableName);

        void variableSetting();

        void returnStatement();
    }

    extern int stackPointer;
    extern std::vector<variable> stack;
    extern std::vector<function> functionVector;

    void pushToStack(variable source);

    extern int8_t currentRegister;
    extern std::vector<std::string> availableRegisters[4];

    void nextRegister();

    void prevRegister();

    std::string availableRegister(int8_t size, int8_t offset = 0);

    extern std::unordered_map<int8_t, std::string> sizeKeywords;

    variable get_variable(const std::string &name);

    extern std::unordered_map<std::string, int> primitiveDatatypeSizes;
    extern std::vector<datatypeMatches> operatorMatches;
    extern std::vector<std::string> primitiveDatatypes;
    extern std::unordered_map<std::string, std::vector<std::string>> implicitConversations;
}
