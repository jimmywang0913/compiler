#include "AST/variable.hpp"
#include "AST/ConstantValue.hpp"
#include <string>
#include <cstring>

// TODO
VariableNode::VariableNode(const uint32_t line, const uint32_t col,const char *var_name, const char *var_type, const char *var_value,const uint32_t const_col)
    : AstNode{line, col} ,name(var_name), type(var_type), value(var_value),col(const_col){}

// TODO: You may use code snippets in AstDumper.cpp
void VariableNode::print() {}

void VariableNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    if (value != NULL){
        AstNode *CONST;
        //printf ("%s", value);
        std::string str = value;
        if (str == "-1.0"){
            str = "-1.000000";
        }
        const char *v = str.c_str();
        CONST = new ConstantValueNode(location.line, col,v);
        CONST->accept(p_visitor);
    }
     // TODO
}
