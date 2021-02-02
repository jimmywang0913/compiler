#include "AST/return.hpp"

// TODO
ReturnNode::ReturnNode(const uint32_t line, const uint32_t col,AstNode *return_expression)
    : AstNode{line, col} {
        expression=return_expression;
    }

// TODO: You may use code snippets in AstDumper.cpp
void ReturnNode::print() {}

void ReturnNode::visitChildNodes(AstNodeVisitor &p_visitor) {
     // TODO
     if (expression!=NULL){
         expression->accept(p_visitor);
     }
}
