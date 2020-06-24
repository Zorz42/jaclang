#pragma once

enum TokenType {Undefined, Indent, Constant, String, Symbol, Operator, Keyword};

struct Token {
    TokenType type = Undefined;
    std::string text;
    unsigned int line{};
    unsigned int pos{};

    Token(TokenType type_, std::string text_) : type(type_), text(std::move(text_)) {}

    Token() = default;
};

namespace lexer {
    void main();

    inline bool debug_show_tokens;

    inline std::vector<std::string> keywords;
    inline std::list<Token> tokens;
}
