// this parses variable declaration

#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

#define current parser::currToken

bool parser::e::variableDeclaration() {
    if (parser::currToken == --lexer::tokens.end())
        return false;
    if (contains(generator::primitiveDatatypes, current->text)) { // if first text is a primitive datatype
        branch currentBranch;
        currentBranch.alloc();
        currentBranch.name = "variableDeclaration"; // set to variableDeclaration
        appendBranch(current->text, currentBranch);
        parser::nextToken();
        if (current->type != TYPE_INDENT) // check if everything is working out and append it to main branch
            error::syntaxError("Expected name after value type name in variable declaration");
        appendBranch(current->text, currentBranch);
        parser::nextToken();
        if (current->text != "=")
            error::syntaxError("Expected '=' after value type name in variable declaration");
        parser::nextToken();
        branch equationBranch = parser::calculation(true);
        appendBranch(equationBranch, currentBranch);
        appendBranch(currentBranch, *currentBranchScope);
        return true;
    } else
        return false;
}

bool parser::e::variableSetting() {
    if (current == --lexer::tokens.end())
        return false;
    if (current->type == TYPE_INDENT && parser::peekNextToken()->text == "=") {
        branch currentBranch;
        currentBranch.alloc();
        currentBranch.name = "variableSetting";
        appendBranch(current->text, currentBranch);
        parser::nextToken();
        parser::nextToken();
        branch equationBranch = parser::calculation(true);
        appendBranch(equationBranch, currentBranch);
        appendBranch(currentBranch, *currentBranchScope);
        return true;
    } else
        return false;
}
