#include "sema/symbolmanager.hpp"
#include <stack>

void SymbolManager::pushScope(SymbolTable new_scope){
    tables.push(new_scope);
}
void SymbolManager::popScope(){
    tables.pop();
}
