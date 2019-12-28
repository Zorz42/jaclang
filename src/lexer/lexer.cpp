#include "jaclang.h"

/* this is the LEXER which breaks code into lexer::tokens aka more readable code.
 * eg.
 * 
 * int i = 0;
 * token 1: type: indent, text: "i"
 * token 2: type: symbol, text: "="
 * token 3: type: constant, text: "0"
 * token 4: type: symbol, text: ";"
 * 
 * it breaks code so that parser can read it and turn it into syntax tree
 * */

#define IN_STRING (inStringQ || inStringDQ) // macro for later purposes

int currentLine = 1;

std::vector<token> lexer::tokens; // vector of tokens

void newToken(int TYPE=TYPE_UNDEF); // pushes current token to token vector and sets it to empty string
std::string currentToken; // current token in processing

//unsigned long toDec(std::string text); // convert string to float or int

unsigned long prevC = 0;
unsigned long c = 0;

void lexer::main() // main lexer function
{
	bool inStringQ = false;  // if in string with single quotes
	bool inStringDQ = false; // if in string with double quotes
	
	#define CHAR file::inputText.at(c) // shortcut to get current character of string that for loop is parsing
	#define nextChar c++
	#define prevChar c--
	
	for(; c < file::inputText.length(); c++) // the for loop parses through input file
	{
		if((CHAR == ' ' || CHAR == '	' || CHAR == '\n') && !IN_STRING) // tabs and spaces separate tokens if not in string
		{
			newToken();
			if(CHAR == '	')
				prevC++;
		}
		else if(CHAR == '"' && !inStringQ) // if double quotes occur and if not in single quoted string
		{
			inStringDQ = !inStringDQ; // change to true if false and vice versa
			if(!inStringDQ) // if just got out of string
				newToken(TYPE_STRING); // then this token is string
			else
				newToken(); // if coming to string end the token and start string token
		}
		else if(CHAR == '\'' && !inStringDQ) // the same thing but for single quotes
		{
			inStringQ = !inStringQ;
			if(!inStringQ)
				newToken(TYPE_STRING);
			else
				newToken();
		}
		else if(contains(lexer::symbols, CHAR) && !IN_STRING) // if character is symbol (and not in string)
		{
			bool special = false;
			const unsigned long OPERATOR = lexer::operators.find(CHAR); // OPERATOR indicates position in operators
			if(CHAR == '.' && isInt(currentToken))
			{
				currentToken += CHAR;
				special = true;
			}
			else // '.' character is used also in decimals and should be interpreted as part of constant
			{ // if current token was not integer then its not decimal point
				newToken(); // it is symbol and cut the previous token
				currentToken = CHAR; // set token to symbols since all (except one) are single character
			} // if not just append it to the token since its constant
			if(CHAR == '*' || CHAR == '&') // symbols and operators share two symbols '*' and '&'
			{
				nextChar;
				if(CHAR == lexer::operators[OPERATOR] || CHAR == '=') // if its "**", "*=", "&&", "&="
				{
					currentToken += CHAR; // then it is operator
					newToken(TYPE_OPERATOR);
				}
				else
				{
					prevChar; // if not decrement back and make new token to be determined later
					newToken(); // because '*' or '&' can be operator or symbol based on the neighbor token
				}
			}
			else if(!special)
			{
				newToken(TYPE_SYMBOL); // if its nothing special just append it to tokens
			}
		}
		else if(contains(lexer::operators, CHAR) && !IN_STRING) // operators are the same, but multi-char
		{
			const unsigned long OPERATOR = lexer::operators.find(CHAR); // finds in operators
			newToken(); // dumps the token
			currentToken = CHAR; // sets token to that operator
			if(CHAR == ':' && file::inputText.at(c + 1) == ':') // there is operator ':' and symbol '::'
			{
				nextChar; // increment
				currentToken += ':'; // add ':' to token, so its '::'
				newToken(TYPE_SYMBOL); // its symbol
				// if its ':' it will be handled later
			}
			else if(OPERATOR < 13) // if operator is not in area of single-character operators
			{
				nextChar; // increment to check on next operator
				if(OPERATOR < 4) // operators 0, 1, 2 and 3 have only '=' variant
				{
					if(CHAR == '=') // if this is the variant then write it in
						currentToken += CHAR;
					else // else go back
					{
						prevChar;
						if(CHAR == '=') // if operator is '=' and not '==' then its symbol
							newToken(TYPE_SYMBOL);
					}
				}
				else // all others operators have two variants '=' and himself again eg. '*', '*=', '**'
				{
					if(CHAR == '=' || CHAR == lexer::operators[OPERATOR])
					{
						currentToken += CHAR; // if it is variant, cool
						if((CHAR == '<' || CHAR == '>') && file::inputText.at(c + 1) == '=') // there are 2 three-char operators '<<=' and '>>='
						{
							nextChar;
							currentToken += '=';
						}
					}
					else // if its not, go back
						prevChar;
				}
				newToken(TYPE_OPERATOR); // create new token
			}
			else
				newToken(TYPE_OPERATOR); // just create new token if operator is single-char
		}
		else
			currentToken += CHAR; // else just append character to token
		if(CHAR == '\n')// if character is newline
		{
			currentLine++; // new line
			prevC = c + 1; // and update offset
		}
	}
	#undef CHAR
	
	token prevToken; // previous token
	for(token& iter : lexer::tokens) // iterate through tokens
	{
		if(iter.type == TYPE_UNDEF) // if type is undefined
		{
			if(isDec(iter.text)) // constant
				iter.type = TYPE_CONST;
			else if(iter.text == "*" || iter.text == "&") // unresolved '*' and '&'
			{
				if(prevToken.type == TYPE_CONST || prevToken.type == TYPE_INDENT) // if in operation, then its operator
					iter.type = TYPE_OPERATOR;
				else // else its symbol
					iter.type = TYPE_SYMBOL;
			}
			else
				iter.type = TYPE_INDENT; // indent is only left
		}
		
		if(debug)
		{
			coutd << int(iter.type) << ": " << iter.text;
#define POS_OFFSET 15
			if(iter.text.size() < POS_OFFSET)
				for(unsigned long i = 0; i < POS_OFFSET - iter.text.size(); i++)
					coutd << " ";
			coutd << " " << iter.line << ", " << iter.pos << std::endl;
		}
		
		prevToken = iter; // set previous token
	}
}

void newToken(int TYPE)
{
	if(currentToken.empty()) // if token is empty just do nothing
		return;
	token obj;
	obj.text = currentToken;
	obj.type = TYPE;
	obj.line = currentLine; // set token line to current line
	obj.pos = c - prevC - obj.text.size(); // position in line - length of obj
	if(obj.type == TYPE_OPERATOR || obj.type == TYPE_SYMBOL)
		obj.pos++;
	lexer::tokens.push_back(obj); // append token
	currentToken = ""; // reset token
}

/*unsigned long toDec(std::string text) // not going to explain it is a little too complex and i don't really want to
{
	char* p;
	unsigned long converted = strtol(text.c_str(), &p, 10);
	if(*p == 0)
		return converted;
	else if(!contains(text, '.'))
		return -1;
	else
	{
		int i = 0;
		for(char ch : text)
		{
			if(ch == '.')
				break;
			i++;
		}
		for(unsigned long c2 = 1; c < text.size() - i; c++)
		{
			char* p2;
			char temp = text.at(i + c2);
			std::string oneDec(1, temp);
            unsigned long res = strtol(oneDec.c_str(), &p2, 10);
			converted += res / c2*c2*c2*c2*c2*c2*c2*c2*c2*c2; // pow(10, c)
		}
		return converted;
	}
}*/

bool isInt(const std::string& text) // converts to integer
{
	char* p;
	strtol(text.c_str(), &p, 10);
	
	return *p == 0;
}

bool isDec(const std::string& text) // checks if its decimal
{
	int dot = 0;
	for(char i : text)
	{
		if(!contains(lexer::inNumber, i))
			return false;
		if(i == '.')
			dot++;
		if(dot == 2) // if there are more than 2 dots
			return false;
	}
	
	return true;
}
