// this file parses expressions

#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

#define CURRENT parser::curr_token
#define CURRENT_OBJ(x) current_branch.sub.at(x)
#define ERASE_ELEMENT(x) current_branch.sub.erase(current_branch.sub.begin() + x)

void putInBraces(Branch& current_branch, std::vector<std::string> operators);

Branch parser::expr(bool optional_existence) {
    /*
     this goes through tokens and turns them into a tree
     */
    std::vector<std::string> equation_symbols = {"+", "-", "*", "/", "==", "<", ">"};
    Branch current_branch;
    current_branch.name = "expr";
    if(CURRENT->text == "-") { // turns -1 + 1 to 0-1 + 1
        appendBranch("0", current_branch);
        appendBranch("-", current_branch);
        parser::nextToken();
    }
    bool first_time = true;
    while(true) { // main expression loop
        if(parser::e::functionCall(current_branch));
        else if(CURRENT->type == Constant) // constant is very straight forward
            appendBranch(CURRENT->text, current_branch);
        else if(CURRENT->type == Indent) // variable has ':' prefixed
            appendBranch(":" + CURRENT->text, current_branch);
        else if(CURRENT->text == "(") { // '(' counts as another sub-expression
            parser::nextToken();
            Branch obj = parser::expr();
            appendBranch(obj, current_branch);
        } else
            if(optional_existence && first_time) {
                prevToken();
                Branch empty_expression;
                empty_expression.name = "expr";
                appendBranch("0", empty_expression);
                return empty_expression;
            }
            else
                error::syntaxError("Value expected");
        
        parser::nextToken();
        if(parser::curr_token == lexer::tokens.end()) // expressions always end with operator
            break;
        if(find(equation_symbols, CURRENT->text) != equation_symbols.size()) { // if there is valid operator
            if(parser::curr_token == --lexer::tokens.end()) // if operator is the last token
                error::syntaxError("File ends with a operator");
            appendBranch(CURRENT->text, current_branch);
        } else
            break;
        parser::nextToken();
        first_time = false;
    }
    if(parser::curr_token != lexer::tokens.end() && parser::peekNextToken() != lexer::tokens.end() && CURRENT->text != ")")
        parser::prevToken();
    
    // make sure that some operators get calculated first
    putInBraces(current_branch, {"*", "/"});
    putInBraces(current_branch, {"+", "-"});
    return current_branch;
}

void putInBraces(Branch& current_branch, std::vector<std::string> operators) {
    for(unsigned long i = 1; i < current_branch.sub.size(); i += 2)
        if(contains(operators, CURRENT_OBJ(i).name)) {
            Branch obj;
            obj.name = "expr";
            i--;
            parser::appendBranch(CURRENT_OBJ(i), obj);
            current_branch.sub.erase(current_branch.sub.begin() + i);
            while(i < current_branch.sub.size() - 2 && contains(operators, CURRENT_OBJ(i).name)) {
                // remove elements with those operators from equation and append it to separate equation
                parser::appendBranch(CURRENT_OBJ(i), obj);
                current_branch.sub.erase(current_branch.sub.begin() + i);
                parser::appendBranch(CURRENT_OBJ(i), obj);
                current_branch.sub.erase(current_branch.sub.begin() + i);
            }
            
            // now append that branch
            if(obj.sub.size() == 1)
                obj = obj.sub.at(0);
            current_branch.sub.insert(current_branch.sub.begin() + i, Branch());
            CURRENT_OBJ(i) = obj;
            i++;
        }
}
