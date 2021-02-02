#ifndef __SEMA_SYMBOLTABEL_NODE_H
#define __SEMA_SYMBOLTABEL_NODE_H

#include "sema/symbolentry.hpp"
#include <vector>
class SymbolTable {
public:
    void addSymbol(SymbolEntry n);
    // other methods
public:
    std::vector<SymbolEntry> entries;
};

#endif