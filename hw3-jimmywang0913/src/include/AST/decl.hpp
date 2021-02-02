#ifndef __AST_DECL_NODE_H
#define __AST_DECL_NODE_H

#include "AST/ast.hpp"
#include "AST/ConstantValue.hpp"
#include <vector>
#include <string>
using namespace std;
class DeclNode : public AstNode {
  public:
    // variable declaration
    DeclNode(const uint32_t line, const uint32_t col,const uint32_t v_line, const uint32_t v_col,
    		std::vector<const char *> *dec_identifier,const char *dec_type
             /* TODO: identifiers, type */);

    // constant variable declaration
    DeclNode(const uint32_t, const uint32_t col,const uint32_t v_line, const uint32_t v_col,const uint32_t const_col,
     std::vector<const char *> *dec_identifier,const char *const_constant, int tt
            /* TODO: identifiers, constant */);
    DeclNode(const uint32_t line, const uint32_t col,char *dec_identifier);
    void accept(AstNodeVisitor &v) { v.visit(*this); }
    void visitChildNodes(AstNodeVisitor &v) override;
    ~DeclNode() = default;

    void print() override;

  public:
  char *idd;
  std::vector<const char *> *identifier;
  const uint32_t vv_line;
  const uint32_t vv_col;
  const uint32_t cconst_col;
  	const char *type;
    int ttype;
    int ttt;
  const char *constant;
    // TODO: variables
};

#endif
