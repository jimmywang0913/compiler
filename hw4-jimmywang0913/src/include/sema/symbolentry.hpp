#ifndef __SEMA_SYMBOLENTRY_NODE_H
#define __SEMA_SYMBOLENTRY_NODE_H
#include <vector>
#include <string>

class SymbolEntry {

public:
    // Variable names
    // Kind
    // Level
    // ...
    std::string m_name;
    std::string m_kind;
    int m_level;
    std::string m_type;
    std::string m_attribute;
    //SymbolEntry(std::string name,std::string *kind,int level,std::string type,std::string attribute);
};

#endif