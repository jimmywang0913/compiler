#ifndef __AST_PROGRAM_NODE_H
#define __AST_PROGRAM_NODE_H

#include "AST/ast.hpp"
#include "AST/decl.hpp"
#include "AST/CompoundStatement.hpp"
#include "AST/function.hpp"
#include "AST/AstDumper.hpp"

class ProgramNode : public AstNode {
  public:
    ProgramNode(const uint32_t line, const uint32_t col,
                const char *p_name,const char *p_return_type,
                std::vector<AstNode *> *p_declarations, std::vector<AstNode *> *p_functions, 
                CompoundStatementNode *p_statement
                /* TODO: return type, declarations, functions,
                 *       compound statement */);
    ~ProgramNode() = default;

    // visitor pattern version: 
    const char *getNameCString() const; 
    void accept(AstNodeVisitor &v) { v.visit(*this); }
    void visitChildNodes(AstNodeVisitor &v) override;

  private:
    // AstNode#getLocation
    std::string name;
    std::string return_type;
    std::vector<AstNode *> *declarations;
    std::vector<AstNode *> *functions;
    CompoundStatementNode *statement;
    
    // TODO: return type, declarations, functions, compound statement
};

#endif
