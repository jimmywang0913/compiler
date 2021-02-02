#ifndef __AST_RETURN_NODE_H
#define __AST_RETURN_NODE_H

#include "AST/ast.hpp"
#include "AST/expression.hpp"


class ReturnNode : public AstNode {
  public:
    ReturnNode(const uint32_t line, const uint32_t col,
    		AstNode *return_expression
               /* TODO: expression */);
    ~ReturnNode() = default;
    void accept(AstNodeVisitor &v) { v.visit(*this); }
    void visitChildNodes(AstNodeVisitor &v) override;
    void print() override;

  private:
  AstNode *expression;
    // TODO: expression
};

#endif
