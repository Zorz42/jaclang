#pragma once

enum tokenType {tt_undefined, tt_indent, tt_constant, tt_string, tt_symbol, tt_operator, tt_keyword};

struct token {
    tokenType type = tt_undefined;
    std::string text;
    int line{};
    unsigned long pos{};

    token(tokenType TYPE, std::string TEXT) : type(TYPE), text(std::move(TEXT)) {}

    /*explicit token(std::string TEXT) : type(tt_undefined), text(std::move(TEXT)) {}*/

    token() = default;
};

namespace lexer {
    void main();

    inline std::vector<std::string> keywords;
    inline std::list<token> tokens;
}
