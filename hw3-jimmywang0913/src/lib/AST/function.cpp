#include "AST/function.hpp"
#include <vector>
#include "AST/CompoundStatement.hpp"
#include "AST/decl.hpp"
#include <cstring>

// TODO
FunctionNode::FunctionNode(const uint32_t line, const uint32_t col,const char *func_name, std::vector<AstNode *> *func_delerations, const char *func_type, CompoundStatementNode *func_com,char *d1)
    : AstNode{line, col} ,name(func_name) ,type(func_type){
        declaration=func_delerations;
        com=func_com;
        display1=d1;

        char *str3 = new char[514];
        str3[0]='(';
        if (declaration != NULL){
            int currant_place=1;
            for (auto &decl : *declaration){
                DeclNode *node = static_cast<DeclNode *>(decl);
                int times = node->identifier->size();
                for (int i=0;i<times;i++){
                    size_t l1 = strlen(node->type);
                    strcpy(str3+currant_place, node->type);
                    currant_place += l1;
                    str3[currant_place] = ',';
                    currant_place++;
                    str3[currant_place] = ' ';
                    currant_place++;
                }
            }
            str3[currant_place-2] = ')';
            str3[currant_place-1] = 0;
        }
        else {
            str3[1]=')';
        }
        display2=str3;
    }
FunctionNode::FunctionNode(const uint32_t line, const uint32_t col,const char *func_name, std::vector<AstNode *> *func_delerations, const char *func_type, char *d1)
    : AstNode{line, col} ,name(func_name) ,type(func_type){
        declaration=func_delerations;
        display1=d1;
        char *str3 = new char[514];
        str3[0]='(';
        if (declaration != NULL){
            int currant_place=1;
            for (auto &decl : *declaration){
                DeclNode *node = static_cast<DeclNode *>(decl);
                int times = node->identifier->size();
                for (int i=0;i<times;i++){
                    size_t l1 = strlen(node->type);
                    strcpy(str3+currant_place, node->type);
                    currant_place += l1;
                    str3[currant_place] = ',';
                    currant_place++;
                    str3[currant_place] = ' ';
                    currant_place++;
                }
            }
            str3[currant_place-2] = ')';
            str3[currant_place-1] = 0;
        }
        else {
            str3[1]=')';
        }
        display2=str3;
    }
// TODO: You may use code snippets in AstDumper.cpp
void FunctionNode::print() {}
void FunctionNode::visitChildNodes(AstNodeVisitor &p_visitor) {
//     // TODO
    if (declaration != NULL){
        for (auto &decl : *declaration){
            decl->accept(p_visitor);
        }
    }
    if (com != NULL){
        com->accept(p_visitor);
    }

}
