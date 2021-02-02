%{
#include <stdint.h>
//#include "y.tab.h"
#include "parser.h"

#define LIST                strCat(yytext)
#define TOKEN(t)            { LIST; if (opt_tok) {printf("<%s>\n", #t);} }
#define TOKEN_CHAR(t)       { LIST; if (opt_tok) {printf("<%c>\n", (t));}}
#define TOKEN_STRING(t, s)  { LIST; if (opt_tok) {printf("<%s: %s>\n", #t, (s));}}
#define MAX_LINE_LENG       512

uint32_t line_num = 1;
char buffer[MAX_LINE_LENG];

static int32_t opt_src = 1;
static int32_t opt_tok = 1;
static char string_literal[MAX_LINE_LENG];
static char *buffer_ptr = buffer;

static void strCat(const char *Text);

%}

integer 0|[1-9][0-9]*
float {integer}\.(0|[0-9]*[1-9])

%x CCOMMENT

%%
    /* Delimiter */
"," { TOKEN_CHAR(','); return (',');}
";" { TOKEN_CHAR(';'); return ';';}
":" { TOKEN_CHAR(':');return ':';}
"(" { TOKEN_CHAR('(');return '(';}
")" { TOKEN_CHAR(')'); return ')';}
"[" { TOKEN_CHAR('[');return '[';}
"]" { TOKEN_CHAR(']');return ']';}

    /* Operator */
"+"   { TOKEN_CHAR('+'); return '+'; }
"-"   { TOKEN_CHAR('-');return '-';}
"*"   { TOKEN_CHAR('*');return '*';}
"/"   { TOKEN_CHAR('/');return '/';}
"mod" { TOKEN(mod);return mod;}
":="  { TOKEN(:=); return aaa;}
"<"   { TOKEN_CHAR('<'); return '<';}
"<="  { TOKEN(<=); return bbb;}
"<>"  { TOKEN(<>); return ccc;}
">="  { TOKEN(>=); return ddd;}
">"   { TOKEN_CHAR('>'); return '>';}
"="   { TOKEN_CHAR('='); return '=';}
"and" { TOKEN(and);return and;}
"or"  { TOKEN(or);return or;}
"not" { TOKEN(not);return not;}

    /* Keyword */
"array"   { TOKEN(KWarray);return KWarray;}
"begin"   { TOKEN(KWbegin); return KWbegin;}
"boolean" { TOKEN(KWboolean);return KWboolean;}
"def"     { TOKEN(KWdef); return KWdef;}
"do"      { TOKEN(KWdo); return KWdo;}
"else"    { TOKEN(KWelse);return KWelse;}
"end"     { TOKEN(KWend);return KWend;}
"false"   { TOKEN(KWfalse);return KWfalse;}
"for"     { TOKEN(KWfor); return KWfor;}
"integer" { TOKEN(KWinteger); return KWinteger;}
"if"      { TOKEN(KWif);return KWif;}
"of"      { TOKEN(KWof);return KWof;}
"print"   { TOKEN(KWprint); return KWprint;}
"read"    { TOKEN(KWread); return KWread;}
"real"    { TOKEN(KWreal);return KWreal;}
"string"  { TOKEN(KWstring); return KWstring;}
"then"    { TOKEN(KWthen); return KWthen;}
"to"      { TOKEN(KWto); return KWto;}
"true"    { TOKEN(KWtrue); return KWtrue;}
"return"  { TOKEN(KWreturn);return KWreturn;}
"var"     { TOKEN(KWvar);return KWvar;}
"while"   { TOKEN(KWwhile);return KWwhile;}

    /* Identifier */
[a-zA-Z][a-zA-Z0-9]* { TOKEN_STRING(id, yytext);return id;}

    /* Integer (decimal/octal) */
{integer} { TOKEN_STRING(integer, yytext);return integer;}
0[0-7]+   { TOKEN_STRING(oct_integer, yytext);}

    /* Floating-Point */
{float} { TOKEN_STRING(float, yytext);return floatt;}

    /* Scientific Notation [Ee][+-]?[0-9]+ */
({integer}|{float})[Ee][+-]?({integer}) { TOKEN_STRING(scientific, yytext);return scientific;}

    /* String */
\"([^"\n]|\"\")*\" {
    char *yyt_ptr = yytext;
    char *str_ptr = string_literal;

    /* Condition guard of the end of string literal
       Check only when *yyt_ptr == '"' */
    while (*(yyt_ptr + 1) != '\0') {
        ++yyt_ptr;

        /* Handle the situation of double quotes */
        if (*yyt_ptr == '"' && *(yyt_ptr + 1) != '\0')
            *str_ptr++ = *yyt_ptr++;

        while (*yyt_ptr != '"')
            *str_ptr++ = *yyt_ptr++;
    }
    *str_ptr = '\0';
    TOKEN_STRING(string, string_literal);return string;
}
    /* Whitespace */
[ \t]+ { LIST; }

    /* Pseudocomment */
"//&"[ST][+-].* {
    LIST;
    char option = yytext[3];
    switch (option) {
    case 'S':
        opt_src = (yytext[4] == '+') ? 1 : 0;
        break;
    case 'T':
        opt_tok = (yytext[4] == '+') ? 1 : 0;
        break;
    }
}

    /* C++ Style Comment */
"//".* { LIST; }

    /* C Style Comment */
"/*"           { LIST; BEGIN(CCOMMENT); }
<CCOMMENT>"*/" { LIST; BEGIN(INITIAL); }
<CCOMMENT>.    { LIST; }

    /* Newline */
<INITIAL,CCOMMENT>\n {
    if (opt_src) {
        printf("%d: %s\n", line_num, buffer);
    }
    ++line_num;
    buffer[0] = '\0';
    buffer_ptr = buffer;
}

    /* Catch the character which is not accepted by rules above */
. {
    printf("Error at line %d: bad character \"%s\"\n", line_num, yytext);
    exit(-1);
}

%%

static void strCat(const char *Text) {
    while (*Text) {
        *buffer_ptr++ = *Text++;
    }
    *buffer_ptr = '\0';
}
