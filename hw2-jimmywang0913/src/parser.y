%{
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


extern int32_t line_num;   /* declared in scanner.l */
extern char buffer[512];  /* declared in scanner.l */
extern FILE *yyin;        /* declared by lex */
extern char *yytext;      /* declared by lex */

extern int yylex(void); 
static void yyerror(const char *msg);
%}

%union {
    float 	floatval;
    int 	intval;
    char	*str;

}

%token id KWarray KWbegin KWboolean KWdef KWdo KWelse KWend KWfalse KWfor KWif KWof KWprint KWread KWreal KWstring KWthen KWto KWtrue KWreturn KWinteger KWvar KWwhile mod aaa bbb ccc ddd;
%token integer;
%token floatt;
%token string scientific;
%token and or not;
%left '+' '-';
%left '*' '/';
%right '=' '!';
%%
program : program_name variable_const_declaration func_define_declaration compound_statement KWend ;
variable_const_declaration : | vcd2;
vcd2 : variable_declaration
	| const_declaration 
	| vcd2 variable_declaration 
	| vcd2 const_declaration;
func_define_declaration : | func_declaration;
variable_declaration : KWvar id_list ':' variable_type ';'
	| array_declaration;
array_declaration : KWvar id_list ':' KWarray integer KWof array_declaration2 variable_type ';';
array_declaration2 : | array_declaration3;
array_declaration3 : KWarray integer KWof | array_declaration3 KWarray integer KWof;
variable_type : KWboolean | KWreal | KWstring | KWinteger;
const_declaration : KWvar id_list ':' datatype ';';
datatype : integer | floatt | string | KWtrue | KWfalse | scientific;
program_name: id ';';

func_declaration : functiond 
	| func_declaration functiond;
functiond : id '(' argument ')' ':' variable_type functiond2
	| id '(' argument ')' functiond2;
functiond2 : ';'
	| compound_statement KWend;
argument : | argument2 | argument ';' argument2;
argument2 : id_list ':' variable_type;
id_list : id | id_list ',' id;
statement : | statement2;
statement2 :  compound_statement
	| simple_statement
	| conditional_statement
	| while_statement
	| for_statement
	| return_statement
	| procedure_statement
	| statement2 compound_statement 
	| statement2 simple_statement 
	| statement2 conditional_statement
	| statement2 while_statement 
	| statement2 for_statement
	| statement2 return_statement
	| statement2 procedure_statement;
compound_statement : KWbegin variable_const_declaration statement KWend ;
simple_statement : variable_reference aaa expression ';'
	| KWprint variable_reference ';'
	| KWprint expression ';'
	| KWread variable_reference ';' ;
variable_reference : id | array;
array : id array2;
array2 :'[' expression ']' | array2 '[' expression ']' ;
conditional_statement : KWif expression KWthen compound_statement KWelse compound_statement KWend KWif
	| KWif expression KWthen compound_statement KWend KWif;
while_statement : KWwhile expression KWdo compound_statement KWend KWdo;
for_statement : KWfor id aaa integer KWto integer KWdo compound_statement KWend KWdo;
return_statement : KWreturn expression ';';
procedure_statement : id '(' expression_list ')' ';' ;
expression_list : | expression_list1;
expression_list1 : expression 
	| expression_list1 ',' expression ;
expression : expression_val 
	| '(' expression ')'
	| expression operator expression_val
	| id '(' expression_list ')';
expression_val : expression_val2
	| '-' expression_val2
	| not expression_val2;
expression_val2 : | integer | floatt | id | '(' expression ')' | string | id '[' expression ']'| scientific | id '(' expression ')';
operator : negative | '*' | '/' | mod | '+' | '<' | bbb | ccc | ddd | '>' | '=' | and | or | nott ;
nott : not | nott not;
negative : '-' | negative '-';

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
    if (argc != 2) {
        fprintf(stderr, "Usage: ./parser <filename>\n");
        exit(-1);
    }

    yyin = fopen(argv[1], "r");
    assert(yyin != NULL && "fopen() fails.");

    yyparse();

    printf("\n"
           "|--------------------------------|\n"
           "|  There is no syntactic error!  |\n"
           "|--------------------------------|\n");
    return 0;
}
