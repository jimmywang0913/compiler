#include "AST/while.hpp"

// TODO
WhileNode::WhileNode(const uint32_t line, const uint32_t col,AstNode *while_expression,CompoundStatementNode *while_statement)
    : AstNode{line, col} {
        expression=while_expression;
        statement=while_statement;
    }

// TODO: You may use code snippets in AstDumper.cpp
void WhileNode::print() {}

void WhileNode::visitChildNodes(AstNodeVisitor &p_visitor) {
//     // TODO
    if (expression!=NULL){
        expression->accept(p_visitor);
    }
    if(statement != NULL){
        statement->accept(p_visitor);
    }
}
