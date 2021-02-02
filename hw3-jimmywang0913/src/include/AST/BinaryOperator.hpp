#ifndef __AST_BINARY_OPERATOR_NODE_H
#define __AST_BINARY_OPERATOR_NODE_H

#include "AST/expression.hpp"
#include "AST/UnaryOperator.hpp"

#include <memory>

class BinaryOperatorNode : public ExpressionNode {
  public:
    BinaryOperatorNode(const uint32_t line, const uint32_t col,
    			char *bin_operator,AstNode *left_expression,AstNode *right_expression
                       /* TODO: operator, expressions */);
    void accept(AstNodeVisitor &v) { v.visit(*this); }
    void visitChildNodes(AstNodeVisitor &v) override;
    ~BinaryOperatorNode() = default;

    void print() override;

  public:
    // TODO: operator, expressions
    char *m_operator;
    AstNode *bin_left_expression;
    AstNode *bin_right_expression;
};

#endif
