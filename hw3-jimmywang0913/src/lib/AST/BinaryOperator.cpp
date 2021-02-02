#include "AST/BinaryOperator.hpp"
#include "AST/expression.hpp"

// TODO
BinaryOperatorNode::BinaryOperatorNode(const uint32_t line, const uint32_t col,char *bin_operator,AstNode *left_expression, AstNode *right_expression):
    ExpressionNode{line, col}{
        m_operator=bin_operator;
        bin_left_expression=left_expression;
        bin_right_expression=right_expression;
    }

// TODO: You may use code snippets in AstDumper.cpp
void BinaryOperatorNode::print() {}

void BinaryOperatorNode::visitChildNodes(AstNodeVisitor &p_visitor) {
//     // TODO
    if (bin_left_expression != NULL){
        bin_left_expression->accept(p_visitor);
    }
    if (bin_right_expression != NULL){
        bin_right_expression->accept(p_visitor);
    }
}
