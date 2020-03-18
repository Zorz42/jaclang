#pragma once

struct token {
    int type = 0;
    std::string text;
    int line{};
    unsigned long pos{};

    token(int8_t TYPE, std::string TEXT) : type(TYPE), text(std::move(TEXT)) {}

    explicit token(std::string TEXT) : type(TYPE_UNDEF), text(std::move(TEXT)) {}

    token() = default;
};

namespace lexer {
    void main();

    extern std::vector<std::string> keywords;
    extern std::list<token> tokens;
}
