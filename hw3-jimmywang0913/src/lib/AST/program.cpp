#include "AST/program.hpp"
#include <vector>

// TODO
ProgramNode::ProgramNode(const uint32_t line, const uint32_t col,
                         const char *p_name, const char *p_return_type,
                	  std::vector<AstNode *> *p_declarations, std::vector<AstNode *> *p_functions, 
                CompoundStatementNode *p_statement)
    : AstNode{line, col}, name(p_name){
    	return_type=p_return_type;
    	declarations=p_declarations;
    	functions=p_functions;
    	statement=p_statement;
    }
const char *ProgramNode::getNameCString() const { return name.c_str(); }

void ProgramNode::visitChildNodes(AstNodeVisitor &p_visitor) { // visitor pattern version
    //TODO
    if (declarations != NULL){
         for (auto &decl : *declarations) {
          
          decl->accept(p_visitor);
          }
          
    }
    if (functions != NULL){
         for (auto &func : *functions) {
          func->accept(p_visitor);
     }
    }
     statement->accept(p_visitor);
     
}
