/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    id = 258,
    KWarray = 259,
    KWbegin = 260,
    KWboolean = 261,
    KWdef = 262,
    KWdo = 263,
    KWelse = 264,
    KWend = 265,
    KWfalse = 266,
    KWfor = 267,
    KWif = 268,
    KWof = 269,
    KWprint = 270,
    KWread = 271,
    KWreal = 272,
    KWstring = 273,
    KWthen = 274,
    KWto = 275,
    KWtrue = 276,
    KWreturn = 277,
    KWinteger = 278,
    KWvar = 279,
    KWwhile = 280,
    mod = 281,
    aaa = 282,
    bbb = 283,
    ccc = 284,
    ddd = 285,
    integer = 286,
    floatt = 287,
    string = 288,
    and = 289,
    or = 290,
    not = 291
  };
#endif
/* Tokens.  */
#define id 258
#define KWarray 259
#define KWbegin 260
#define KWboolean 261
#define KWdef 262
#define KWdo 263
#define KWelse 264
#define KWend 265
#define KWfalse 266
#define KWfor 267
#define KWif 268
#define KWof 269
#define KWprint 270
#define KWread 271
#define KWreal 272
#define KWstring 273
#define KWthen 274
#define KWto 275
#define KWtrue 276
#define KWreturn 277
#define KWinteger 278
#define KWvar 279
#define KWwhile 280
#define mod 281
#define aaa 282
#define bbb 283
#define ccc 284
#define ddd 285
#define integer 286
#define floatt 287
#define string 288
#define and 289
#define or 290
#define not 291

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 17 "parser.y" /* yacc.c:1909  */

    float 	floatval;
    int 	intval;
    char	*str;


#line 133 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
