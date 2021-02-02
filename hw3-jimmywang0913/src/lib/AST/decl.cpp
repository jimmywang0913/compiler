#include "AST/decl.hpp"
#include "AST/variable.hpp"
#include <vector>
#include <cstring>

// TODO
DeclNode::DeclNode(const uint32_t line, const uint32_t col,const uint32_t v_line, const uint32_t v_col,
		std::vector<const char *> *dec_identifier,const char *dec_type)
    : AstNode{line, col},identifier{dec_identifier},vv_col(v_col),vv_line(v_line),cconst_col(0) {
    	type=dec_type;
		ttype=1;
    }

// TODO
DeclNode::DeclNode(const uint32_t line, const uint32_t col,const uint32_t v_line, const uint32_t v_col,const uint32_t const_col,
		std::vector<const char *> *dec_identifier, const char *const_constant,int tt)
    : AstNode{line, col} ,identifier{dec_identifier},vv_col(v_col),vv_line(v_line),cconst_col(const_col){
	constant=const_constant;
	ttype=2;
	ttt=tt;
    }
DeclNode::DeclNode(const uint32_t line, const uint32_t col,
		char *dec_identifier)
    : AstNode{line, col} ,vv_line(NULL),vv_col(NULL),cconst_col(NULL){
		idd=dec_identifier;
		ttype=3;
    }
// TODO: You may use code snippets in AstDumper.cpp
void DeclNode::print() {
	std::printf("declaration <line: %u,col: %u>", location.line, location.col);
}

void DeclNode::visitChildNodes(AstNodeVisitor &p_visitor) {
     // TODO
	 	int b=0;
		uint32_t bb=cconst_col;
	if (ttype == 1){
		int c=0;
		uint32_t cc=0;
		for (auto id : *identifier){
			c++;
			if (c==1){
				cc=vv_col;
			}
			AstNode *var;
			var= new  VariableNode(location.line, cc,id, type, NULL,bb);
			cc+=strlen(id);
			cc+=2;
			var->accept(p_visitor);
		}
		
	}
	if (ttype == 2){
		int c=0;
		uint32_t cc=0;
		for (auto id : *identifier){
			c++;
			if (c==1){
				cc=vv_col;
			}
			const uint32_t column=cc;
			AstNode *var;
			//printf ("%d\n", bb);
			if (ttt==1){
				var= new  VariableNode(location.line, column,id, "integer", constant,bb);
			}
			else if (ttt==2){
				var= new  VariableNode(location.line, column,id, "real", constant,bb);
			}
			else if (ttt==3){
				char *cpy = new char[strlen(constant)];
				strcpy(cpy,constant+1);
				cpy[strlen(constant)-2]=0;
				var= new  VariableNode(location.line, column,id, "string", cpy,bb);
			}
			else {
				var= new  VariableNode(location.line, column,id, "boolean", constant,bb);
			}
			cc+=strlen(id);
			cc+=2;
			var->accept(p_visitor);
		}
		
	}
	if (ttype == 3){
		AstNode *var;
		var= new  VariableNode(location.line, location.col,idd, "integer", NULL,bb);
		var->accept(p_visitor);
	}

}
