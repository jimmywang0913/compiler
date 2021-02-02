#ifndef __AST_FUNCTION_NODE_H
#define __AST_FUNCTION_NODE_H

#include "AST/ast.hpp"
#include "AST/decl.hpp"
#include "AST/return.hpp"
#include "AST/CompoundStatement.hpp"
#include <vector>
#include <string>


class FunctionNode : public AstNode {
  public:
    FunctionNode(const uint32_t line, const uint32_t col,
    		const char *func_name, std::vector<AstNode *> *func_delerations, const char *func_type , CompoundStatementNode *func_com,char *d1
                 /* TODO: name, declarations, return type,
                  *       compound statement (optional) */);
    FunctionNode(const uint32_t line, const uint32_t col,
    		const char *func_name, std::vector<AstNode *> *func_delerations, const char *func_type,char *d1);
    void accept(AstNodeVisitor &v) { v.visit(*this); }
    void visitChildNodes(AstNodeVisitor &v) override;
    ~FunctionNode() = default;

    void print() override;

  public:
    // TODO: name, declarations, return type, compound statement
    const char *name;
    std::string str1;
    std::vector<AstNode *> *declaration;
    const char *type;
    char *display1,*display2;
    CompoundStatementNode *com;
};

#endif
