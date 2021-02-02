#include "AST/print.hpp"

// TODO
PrintNode::PrintNode(const uint32_t line, const uint32_t col,AstNode *print_expression)
    : AstNode{line, col} {
        expression=print_expression;
    }

// TODO: You may use code snippets in AstDumper.cpp
void PrintNode::print() {}

void PrintNode::visitChildNodes(AstNodeVisitor &p_visitor) {
//     // TODO
    expression->accept(p_visitor);
}
