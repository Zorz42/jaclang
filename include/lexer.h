#pragma once

enum TokenType {Undefined, Indent, Constant, String, Symbol, Operator, Keyword};

struct Token {
    TokenType type = Undefined;
    std::string text;
    int line{};
    unsigned long pos{};

    Token(TokenType type, std::string text) : type(type), text(std::move(text)) {}

    Token() = default;
};

namespace lexer {
    void main();

    inline std::vector<std::string> keywords;
    inline std::list<Token> tokens;
}
