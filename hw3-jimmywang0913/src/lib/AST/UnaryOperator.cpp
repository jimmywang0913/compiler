#include "AST/UnaryOperator.hpp"

// TODO
UnaryOperatorNode::UnaryOperatorNode(const uint32_t line, const uint32_t col,
					char *unary_operator, 
					AstNode *un_bin_expression)
    : ExpressionNode{line, col} {
		bin_expression=un_bin_expression;
		m_operator=unary_operator;
	}

// TODO: You may use code snippets in AstDumper.cpp

void UnaryOperatorNode::print() {}

void UnaryOperatorNode::visitChildNodes(AstNodeVisitor &p_visitor) {
//     // TODO
	if (bin_expression != NULL){
		bin_expression->accept(p_visitor);
	}
}
