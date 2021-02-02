#ifndef __AST_PRINT_NODE_H
#define __AST_PRINT_NODE_H

#include "AST/ast.hpp"
#include "AST/expression.hpp"

class PrintNode : public AstNode {
  public:
    PrintNode(const uint32_t line, const uint32_t col,
    	AstNode *print_expression
              /* TODO: expression */);
    ~PrintNode() = default;
    void accept(AstNodeVisitor &v) { v.visit(*this); }
    void visitChildNodes(AstNodeVisitor &v) override;
    void print() override;

  private:
    // TODO: expression
    AstNode *expression;
};

#endif
