#ifndef __AST_COMPOUND_STATEMENT_NODE_H
#define __AST_COMPOUND_STATEMENT_NODE_H

#include "AST/ast.hpp"
#include "AST/decl.hpp"
#include <vector>


class CompoundStatementNode : public AstNode {
  public:
    CompoundStatementNode(const uint32_t line, const uint32_t col,
    			std::vector<AstNode *> *com_declarations, std::vector<AstNode *> *statements
                          /* TODO: declarations, statements */);
    ~CompoundStatementNode() = default;

    void accept(AstNodeVisitor &v) { v.visit(*this); }
    void visitChildNodes(AstNodeVisitor &v) override;

  private:
    // TODO: declarations, statements
     std::vector<AstNode *> *declarations;
     std::vector<AstNode *> *m_statements;
};

#endif
