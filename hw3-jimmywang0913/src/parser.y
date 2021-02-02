%{
#include "AST/ast.hpp"
#include "AST/program.hpp"
#include "AST/decl.hpp"
#include "AST/variable.hpp"
#include "AST/ConstantValue.hpp"
#include "AST/function.hpp"
#include "AST/CompoundStatement.hpp"
#include "AST/print.hpp"
#include "AST/BinaryOperator.hpp"
#include "AST/UnaryOperator.hpp"
#include "AST/FunctionInvocation.hpp"
#include "AST/VariableReference.hpp"
#include "AST/assignment.hpp"
#include "AST/read.hpp"
#include "AST/if.hpp"
#include "AST/while.hpp"
#include "AST/for.hpp"
#include "AST/return.hpp"
#include "AST/AstDumper.hpp"
#include "visitor/AstNodeVisitor.hpp"
#include "AST/expression.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>
using namespace std;
extern "C" int yylex();
extern "C" int yyparse();

#define YYLTYPE yyltype

typedef struct YYLTYPE {
    uint32_t first_line;
    uint32_t first_column;
    uint32_t last_line;
    uint32_t last_column;
} yyltype;

/* Declared by scanner.l */
extern uint32_t line_num;
extern char buffer[512];
extern FILE *yyin;
extern char *yytext;
/* End */

static AstNode *root;
static AstDumper vs;
int const_decl_state;
int arr_or_not;
char func[512];
std::string arr;
extern "C" int yylex(void);
static void yyerror(const char *msg);
extern int yylex_destroy(void);
%}
%code requires {
    class AstNode;
}

    /* For yylval */
%union {
    /* basic semantic value */
    char *identifier;
	const char 				*constid;
    AstNode *                                node;
    ProgramNode*                             prog;
    DeclNode*                                decl;
    FunctionNode*                            func;
    //ExpressionNode*                          Express;
    ConstantValueNode*                       Const;
    CompoundStatementNode*                   compound;
    VariableReferenceNode*                   vari;
    IfNode*                                  ifnode;
    ReturnNode*				return_node;
    WhileNode*					while_node;
    ForNode*					for_node;
    std::vector<AstNode *>*                	node_list;
    std::vector<const char *> 			*id_list;
    };

%type <identifier> ProgramName ID REAL_LITERAL
%type <constid>	ScalarType Type NegOrNot MINUS LiteralConstant INT_LITERAL STRING_LITERAL StringAndBoolean ArrType FunctionName ReturnType
%type <prog>                        Program
%type <compound>                    CompoundStatement ElseOrNot
%type <node_list>        		Declarations DeclarationList ArrRefList ArrRefs
%type <decl>                        Declaration FormalArg
%type <node_list>    			Functions FunctionList StatementList Statements 
%type <func>                        FunctionDeclaration Function FunctionDefinition
%type <ifnode>				Condition
%type <return_node>			Return
%type <while_node>			While
%type <for_node>			For
%type <node_list>                   ExpressionList Expressions FormalArgs FormalArgList
%type <id_list>                     IdList          
%type <node>				Statement IntegerAndReal Simple
%type <node>                 Expression StringAndBooleans IntegerAndReals FunctionInvocation VariableReference FunctionCall
    /* Delimiter */
%token COMMA SEMICOLON COLON
%token L_PARENTHESIS R_PARENTHESIS
%token L_BRACKET R_BRACKET

    /* Operator */
%token ASSIGN
%left OR
%left AND
%right NOT
%left LESS LESS_OR_EQUAL EQUAL GREATER GREATER_OR_EQUAL NOT_EQUAL
%left PLUS MINUS
%left MULTIPLY DIVIDE MOD
%right UNARY_MINUS

    /* Keyword */
%token ARRAY 
%token <constid> BOOLEAN INTEGER REAL STRING
%token END BEGIN_ /* Use BEGIN_ since BEGIN is a keyword in lex */
%token DO ELSE FOR IF THEN WHILE
%token DEF OF TO RETURN VAR
%token <constid> FALSE TRUE
%token PRINT READ
//%token <std::vector<DeclNode *> *> DeclarationList
//%token <std::vector<FunctionNode *> *> FunctionList
    /* Identifier */
%token ID

    /* Literal */
%token INT_LITERAL
%token REAL_LITERAL
%token STRING_LITERAL

%%
    /*
       Program Units
                     */

Program:
    ProgramName SEMICOLON
    /* ProgramBody */
    DeclarationList FunctionList CompoundStatement //DeclarationList FunctionList
    /* End of ProgramBody */
    END {
        root = new ProgramNode(@1.first_line, @1.first_column,
                               $1, "void", $3, $4, $5);

        free($1);
    }
;

ProgramName:
    ID{
    	$$=$1;
    }
;

DeclarationList:
    Epsilon{$$=NULL;}
    |
    Declarations{$$=$1;}
;

Declarations:
    Declaration{$$= new std::vector<AstNode*>;$$->push_back($1);}
    |
    Declarations Declaration{$$=$1;$1->push_back($2);}
;

FunctionList:
    Epsilon{$$=NULL;}
    |
    Functions{$$=$1;}
;

Functions:
    Function{$$= new std::vector<AstNode *>;$$->push_back($1);}
    |
    Functions Function{$$=$1;$1->push_back($2);}
    
;

Function:
    FunctionDeclaration{$$=$1;}
    |
    FunctionDefinition{$$=$1;}
;

FunctionDeclaration:
    FunctionName L_PARENTHESIS FormalArgList R_PARENTHESIS ReturnType SEMICOLON{size_t l1 = strlen($1), l2 = strlen($5); char *str3 = new char[514]; strcpy(str3, $1); str3[l1] = ' '; strcpy(str3+l1+1, $5); str3[l1+l2+1] = 0;$$ = new FunctionNode(@1.first_line, @1.first_column,$1,$3,$5,str3);}
;

FunctionDefinition:
    FunctionName L_PARENTHESIS FormalArgList R_PARENTHESIS ReturnType
    CompoundStatement
    END{size_t l1 = strlen($1), l2 = strlen($5); char *str4 = new char[514]; strcpy(str4, $1); str4[l1] = ' '; strcpy(str4+l1+1, $5); str4[l1+l2+1] = 0;$$ = new FunctionNode(@1.first_line, @1.first_column,$1,$3,$5,$6,str4);}
;

FunctionName:
    ID{$$=$1;}
;

FormalArgList:
    Epsilon{$$=NULL;}
    |
    FormalArgs{$$=$1;}
;

FormalArgs:
    FormalArg{$$= new std::vector<AstNode *>(); $$->push_back($1);}
    |
    FormalArgs SEMICOLON FormalArg{$1->push_back($3);$$=$1;}
;

FormalArg:
    IdList COLON Type{$$ = new DeclNode(@1.first_line, @1.first_column,@1.first_line, @1.first_column,$1,$3);}
;

IdList:
    ID{$$= new std::vector<const char *>;$$->push_back($1);}
    |
    IdList COMMA ID{$1->push_back($3);$$=$1;}
;

ReturnType:
    COLON ScalarType{$$=$2;}
    |
    Epsilon{$$="void";}
;

    /*
       Data Types and Declarations
                                   */

Declaration:
    VAR IdList COLON Type SEMICOLON{$$=new DeclNode(@1.first_line, @1.first_column,@2.first_line, @2.first_column,$2,$4);}
    |
    VAR IdList COLON LiteralConstant SEMICOLON{$$= new DeclNode(@1.first_line, @1.first_column,@2.first_line, @2.first_column,@3.first_column+2,$2,$4,const_decl_state);}
;

Type:
    ScalarType{$$= $1;arr_or_not=0;}
    |
    ArrType{$$=$1;arr_or_not=1;}
;

ScalarType:
    INTEGER{$$=$1;}
    |
    REAL{$$=$1;}
    |
    STRING{$$=$1;}
    |
    BOOLEAN{$$=$1;}
;

ArrType:
    ArrDecl ScalarType{ size_t l1 = strlen($2), l2 = strlen(arr.c_str()); char *str1 = new char[514]; strcpy(str1, $2); str1[l1] = ' '; strcpy(str1+l1+1, arr.c_str()); str1[l1+l2+1] = 0; $$ = str1;}
;

ArrDecl:
    ARRAY INT_LITERAL OF{arr.clear();arr += "[";arr += $2;arr += "]";}
    |
    ArrDecl ARRAY INT_LITERAL OF{arr += "[";arr += $3;arr += "]";}
;

LiteralConstant:
    NegOrNot INT_LITERAL{$$=$2;const_decl_state=1;}
    |
    NegOrNot REAL_LITERAL{size_t l1 = strlen($1), l2 = strlen($2); char *str2 = new char[514]; strcpy(str2, $1);  strcpy(str2+l1, $2); str2[l1+l2] = 0; $$=str2;const_decl_state=2;}
    |
    StringAndBoolean{$$=$1;}
;

NegOrNot:
    Epsilon{$$=NULL;}
    |
    MINUS %prec UNARY_MINUS{$$=$1;}
;

StringAndBoolean:
    STRING_LITERAL{$$=$1;const_decl_state=3;}
    |
    TRUE{$$=$1;const_decl_state=4;}
    |
    FALSE{$$=$1;const_decl_state=4;}
;
StringAndBooleans:
    STRING_LITERAL{size_t l1 = strlen($1); char *str4 = new char[514];strcpy(str4, $1+1);str4[l1-2]=0;$$= new ConstantValueNode(@1.first_line, @1.first_column,str4);}
    |
    TRUE{$$= new ConstantValueNode(@1.first_line, @1.first_column,$1);}
    |
    FALSE{$$= new ConstantValueNode(@1.first_line, @1.first_column,$1);}
;

IntegerAndReal:
    INT_LITERAL{$$=$1;}
    |
    REAL_LITERAL{$$=$1;}
;
IntegerAndReals:
    INT_LITERAL{$$= new ConstantValueNode(@1.first_line, @1.first_column,$1);}
    |
    REAL_LITERAL{size_t l1 = strlen($1); char *str5 = new char[514];strcpy(str5, $1);int flag =0;
    for (int i=0;i<l1;i++){
    if (flag==0){
    	if (str5[i]=='.'){
    		for (int j=0;j<6;j++){
    			if (str5[j+1+i]==0){
    				str5[j+1+i]='0';
    				str5[j+2+i]=0;
    			}
    		}
    		flag=1;
    	}
    }
    	
    }$$= new ConstantValueNode(@1.first_line, @1.first_column,str5);}
;
    /*
       Statements
                  */

Statement:
    CompoundStatement{$$=$1;}
    |
    Simple{$$=$1;}
    |
    Condition{$$=$1;}
    |
    While{$$=$1;}
    |
    For{$$=$1;}
    |
    Return{$$=$1;}
    |
    FunctionCall{$$=$1;}
;

CompoundStatement:
    BEGIN_
    DeclarationList
    StatementList
    END{$$= new CompoundStatementNode( @1.first_line, @1.first_column, $2, $3);}
;

Simple:
    VariableReference ASSIGN Expression SEMICOLON{$$=new AssignmentNode(@2.first_line, @2.first_column, $1, $3);}
    |
    PRINT Expression SEMICOLON{$$=new PrintNode(@1.first_line, @1.first_column,$2);}
    |
    READ VariableReference SEMICOLON{$$= new ReadNode(@1.first_line, @1.first_column,$2); }
;

VariableReference:
    ID ArrRefList{$$= new VariableReferenceNode(@1.first_line, @1.first_column,$1,$2);}
;

ArrRefList:
    Epsilon{$$=NULL;}
    |
    ArrRefs{$$= $1;}
;

ArrRefs:
    L_BRACKET Expression R_BRACKET{$$= new std::vector<AstNode *>();$$->push_back($2);}
    |
    ArrRefs L_BRACKET Expression R_BRACKET{$1->push_back($3);$$=$1;}
;

Condition:
    IF Expression THEN
    CompoundStatement
    ElseOrNot
    END IF{$$= new IfNode(@1.first_line, @1.first_column,$2, $4, $5);}
;

ElseOrNot:
    ELSE
    CompoundStatement{$$=$2;}
    |
    Epsilon{$$=NULL;}
;

While:
    WHILE Expression DO
    CompoundStatement
    END DO{$$= new WhileNode(@1.first_line, @1.first_column,$2,$4);}
;

For:
    FOR ID ASSIGN INT_LITERAL TO INT_LITERAL DO
    CompoundStatement
    END DO{DeclNode *tmp = new DeclNode(@2.first_line, @2.first_column,$2);
    VariableReferenceNode *tmp3 = new VariableReferenceNode(@2.first_line, @2.first_column,$2);
    ConstantValueNode *tmp4 = new ConstantValueNode(@3.first_line, @4.first_column,$4);
    ConstantValueNode *tmp5= new ConstantValueNode(@6.first_line, @6.first_column,$6);
    AssignmentNode *tmp1= new AssignmentNode(@3.first_line, @3.first_column,tmp3,tmp4);
    $$= new ForNode(@1.first_line, @1.first_column,tmp,tmp1,tmp5,$8);}
;

Return:
    RETURN Expression SEMICOLON{$$= new ReturnNode(@1.first_line, @1.first_column,$2);}
;

FunctionCall:
    FunctionInvocation SEMICOLON{
    	$$=$1;
    }
;

FunctionInvocation:
    ID L_PARENTHESIS ExpressionList R_PARENTHESIS{
    	$$= new FunctionInvocationNode(@1.first_line, @1.first_column,$1, $3);
    }
;

ExpressionList:
    Epsilon{$$=NULL;}
    |
    Expressions{$$=$1;}
;

Expressions:
    Expression{$$= new std::vector<AstNode *>();$$->push_back($1);}
    |
    Expressions COMMA Expression{$1->push_back($3);$$=$1;}
;

StatementList:
    Epsilon{$$=NULL;}
    |
    Statements{$$=$1;}
;

Statements:
    Statement{$$= new std::vector<AstNode *>;$$->push_back($1);}
    |
    Statements Statement{$1->push_back($2);$$=$1;}
;

Expression:
    L_PARENTHESIS Expression R_PARENTHESIS{$$=$2;}
    |
    MINUS Expression %prec UNARY_MINUS{$$= new UnaryOperatorNode(@1.first_line, @1.first_column, "neg",$2);}
    |
    Expression MULTIPLY Expression{$$= new BinaryOperatorNode(@2.first_line, @2.first_column,"*",$1, $3);}
    |
    Expression DIVIDE Expression{$$= new BinaryOperatorNode(@2.first_line, @2.first_column,"/",$1, $3);}
    |
    Expression MOD Expression{$$= new BinaryOperatorNode(@2.first_line, @2.first_column,"mod",$1, $3);}
    |
    Expression PLUS Expression{$$= new BinaryOperatorNode(@2.first_line, @2.first_column,"+",$1, $3);}
    |
    Expression MINUS Expression{$$= new BinaryOperatorNode(@2.first_line, @2.first_column,"-",$1, $3);}
    |
    Expression LESS Expression{$$= new BinaryOperatorNode(@2.first_line, @2.first_column,"<",$1, $3);}
    |
    Expression LESS_OR_EQUAL Expression{$$= new BinaryOperatorNode(@2.first_line, @2.first_column,"<=",$1, $3);}
    |
    Expression GREATER Expression{$$= new BinaryOperatorNode(@2.first_line, @2.first_column,">",$1, $3);}
    |
    Expression GREATER_OR_EQUAL Expression{$$= new BinaryOperatorNode(@2.first_line, @2.first_column,">=",$1, $3);}
    |
    Expression EQUAL Expression{$$= new BinaryOperatorNode(@2.first_line, @2.first_column,"=",$1, $3);}
    |
    Expression NOT_EQUAL Expression{$$= new BinaryOperatorNode(@2.first_line, @2.first_column,"<>",$1, $3);}
    |
    NOT Expression{$$= new UnaryOperatorNode(@1.first_line, @1.first_column,"not",$2);}
    |
    Expression AND Expression{$$= new BinaryOperatorNode(@2.first_line, @2.first_column,"and",$1, $3);}
    |
    Expression OR Expression{$$= new BinaryOperatorNode(@2.first_line, @2.first_column,"or",$1, $3);}
    |
    IntegerAndReals{$$=$1;}
    |
    StringAndBooleans{$$=$1;}
    |
    VariableReference{$$=$1;}
    |
    FunctionInvocation{$$=$1;}
;

    /*
       misc
            */
Epsilon:
;
%%

void yyerror(const char *msg) {
    fprintf(stderr,
            "\n"
            "|-----------------------------------------------------------------"
            "---------\n"
            "| Error found in Line #%d: %s\n"
            "|\n"
            "| Unmatched token: %s\n"
            "|-----------------------------------------------------------------"
            "---------\n",
            line_num, buffer, yytext);
    exit(-1);
}

int main(int argc, const char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: ./parser <filename> [--dump-ast]\n");
        exit(-1);
    }

    yyin = fopen(argv[1], "r");
    assert(yyin != NULL && "fopen() fails.");

    yyparse();

    if (argc >= 3 && strcmp(argv[2], "--dump-ast") == 0) {
        root->accept(vs);
    }
    
    printf("\n"
           "|--------------------------------|\n"
           "|  There is no syntactic error!  |\n"
           "|--------------------------------|\n");

    delete root;
    fclose(yyin);
    yylex_destroy();
    return 0;
}
