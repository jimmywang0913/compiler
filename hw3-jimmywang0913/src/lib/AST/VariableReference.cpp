#include "AST/VariableReference.hpp"
#include <vector>

// TODO
VariableReferenceNode::VariableReferenceNode(const uint32_t line,
                                             const uint32_t col,
                                             const char *name)
    : ExpressionNode{line, col},m_name(name) {
        
    }

// TODO
VariableReferenceNode::VariableReferenceNode(const uint32_t line,
                                             const uint32_t col,
                                             const char *name,
                                             std::vector<AstNode *> *var_r_expressions)
    : ExpressionNode{line, col},m_name(name) {
        expressions=var_r_expressions;
    }

// TODO: You may use code snippets in AstDumper.cpp
void VariableReferenceNode::print() {}

void VariableReferenceNode::visitChildNodes(AstNodeVisitor &p_visitor) {
     // TODO
    if (expressions != NULL){
        for (auto &exp : *expressions){
            exp->accept(p_visitor);
        }
    }
}
