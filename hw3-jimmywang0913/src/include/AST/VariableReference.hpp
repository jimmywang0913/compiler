#ifndef __AST_VARIABLE_REFERENCE_NODE_H
#define __AST_VARIABLE_REFERENCE_NODE_H

#include "AST/expression.hpp"
#include <vector>

class VariableReferenceNode : public ExpressionNode {
  public:
    // normal reference
    VariableReferenceNode(const uint32_t line, const uint32_t col,const char *name
                          /* TODO: name */);
    // array reference
    VariableReferenceNode(const uint32_t line, const uint32_t col,const char *name,std::vector<AstNode *> *var_r_expressions
    //                       /* TODO: name, expressions */
    );
    void accept(AstNodeVisitor &v) { v.visit(*this); }
    void visitChildNodes(AstNodeVisitor &v) override;
    ~VariableReferenceNode() = default;

    void print() override;

  public:
    // TODO: variable name, expressions
    const char *m_name;
    std::vector<AstNode *>* expressions;
    
};

#endif
