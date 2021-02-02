#ifndef __SEMA_SYMBOLMANAGER_NODE_H
#define __SEMA_SYMBOLMANAGER_NODE_H
#include "sema/symboltable.hpp"
#include <stack>

class SymbolManager {
public:
    void pushScope(SymbolTable new_scope);
    void popScope();
    // other methods
    std::stack <SymbolTable > tables;
};

#endif