// This will detect scopes

#ifndef IGNORE_MAIN_INCLUDE
#include "jaclang.h"
#endif

#define CURRENT parser::curr_token

bool parser::e::beginScope() { // a simple scope begin
    if(CURRENT->text == "{") {
        Branch scope;
        scope.alloc();
        scope.name = "scope";
        appendBranch(scope, *current_branch_scope);
        current_branch_scope = &(current_branch_scope->sub->back());
        current_branch_scope->alloc();
        scopes.push_back(current_branch_scope);
        return true;
    } else
        return false;
}

bool parser::e::endScope() { // a simple scope end
    if(CURRENT->text == "}") {
        if(scopes.size() < 2)
            error::syntaxError("There is no scope to end");
        scopes.pop_back();
        current_branch_scope = scopes.back();
        return true;
    } else
        return false;
}
