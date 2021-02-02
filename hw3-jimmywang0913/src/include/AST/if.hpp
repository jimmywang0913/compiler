#ifndef __AST_IF_NODE_H
#define __AST_IF_NODE_H

#include "AST/ast.hpp"
#include "AST/CompoundStatement.hpp"
#include "AST/expression.hpp"
class IfNode : public AstNode {
  public:
    IfNode(const uint32_t line, const uint32_t col,
    	AstNode *if_expression,CompoundStatementNode *if_statement1,
    	CompoundStatementNode *if_statement2
           /* TODO: expression, compound statement, compound statement */);
    ~IfNode() = default;
    void accept(AstNodeVisitor &v) { v.visit(*this); }
    void visitChildNodes(AstNodeVisitor &v) override;
    void print() override;

  private:
    // TODO: expression, compound statement, compound statement
    AstNode *expression;
    CompoundStatementNode *statememt1;
    CompoundStatementNode *statement2;
    };

#endif
