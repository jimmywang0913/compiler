#ifndef __AST_WHILE_NODE_H
#define __AST_WHILE_NODE_H

#include "AST/ast.hpp"
#include "AST/CompoundStatement.hpp"
#include "AST/expression.hpp"


class WhileNode : public AstNode {
  public:
    WhileNode(const uint32_t line, const uint32_t col,
    	AstNode *while_expression,CompoundStatementNode *while_statement
              /* TODO: expression, compound statement */);
    void accept(AstNodeVisitor &v) { v.visit(*this); }
    void visitChildNodes(AstNodeVisitor &v) override;
    ~WhileNode() = default;

    void print() override;

  private:
    // TODO: expression, compound statement
    AstNode *expression;
    CompoundStatementNode *statement;
    
};

#endif
