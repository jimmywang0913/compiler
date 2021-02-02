%{
#include <stdint.h>

#define LIST                strCat(yytext)
#define LLIST		    strCatt(yytext)
#define TOKEN(t)            { LIST; if (opt_tok) printf("<%s>\n", #t); }
#define TOKEN_CHAR(t)       { LIST; if (opt_tok) printf("<%c>\n", (t)); }
#define TOKEN_STRING(t, s)  { LIST; if (opt_tok) printf("<%s: %s>\n", #t, (s)); }
#define MAX_LINE_LENG       512

uint32_t line_num = 1;
char buffer[MAX_LINE_LENG];
char buffer2[MAX_LINE_LENG];

static int32_t opt_src = 1;
static int32_t opt_tok = 1;
static char *buffer_ptr = buffer;
static char *buffer_ptr2 = buffer2;

static void strCat(const char *Text);
static void strCatt(const char *Text2);
int count=0;
int i=0;
%}
digit	[0-9]
digitwithout0	[1-9]
letter	[a-zA-Z]
%Start	COMMENT COMMENT2 str COMMENTT RE REE LAST LAST2 LAST3 LAST4
%%

<COMMENT2>.*	LIST;
<COMMENT2>\n	{if (opt_src) {
        printf("%d: %s\n", line_num, buffer);
    }
    ++line_num; 
    buffer[0] = '\0';
    buffer_ptr = buffer;BEGIN 0;}
<LAST>[^\"][^\"]*	{LLIST;LIST;}
<LAST>[\"]	{BEGIN LAST4;LIST;}
<LAST4>[\"]	{LLIST;BEGIN LAST;LIST;}
<LAST4>[^\"]	{yyless(0);if (opt_tok) {
	printf ("<string: ");
        printf("%s", buffer2);printf (">\n");
    }
    buffer2[0] = '\0';
    buffer_ptr2 = buffer2;BEGIN INITIAL;
			}
<COMMENT>"*/"	{LIST;BEGIN INITIAL;}
<COMMENT>.*"*/"	{LIST;BEGIN INITIAL;}
<COMMENTT>[\/]	{LIST;BEGIN INITIAL;}
<COMMENTT>\n	{if (opt_src) {
        printf("%d: %s\n", line_num, buffer);
    }
    ++line_num;
    buffer[0] = '\0';
    buffer_ptr = buffer;BEGIN COMMENT;}
<COMMENTT>[^\/]	{LIST;BEGIN COMMENT;}
<COMMENT>[^\*\n]*	{LIST;BEGIN COMMENTT;}
<COMMENT>\n	{if (opt_src) {
        printf("%d: %s\n", line_num, buffer);
    }
    ++line_num;
    buffer[0] = '\0';
    buffer_ptr = buffer;}
<INITIAL>"/*"	{LIST;BEGIN COMMENT;}

\n	{
    if (opt_src) {
        printf("%d: %s\n", line_num, buffer);
    }
    ++line_num; 
    buffer[0] = '\0';
    buffer_ptr = buffer;
}
"//&S+".*	{LIST;opt_src=1;}
"//&S-".*	{LIST;opt_src=0;}
"//&T+".*	{LIST;opt_tok=1;}
"//&T-".*	{LIST;opt_tok=0;}
" "	LIST;
<INITIAL>"//"	{LIST;BEGIN COMMENT2;}


<INITIAL>[\"]	{BEGIN LAST;LIST;}

<INITIAL>array	TOKEN(KWarray);
<INITIAL>begin	TOKEN(KWbegin);
<INITIAL>boolean	TOKEN(KWboolean);
<INITIAL>def	TOKEN(KWdef);
<INITIAL>do	TOKEN(KWdo);
<INITIAL>else	TOKEN(KWelse);
<INITIAL>end	TOKEN(KWend);
<INITIAL>false	TOKEN(KWfalse);
<INITIAL>for	TOKEN(KWfor);
<INITIAL>integer	TOKEN(KWinteger);
<INITIAL>if	TOKEN(KWif);
<INITIAL>of	TOKEN(KWof);
<INITIAL>print	TOKEN(KWprint);
<INITIAL>read	TOKEN(KWread);
<INITIAL>real	TOKEN(KWreal);
<INITIAL>string	TOKEN(KWstring);
<INITIAL>then	TOKEN(KWthen);
<INITIAL>to	TOKEN(KWto);
<INITIAL>true	TOKEN(KWtrue);
<INITIAL>return	TOKEN(KWreturn);
<INITIAL>var	TOKEN(KWvar);
<INITIAL>while	TOKEN(KWwhile);
<INITIAL>mod	TOKEN(mod);
<INITIAL>and	TOKEN(and);
<INITIAL>or	TOKEN(or);
<INITIAL>not	TOKEN(not);
<INITIAL>0	TOKEN_STRING(integer,yytext);
<INITIAL>{digitwithout0}{digit}*	TOKEN_STRING(integer,yytext);
<INITIAL>000*	TOKEN_STRING(oct_integer,yytext);
<INITIAL>0{digit}{digit}*	TOKEN_STRING(oct_integer,yytext);
<INITIAL>0{digit}{digitwithout0}\.	{yyless(2);i=0;count=0;TOKEN_STRING(oct_integer,yytext);
							}
<INITIAL>[a-zA-Z][a-zA-Z0-9]*	TOKEN_STRING(id,yytext);
<INITIAL>"("	{ TOKEN_CHAR('('); }
<INITIAL>")"	{ TOKEN_CHAR(')'); }
<INITIAL>";"	TOKEN_CHAR(';');
<INITIAL>","	TOKEN_CHAR(',');
<INITIAL>":"	TOKEN_CHAR(':');
<INITIAL>"["	TOKEN_CHAR('[');
<INITIAL>"]"	TOKEN_CHAR(']');
<INITIAL>"+"	TOKEN_CHAR('+');
<INITIAL>"-"	TOKEN_CHAR('-');
<INITIAL>"*"	TOKEN_CHAR('*');
<INITIAL>"/"	TOKEN_CHAR('/');
<INITIAL>"<"	TOKEN_CHAR('<');
<INITIAL>">"	TOKEN_CHAR('>');
<INITIAL>"="	TOKEN_CHAR('=');
<INITIAL>":="	TOKEN(:=);
<INITIAL>"<="	TOKEN(<=);
<INITIAL>"<>"	TOKEN(<>);
<INITIAL>">="	TOKEN(>=);
<INITIAL>{digitwithout0}{digit}*\.{digit}*{digitwithout0}	TOKEN_STRING(float,yytext);
<INITIAL>"0.0"	TOKEN_STRING(float,yytext);
<INITIAL>{digitwithout0}{digit}*[Ee]{digitwithout0}{digit}*	TOKEN_STRING(scientific,yytext);
<INITIAL>{digitwithout0}{digit}*[Ee]0{digitwithout0}{digit}*	TOKEN_STRING(scientific,yytext);
<INITIAL>{digitwithout0}{digit}*[Ee]0	TOKEN_STRING(scientific,yytext);
<INITIAL>{digitwithout0}{digit}*[Ee][\+\-]{digitwithout0}{digit}*	TOKEN_STRING(scientific,yytext);
<INITIAL>{digitwithout0}{digit}*[Ee]{digitwithout0}{digit}*\.{digit}*{digitwithout0}	TOKEN_STRING(scientific,yytext);
<INITIAL>{digitwithout0}{digit}*[Ee][\+\-]{digitwithout0}{digit}*\.{digit}*{digitwithout0}	TOKEN_STRING(scientific,yytext);
<INITIAL>{digitwithout0}{digit}*[Ee]"0.0"	TOKEN_STRING(scientific,yytext);
<INITIAL>{digitwithout0}{digit}*[Ee][\+\-]"0.0"	TOKEN_STRING(scientific,yytext);
<INITIAL>{digitwithout0}{digit}*\.{digit}*{digitwithout0}[Ee]{digitwithout0}{digit}*	TOKEN_STRING(scientific,yytext);
<INITIAL>000*{digitwithout0}[Ee]	{i=0;count=0;yyless(2);count=0;i=0;TOKEN_STRING(oct_integer,yytext);
									}
<INITIAL>{digitwithout0}{digit}*\.{digit}*{digitwithout0}[Ee][\+\-]{digitwithout0}{digit}*	TOKEN_STRING(scientific,yytext);
<INITIAL>{digitwithout0}{digit}*\.{digit}*{digitwithout0}[Ee]{digitwithout0}{digit}*\.{digit}*{digitwithout0}	TOKEN_STRING(scientific,yytext);
<INITIAL>{digitwithout0}{digit}*\.{digit}*{digitwithout0}[Ee][\+\-]{digitwithout0}{digit}*\.{digit}*{digitwithout0}	TOKEN_STRING(scientific,yytext);
<INITIAL>{digitwithout0}{digit}*\.{digit}*{digitwithout0}[Ee]"0.0"	TOKEN_STRING(scientific,yytext);
<INITIAL>{digitwithout0}{digit}*\.{digit}*{digitwithout0}[Ee][\+\-]"0.0"	TOKEN_STRING(scientific,yytext);
<INITIAL>{digitwithout0}{digit}*\.0[Ee]0	TOKEN_STRING(scientific,yytext);
<INITIAL>{digitwithout0}{digit}*\.0[Ee]0{digitwithout0}{digit}*	TOKEN_STRING(scientific,yytext);
<INITIAL>{digitwithout0}{digit}*\.0[Ee]{digitwithout0}{digit}*	TOKEN_STRING(scientific,yytext);
<INITIAL>"0.0"[Ee]{digitwithout0}{digit}*	TOKEN_STRING(scientific,yytext);
<INITIAL>"0.0"[Ee][\+\-]{digitwithout0}{digit}*
<INITIAL>"0.0"[Ee]{digitwithout0}{digit}*\.{digit}*{digitwithout0}	TOKEN_STRING(scientific,yytext);
<INITIAL>"0.0"[Ee][\+\-]{digitwithout0}{digit}*\.{digit}*{digitwithout0}	TOKEN_STRING(scientific,yytext);
<INITIAL>"0.0"[Ee]"0.0"	TOKEN_STRING(scientific,yytext);
<INITIAL>"0.0"[Ee][\+\-]"0.0"	TOKEN_STRING(scientific,yytext);
<INITIAL>\t	LIST;
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
static void strCatt(const char *Text2) {
    while (*Text2) {
        *buffer_ptr2++ = *Text2++;
    }
    *buffer_ptr2 = '\0';
}


int main(int argc, const char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: ./scanner [filename]\n");
        exit(-1);
    }

    yyin = fopen(argv[1], "r");
    if (yyin == NULL) {
        fprintf(stderr, "Open file error\n");
        exit(-1);
    }

    yylex();

    return 0;
}
