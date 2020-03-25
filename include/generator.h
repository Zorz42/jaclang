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
    void main();

    namespace e {
        void systemFunctionCall();

        void functionDeclaration();

        void variableDeclaration(unsigned long scopeOnStack);

        std::string calculation(branch &calculation);

        function *functionCall(const std::string &functionName);

        void variableSetting();

        void returnStatement();
    }

    inline int stackPointer = 0;
    inline int biggestStackPointer = 0;

    void incStackPointer(int value);

    void decStackPointer(int value);

    inline std::vector<variable> stack;
    inline std::vector<function> functionVector;

    void pushToStack(variable source);

    inline int8_t currentRegister = 0;
    inline std::vector<std::string> availableRegisters[4];

    void nextRegister();

    void prevRegister();

    std::string availableRegister(int8_t size, int8_t offset = 0);

    inline std::unordered_map<int8_t, std::string> sizeKeywords;

    variable get_variable(const std::string &name);

    inline std::unordered_map<std::string, int> primitiveDatatypeSizes;
    inline std::vector<datatypeMatches> operatorMatches;
    inline std::vector<std::string> primitiveDatatypes;
    inline std::unordered_map<std::string, std::vector<std::string>> implicitConversations;

    inline function *currentFunction = nullptr;

    void checkForImplicitConversion(const std::string &dest, const std::string &source);
}
