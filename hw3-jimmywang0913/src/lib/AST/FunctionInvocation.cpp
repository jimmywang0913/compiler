#include "AST/FunctionInvocation.hpp"
#include <vector>

// TODO
FunctionInvocationNode::FunctionInvocationNode(const uint32_t line,
                                               const uint32_t col, 
                                               const char *funci_name,
                                               std::vector<AstNode *> *funci_expression)
    : ExpressionNode{line, col} ,name(funci_name){
        expression=funci_expression;
    }

// TODO: You may use code snippets in AstDumper.cpp
void FunctionInvocationNode::print() {}
void FunctionInvocationNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    if (expression!=NULL){
        for (auto &exp : *expression){
            exp->accept(p_visitor);
        }
    }
     // TODO
}
