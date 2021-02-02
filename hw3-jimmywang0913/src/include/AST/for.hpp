#ifndef __AST_FOR_NODE_H
#define __AST_FOR_NODE_H

#include "AST/ast.hpp"
#include "AST/decl.hpp"
#include "AST/CompoundStatement.hpp"
#include "AST/expression.hpp"


class ForNode : public AstNode {
  public:
    ForNode(const uint32_t line, const uint32_t col,
    	DeclNode *for_declaration, AstNode *for_assignment,
    	AstNode *for_expression,CompoundStatementNode *for_statement
            /* TODO: declaration, assignment, expression,
             *       compound statement */);
    ~ForNode() = default;
    void accept(AstNodeVisitor &v) { v.visit(*this); }
    void visitChildNodes(AstNodeVisitor &v) override;
    void print() override;

  private:
    // TODO: declaration, assignment, expression, compound statement
    DeclNode *declaration;
    AstNode *assignment;
    AstNode *expression;
    CompoundStatementNode *statement;
    
};

#endif
