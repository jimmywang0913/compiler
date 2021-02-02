#ifndef __AST_CONSTANT_VALUE_NODE_H
#define __AST_CONSTANT_VALUE_NODE_H

#include "AST/expression.hpp"

class ConstantValueNode : public ExpressionNode {
  public:
    ConstantValueNode(const uint32_t line, const uint32_t col,
    		const char *const_value
                      /* TODO: constant value */);
    void accept(AstNodeVisitor &v) { v.visit(*this); }
    ~ConstantValueNode() = default;

    void print() override;

  public:
    // TODO: constant value
    const char *value;
};

#endif
