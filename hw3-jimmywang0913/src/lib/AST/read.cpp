#include "AST/read.hpp"

// TODO
ReadNode::ReadNode(const uint32_t line, const uint32_t col,AstNode *read_var_r)
    : AstNode{line, col} {
        var_r=read_var_r;
    }

// TODO: You may use code snippets in AstDumper.cpp
void ReadNode::print() {}

void ReadNode::visitChildNodes(AstNodeVisitor &p_visitor) {
     // TODO
     if (var_r!=NULL){
         var_r->accept(p_visitor);
     }
}
