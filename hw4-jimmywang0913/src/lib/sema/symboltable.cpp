#include "sema/symboltable.hpp"
#include "sema/symbolentry.hpp"
#include <vector>
using namespace std;
void SymbolTable::addSymbol(SymbolEntry s){
    entries.push_back(s);
}
