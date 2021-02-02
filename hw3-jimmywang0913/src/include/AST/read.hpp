#ifndef __AST_READ_NODE_H
#define __AST_READ_NODE_H

#include "AST/ast.hpp"
#include "AST/VariableReference.hpp"


class ReadNode : public AstNode {
  public:
    ReadNode(const uint32_t line, const uint32_t col,
    	AstNode *read_var_r
             /* TODO: variable reference */);
    ~ReadNode() = default;
    void accept(AstNodeVisitor &v) { v.visit(*this); }
    void visitChildNodes(AstNodeVisitor &v) override;
    void print() override;

  private:
    // TODO: variable reference
    AstNode *var_r;
};

#endif
