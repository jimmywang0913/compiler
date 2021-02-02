#include "AST/CompoundStatement.hpp"
#include <vector>
    
// TODO
CompoundStatementNode::CompoundStatementNode(const uint32_t line,
                                             const uint32_t col, std::vector<AstNode *> *com_declarations,std::vector<AstNode *> *statements)
    : AstNode{line, col} {
    declarations=com_declarations;
	m_statements=statements;
    }

// TODO: You may use code snippets in AstDumper.cpp
void CompoundStatementNode::visitChildNodes(AstNodeVisitor &p_visitor) {
     // TODO
     if (declarations != NULL){
         for (auto &decl : *declarations){
             decl->accept(p_visitor);
         }
     }
     if (m_statements != NULL){
         for (auto &st : *m_statements){
             st->accept(p_visitor);
         }
     }
     
 }
