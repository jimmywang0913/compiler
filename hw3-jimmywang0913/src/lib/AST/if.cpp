#include "AST/if.hpp"
#include "AST/expression.hpp"
// TODO
IfNode::IfNode(const uint32_t line, const uint32_t col,AstNode *if_expression,CompoundStatementNode *if_statement1,
    	CompoundStatementNode *if_statement2)
    : AstNode{line, col} {
        expression=if_expression;
        statememt1=if_statement1;
        if (if_statement2 != NULL){
            statement2=if_statement2;
        }

    }

// TODO: You may use code snippets in AstDumper.cpp
void IfNode::print() {}

void IfNode::visitChildNodes(AstNodeVisitor &p_visitor) {
//     // TODO
    expression->accept(p_visitor);
    statememt1->accept(p_visitor);
    if (statement2 != NULL){
        statement2->accept(p_visitor);
    }
}
