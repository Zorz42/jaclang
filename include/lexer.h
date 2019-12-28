#pragma once

struct token
{
    int8_t type;
    std::string text;
    int line;
    unsigned long pos;
    
    token(int8_t TYPE , std::string TEXT) : type(TYPE), text(std::move(TEXT)) {}
    token(std::string TEXT) : type(TYPE_UNDEF), text(std::move(TEXT)) {}
    token() : type(0), text("") {}
};

namespace lexer
{
	void main();
	
	extern std::vector<token> tokens;

	const std::string symbols = "()[]{},*&#.";
	const std::string operators = "!%^=><+-*%&|/~?:$@";
	const std::string inNumber = "0123456789.";
}
