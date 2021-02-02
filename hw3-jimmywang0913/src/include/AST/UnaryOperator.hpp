#ifndef __AST_UNARY_OPERATOR_NODE_H
#define __AST_UNARY_OPERATOR_NODE_H

#include "AST/expression.hpp"


class UnaryOperatorNode : public ExpressionNode {
  public:
    UnaryOperatorNode(const uint32_t line, const uint32_t col,
    		char *unary_operator, AstNode *un_bin_expression
                      /* TODO: operator, expression */);
    ~UnaryOperatorNode() = default;
    void accept(AstNodeVisitor &v) { v.visit(*this); }
    void visitChildNodes(AstNodeVisitor &v) override;
    void print() override;

  public:
    // TODO: operator, expression
    char *m_operator;
    AstNode *bin_expression;
    };

#endif
