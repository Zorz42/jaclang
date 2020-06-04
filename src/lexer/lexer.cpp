#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

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

void newToken(tokenType TYPE); // pushes current token to token vector and sets it to empty string
std::string currentToken; // current token in processing

unsigned long c = 0, prevC = 0;
std::list<char>::iterator cIter;

tokenType isToken(char tok);
tokenType isToken(char tok1, char tok2);

void lexer::main() { // main lexer function
    bool inStringQ = false, inStringDQ = false;  // if in string with single quotes or double quotes

#define CHAR *cIter // shortcut to get current character of string that for loop is parsing

    for (cIter = file::inputText.begin(); cIter != file::inputText.end(); cIter++) {
        if ((CHAR == ' ' || CHAR == '	' || CHAR == '\n') && !IN_STRING) {
            // tabs, spaces and newlines separate tokens if not in string
            newToken(tt_undefined);
            if (CHAR == '	')
                prevC++;
        } else if (CHAR == '\"' && !inStringQ) {
            inStringDQ = !inStringDQ;
            newToken(inStringDQ ? tt_undefined : tt_string);
            // if just got out of string, then this token is string
            // if coming to string end the token and start string token
        } else if (CHAR == '\'' && !inStringDQ) { // the same thing but for single quotes
            inStringQ = !inStringQ;
            newToken(inStringQ ? tt_undefined : tt_string);
        } else if (!IN_STRING) {
            char tok1 = CHAR, tok2;
            cIter++;
            tok2 = CHAR;
            tokenType isTokenResult = isToken(tok1, tok2); // check if is operator or symbol
            if (isTokenResult != tt_undefined) {
                newToken(tt_undefined);
                currentToken = std::string(1, tok1) + tok2;
                newToken(isTokenResult);
                c++;
                goto END;
            } else
                cIter--;
            
            isTokenResult = isToken(CHAR); // check if is operator or symbol
            if (isTokenResult != tt_undefined) {
                newToken(tt_undefined);
                currentToken = CHAR;
                newToken(isTokenResult);
                goto END;
            }
            currentToken.push_back(CHAR);
        } else
            currentToken.push_back(CHAR); // else just append character to token
        END:
        if (CHAR == '\n') { // if character is newline
            currentLine++;
            prevC = c + 1; // and update offset
        }
        c++;
    }
#undef CHAR

    token prevToken; // previous token
    for (token &iter : lexer::tokens) { // iterate through tokens
        if (iter.type == tt_undefined) { // if type is undefined
            if (iter.text == "*" || iter.text == "&") { // unresolved '*' and '&'
                if (prevToken.type == tt_constant || prevToken.type == tt_indent) // if in operation, then its operator
                    iter.type = tt_operator;
                else // else its symbol
                    iter.type = tt_symbol;
            } else if (isInt(iter.text))
                iter.type = tt_constant;
            else
                iter.type = contains(lexer::keywords, iter.text) ? tt_keyword : tt_indent; // name is only left
        }

#define POS_OFFSET 15

        if (debug_show_tokens) { // print debug tokens
            coutd << int(iter.type) << ": " << iter.text;
            if (iter.text.size() < POS_OFFSET)
                for (unsigned long i = 0; i < POS_OFFSET - iter.text.size(); i++)
                    coutd << " ";
            coutd << " " << iter.line << ", " << iter.pos << std::endl;
        }

        prevToken = iter; // set previous token
    }
}

void newToken(tokenType TYPE) {
    if (currentToken.empty()) // if token is empty just do nothing
        return;
    token obj;
    obj.text = currentToken;
    obj.type = TYPE;
    obj.line = currentLine; // set token line to current line
    obj.pos = c - prevC - obj.text.size(); // position in line - length of obj
    if (obj.type == tt_operator || obj.type == tt_symbol)
        obj.pos++;
    lexer::tokens.push_back(obj); // append token
    currentToken.clear(); // reset token
}

bool isInt(const std::string &text) { // converts to integer
    return !text.empty() && std::find_if(text.begin(),
                                         text.end(), [](unsigned char c) { return !std::isdigit(c); }) == text.end();
}

tokenType isToken(char tok) {
    switch (tok) {
        case '=': // symbols with one character
        case '{':
        case '}':
        case '(':
        case ')':
            return tt_symbol;
        case '+': // operators with one character
        case '-':
        case '*':
        case '/':
        case '>':
        case '<':
            return tt_operator;
        default:
            return tt_undefined;
    }
}

tokenType isToken(char tok1, char tok2) {
    switch (tok1) {
        case '=': // operators with one character
            switch (tok2) {
                case '=':
                    return tt_operator;
            }
        default:
            return tt_undefined;
    }
}
