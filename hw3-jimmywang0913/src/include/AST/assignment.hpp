#ifndef __AST_ASSIGNMENT_NODE_H
#define __AST_ASSIGNMENT_NODE_H

#include "AST/program.hpp"
#include "AST/variable.hpp"
#include "AST/expression.hpp"
#include "AST/VariableReference.hpp"
#include "AST/ast.hpp"

class AssignmentNode : public AstNode {
  public:
    AssignmentNode(const uint32_t line, const uint32_t col,
    		AstNode *value, AstNode *expression
                   /* TODO: variable reference, expression */);
    void accept(AstNodeVisitor &v) { v.visit(*this); }
    void visitChildNodes(AstNodeVisitor &v) override;
    ~AssignmentNode() = default;

    void print() override;

  private:
  AstNode *variable_ref;
  AstNode *EXPR;
    // TODO: variable reference, expression
};

#endif
