#pragma once

struct token
{
	int8_t type;
	std::string text;
	int line;
	int pos;
	
	token(int TYPE , std::string TEXT) : type(TYPE), text(TEXT) {}
	token(std::string TEXT) : type(TYPE_UNDEF), text(TEXT) {}
	token() : type(0), text("") {}
};

namespace lexer
{
	void main();
	
	extern std::vector<token> toks;

	const std::string symbols = "()[]{},;*&#.";
	const std::string operators = "!%^=><+-*%&|/~?:$@";
	extern std::vector<std::string> keywords;
	const std::string inNumber = "0123456789.";
}