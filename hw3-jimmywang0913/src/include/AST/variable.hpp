#ifndef __AST_VARIABLE_NODE_H
#define __AST_VARIABLE_NODE_H

#include "AST/ast.hpp"


class VariableNode : public AstNode {
  public:
    VariableNode(const uint32_t line, const uint32_t col,
    		const char *var_name, const char *var_type, const char *var_value, const uint32_t const_col
                 /* TODO: variable name, type, constant value */);
    ~VariableNode() = default;
    void accept(AstNodeVisitor &v) { v.visit(*this); }
    void visitChildNodes(AstNodeVisitor &v) override;
    void print() override;

  public:
    // TODO: variable name, type, constant value
    const char *name;
    const char *type;
    const char *value;
    const uint32_t col;
};

#endif
