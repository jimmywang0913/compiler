#include "AST/for.hpp"

// TODO
ForNode::ForNode(const uint32_t line, const uint32_t col,DeclNode *for_declaration, AstNode *for_assignment,
    	AstNode *for_expression,CompoundStatementNode *for_statement)
    : AstNode{line, col} {
        declaration=for_declaration;
        assignment=for_assignment;
        expression=for_expression;
        statement=for_statement;
    }

// TODO: You may use code snippets in AstDumper.cpp
void ForNode::print() {}

void ForNode::visitChildNodes(AstNodeVisitor &p_visitor) {
//     // TODO
    if (declaration!=NULL){
        declaration->accept(p_visitor);
    }
    if (assignment!=NULL){
        assignment->accept(p_visitor);
    }
    if (expression!=NULL){
        expression->accept(p_visitor);
    }
    if (statement!=NULL){
        statement->accept(p_visitor);
    }

}
