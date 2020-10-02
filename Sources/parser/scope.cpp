// This will detect scopes

#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

#define CURRENT parser::curr_token

bool parser::e::beginScope() { // a simple scope begin
    if(CURRENT->text == "{") {
        appendBranch(Branch("scope"), *current_branch_scope);
        current_branch_scope = &(current_branch_scope->sub.back());
        scopes.push(current_branch_scope);
        return true;
    } else
        return false;
}

bool parser::e::endScope() { // a simple scope end
    if(CURRENT->text == "}") {
        if(scopes.size() < 2)
            error::syntaxError("There is no scope to end");
        scopes.pop();
        current_branch_scope = scopes.top();
        return true;
    } else
        return false;
}
