#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

#define IN_STRING (in_string_q || in_string_dq)

int current_line = 1;

void newToken(TokenType TYPE); // pushes current token to token vector and sets it to empty string
std::string current_token; // current token in processing

unsigned int c = 0, prev_c = 0;
std::list<char>::iterator c_iter;

TokenType isToken(char tok);
TokenType isToken(char tok1, char tok2);

void lexer::main() { // main lexer function
    bool in_string_q = false, in_string_dq = false;  // if in string with single quotes or double quotes

#define CHAR *c_iter // shortcut to get current character of string that for loop is parsing
    
    for(c_iter = file::input_text.begin(); c_iter != file::input_text.end(); c_iter++) {
        if(!IN_STRING && (CHAR == ' ' || CHAR == '	' || CHAR == '\n')) {
            // tabs, spaces and newlines separate tokens if not in string
            newToken(Undefined);
            if(CHAR == '	')
                prev_c++;
        } else if(CHAR == '\"' && !in_string_q) {
            in_string_dq = !in_string_dq;
            newToken(in_string_dq ? Undefined : String);
            // if just got out of string, then this token is string
            // if coming to string end the token and start string token
        } else if(CHAR == '\'' && !in_string_dq) {
            in_string_q = !in_string_q;
            newToken(in_string_q ? Undefined : String);
        } else if(!IN_STRING) {
            char tok1 = CHAR, tok2;
            c_iter++;
            tok2 = CHAR;
            TokenType isTokenResult = isToken(tok1, tok2); // check if is operator or symbol
            if(isTokenResult != Undefined) {
                newToken(Undefined);
                current_token = std::string(1, tok1) + tok2;
                newToken(isTokenResult);
                c++;
                goto end;
            } else
                c_iter--;
            
            isTokenResult = isToken(CHAR); // check if is operator or symbol
            if(isTokenResult != Undefined) {
                newToken(Undefined);
                current_token = CHAR;
                newToken(isTokenResult);
                goto end;
            }
            current_token.push_back(CHAR);
        } else
            current_token.push_back(CHAR); // else just append character to token
        end:
        if(CHAR == '\n') { // if character is newline
            current_line++;
            prev_c = c + 1; // and update offset
        }
        c++;
    }
#undef CHAR

    Token prev_token; // previous token
    for(Token &iter : lexer::tokens) { // iterate through tokens
        if(iter.type == Undefined) { // if type is undefined
            if(iter.text == "*" || iter.text == "&") { // unresolved '*' and '&'
                if(prev_token.type == Constant || prev_token.type == Indent) // if in operation, then its operator
                    iter.type = Operator;
                else // else its symbol
                    iter.type = Symbol;
            } else if(isInt(iter.text))
                iter.type = Constant;
            else
                iter.type = contains(lexer::keywords, iter.text) ? Keyword : Indent; // name is only left
        }

#define POS_OFFSET 15

        if(debug_show_tokens) { // print debug tokens
            std::cout << int(iter.type) << ": " << iter.text;
            if(iter.text.size() < POS_OFFSET)
                for(unsigned long i = 0; i < POS_OFFSET - iter.text.size(); i++)
                    std::cout << " ";
            std::cout << " " << iter.line << ", " << iter.pos << std::endl;
        }

        prev_token = iter; // set previous token
    }
}

void newToken(TokenType TYPE) {
    if(current_token.empty()) // if token is empty just do nothing
        return;
    Token obj;
    obj.text = current_token;
    obj.type = TYPE;
    obj.line = current_line; // set token line to current line
    obj.pos = c - prev_c - (unsigned int)obj.text.size(); // position in line - length of obj
    if(obj.type == Operator || obj.type == Symbol)
        obj.pos++;
    lexer::tokens.push_back(obj); // append token
    current_token.clear(); // reset token
}

bool isInt(const std::string &text) { // converts to integer
    return !text.empty() && std::find_if(text.begin(), text.end(), [](unsigned char c2) { return !std::isdigit(c2); }) == text.end();
}

TokenType isToken(char tok) {
    switch(tok) {
        case '=': // symbols with one character
        case '{':
        case '}':
        case '(':
        case ')':
        case ',':
            return Symbol;
        case '+': // operators with one character
        case '-':
        case '*':
        case '/':
        case '>':
        case '<':
            return Operator;
        default:
            return Undefined;
    }
}

TokenType isToken(char tok1, char tok2) {
    switch(tok1) {
        case '=': // operators with one character
            switch(tok2) {
                case '=':
                    return Operator;
            }
        default:
            return Undefined;
    }
}
