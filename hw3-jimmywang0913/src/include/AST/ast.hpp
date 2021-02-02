#ifndef __AST_H
#define __AST_H

#include <cstdint>
#include <string>
#include <AST/AstDumper.hpp>
#include <vector>

struct Location {
    Location(const uint32_t line, const uint32_t col) : line(line), col(col) {}

    uint32_t line;
    uint32_t col;
};

class AstNode {
  public:
    AstNode(const uint32_t line, const uint32_t col);
    
    virtual ~AstNode() = 0;

    const Location &getLocation() const;
    virtual void print(){};
    virtual void accept(AstNodeVisitor &v) {}
    virtual void visitChildNodes(AstNodeVisitor &v) {}
  protected:
    const Location location;
};

#endif
