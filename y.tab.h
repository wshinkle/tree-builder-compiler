/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    IDENTIFIER = 258,              /* IDENTIFIER  */
    STRING = 259,                  /* STRING  */
    INTEGER = 260,                 /* INTEGER  */
    BUILDNODE = 261,               /* BUILDNODE  */
    FOR = 262,                     /* FOR  */
    IN = 263,                      /* IN  */
    LBRACKET = 264,                /* LBRACKET  */
    RBRACKET = 265,                /* RBRACKET  */
    LBRACE = 266,                  /* LBRACE  */
    RBRACE = 267,                  /* RBRACE  */
    SEMICOLON = 268,               /* SEMICOLON  */
    COLON = 269,                   /* COLON  */
    EQUALS = 270,                  /* EQUALS  */
    PLUS = 271,                    /* PLUS  */
    NAME = 272,                    /* NAME  */
    WEIGHT = 273,                  /* WEIGHT  */
    ISACHILDOF = 274,              /* ISACHILDOF  */
    ERROR = 275                    /* ERROR  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define IDENTIFIER 258
#define STRING 259
#define INTEGER 260
#define BUILDNODE 261
#define FOR 262
#define IN 263
#define LBRACKET 264
#define RBRACKET 265
#define LBRACE 266
#define RBRACE 267
#define SEMICOLON 268
#define COLON 269
#define EQUALS 270
#define PLUS 271
#define NAME 272
#define WEIGHT 273
#define ISACHILDOF 274
#define ERROR 275

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 14 "tree_builder.y"

    char* string;
    int integer;
    class TreeNode* node;

#line 113 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
