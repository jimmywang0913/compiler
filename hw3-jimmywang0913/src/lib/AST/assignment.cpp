#include "AST/assignment.hpp"

// TODO
AssignmentNode::AssignmentNode(const uint32_t line, const uint32_t col,AstNode *value, AstNode *expression)
    : AstNode{line, col} {
        variable_ref=value;
        EXPR=expression;
    }

// TODO: You may use code snippets in AstDumper.cpp
void AssignmentNode::print() {}

void AssignmentNode::visitChildNodes(AstNodeVisitor &p_visitor) {
     // TODO
     if (variable_ref!=NULL){
         variable_ref->accept(p_visitor);
     }
     if (EXPR != NULL){
         EXPR->accept(p_visitor);
     }
}

