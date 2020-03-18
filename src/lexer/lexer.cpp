#include "jaclang.h"

/* this is the LEXER which breaks code into lexer::tokens aka more readable code.
 * eg.
 * 
 * int i = 0;
 * token 1: type name, text: "int"
 * token 2: type: name, text: "i"
 * token 3: type: symbol, text: "="
 * token 4: type: constant, text: "0"
 * token 5: type: symbol, text: ";"
 *
 * types of tokens with examples:
 * 1. name:   a
 * 2. constant: 10
 * 3. string:   "Hello, World!"
 * 4. symbol:   {
 * 5. operator: +
 *
 * it breaks code so that parser can read it and turn it into syntax tree
 * */

#define IN_STRING (inStringQ || inStringDQ) // macro for later purposes

int currentLine = 1;

std::list<token> lexer::tokens; // vector of tokens

void newToken(int TYPE); // pushes current token to token vector and sets it to empty string
std::string currentToken; // current token in processing

unsigned long c = 0, prevC = 0;
std::list<char>::iterator cIter;

int isToken(char tok);

void lexer::main() { // main lexer function
    bool inStringQ = false, inStringDQ = false;  // if in string with single quotes or double quotes

#define CHAR *cIter // shortcut to get current character of string that for loop is parsing

    for (cIter = file::inputText.begin(); cIter != file::inputText.end(); cIter++) {
        if ((CHAR == ' ' || CHAR == '	' || CHAR == '\n') && !IN_STRING) {
            // tabs, spaces and newlines separate tokens if not in string
            newToken(TYPE_UNDEF);
            if (CHAR == '	')
                prevC++;
        } else if (CHAR == '\"' && !inStringQ) {
            inStringDQ = !inStringDQ;
            newToken(inStringDQ ? TYPE_UNDEF : TYPE_STRING);
            // if just got out of string, then this token is string
            // if coming to string end the token and start string token
        } else if (CHAR == '\'' && !inStringDQ) { // the same thing but for single quotes
            inStringQ = !inStringQ;
            newToken(inStringQ ? TYPE_UNDEF : TYPE_STRING);
        } else if(!IN_STRING) {
            int isTokenResult = isToken(CHAR); // check if is operator or symbol
            if (isTokenResult != TYPE_UNDEF) {
                newToken(TYPE_UNDEF);
                currentToken = CHAR;
                newToken(isTokenResult);
            }
            else
                currentToken += CHAR; // else just append character to token
        } else
            currentToken += CHAR; // else just append character to token
        if (CHAR == '\n') { // if character is newline
            currentLine++;
            prevC = c + 1; // and update offset
        }
        c++;
    }
#undef CHAR

    token prevToken; // previous token
    for (token &iter : lexer::tokens) { // iterate through tokens
        if (iter.type == TYPE_UNDEF) { // if type is undefined
            if (iter.text == "*" || iter.text == "&") { // unresolved '*' and '&'
                if (prevToken.type == TYPE_CONST || prevToken.type == TYPE_INDENT) // if in operation, then its operator
                    iter.type = TYPE_OPERATOR;
                else // else its symbol
                    iter.type = TYPE_SYMBOL;
            } else if (isInt(iter.text))
                iter.type = TYPE_CONST;
            else
                iter.type = TYPE_INDENT; // name is only left
        }

#define POS_OFFSET 15

        if (debug_show_tokens) {
            coutd << int(iter.type) << ": " << iter.text;
            if (iter.text.size() < POS_OFFSET)
                for (unsigned long i = 0; i < POS_OFFSET - iter.text.size(); i++)
                    coutd << " ";
            coutd << " " << iter.line << ", " << iter.pos << std::endl;
        }

        prevToken = iter; // set previous token
    }
}

void newToken(int TYPE) {
    if (currentToken.empty()) // if token is empty just do nothing
        return;
    token obj;
    obj.text = currentToken;
    obj.type = TYPE;
    obj.line = currentLine; // set token line to current line
    obj.pos = c - prevC - obj.text.size(); // position in line - length of obj
    if (obj.type == TYPE_OPERATOR || obj.type == TYPE_SYMBOL)
        obj.pos++;
    lexer::tokens.push_back(obj); // append token
    currentToken.clear(); // reset token
}

bool isInt(const std::string &text) { // converts to integer
    return !text.empty() && std::find_if(text.begin(),
                                         text.end(), [](unsigned char c) { return !std::isdigit(c); }) == text.end();
}

int isToken(char tok) {
    switch (tok) {
        case '=': // symbols with one character
        case '{':
        case '}':
        case '(':
        case ')':
            return TYPE_SYMBOL;
        case '+': // operators with one character
        case '-':
        case '*':
        case '/':
            return TYPE_OPERATOR;
        default:
            return 0;
    }
}
