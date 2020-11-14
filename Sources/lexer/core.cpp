#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

#define IN_STRING (in_string_q || in_string_dq)

int current_line = 1;

void newToken(TokenType TYPE); // pushes current token to token vector and sets it to empty string
std::string current_token; // current token in processing

unsigned int c = 0, prev_c = 0;
std::list<char>::iterator curr_char;

TokenType isToken(char tok);
TokenType isToken(char tok1, char tok2);

void lexer::main() { // main lexer function
    bool in_string_q = false, in_string_dq = false;  // if in string with single quotes or double quotes
    
    for(curr_char = file::input_text.begin(); curr_char != file::input_text.end(); curr_char++) {
        if(!IN_STRING && (*curr_char == ' ' || *curr_char == '	' || *curr_char == '\n')) {
            // tabs, spaces and newlines separate tokens if not in string
            newToken(Undefined);
            if(*curr_char == '	')
                prev_c++;
        } else if(*curr_char == '\"' && !in_string_q) {
            in_string_dq = !in_string_dq;
            newToken(in_string_dq ? Undefined : String);
            // if just got out of string, then this token is string
            // if coming to string end the token and start string token
        } else if(*curr_char == '\'' && !in_string_dq) {
            in_string_q = !in_string_q;
            newToken(in_string_q ? Undefined : String);
        } else if(!IN_STRING) {
            char tok1 = *curr_char, tok2;
            curr_char++;
            tok2 = *curr_char;
            TokenType isTokenResult = isToken(tok1, tok2); // check if is operator or symbol
            if(isTokenResult != Undefined) {
                newToken(Undefined);
                current_token = std::string(1, tok1) + tok2;
                newToken(isTokenResult);
                c++;
                goto end;
            } else
                curr_char--;
            
            isTokenResult = isToken(*curr_char); // check if is operator or symbol
            if(isTokenResult != Undefined) {
                newToken(Undefined);
                current_token = *curr_char;
                newToken(isTokenResult);
                goto end;
            }
            current_token.push_back(*curr_char);
        } else
            current_token.push_back(*curr_char); // else just append character to token
        end:
        if(*curr_char == '\n') { // if character is newline
            current_line++;
            prev_c = c + 1; // and update offset
        }
        c++;
    }

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

bool isInt(const std::string &text) { // checks if string can be integer
    return !text.empty() && std::find_if(text.begin(), text.end(), [](unsigned char c2) { return !std::isdigit(c2); }) == text.end();
}

TokenType isToken(char char1) {
    switch(char1) {
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

TokenType isToken(char char1, char char2) {
    switch(char1) {
        case '=': // operators with one character
            switch(char2) {
                case '=':
                    return Operator;
                default:
                    return Undefined;
            }
        default:
            return Undefined;
    }
}
