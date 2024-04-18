/*
Authors: Wyatt Shinkle and Zach Naymik
Date: 4/18/2024
Description: This file contains the lex file for the parser. It defines the tokens that the parser will use to parse the input file.
*/


%{
#include "y.tab.h"
#include <stdlib.h>
%}

%option nounput

%%

"buildnode"    { return BUILDNODE; }
"for"          { return FOR; }
"in"           { return IN; }
"["            { return LBRACKET; }
"]"            { return RBRACKET; }
"{"            { return LBRACE; }
"}"            { return RBRACE; }
";"            { return SEMICOLON; }
":"            { return COLON; } 
"="            { return EQUALS; }
"+"            { return PLUS; }
"name"         { return NAME; }
"weight"       { return WEIGHT; }
"isachildof"   { return ISACHILDOF; }
","            { return COMMA; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.string = strdup(yytext);
                         //printf("identifier: %s\n", yylval.string);
                         return IDENTIFIER; }
[0-9]+         { yylval.integer = atoi(yytext); 
                  //printf("integer: %d\n", yylval.integer);
                  return INTEGER; }
\"[^"]*\"      { yylval.string = strndup(yytext + 1, yyleng - 2); return STRING; }\
[ \t\n]        { /* ignore whitespace */ }
.              { return ERROR;}

%%
int yywrap() {
   return 1;
}
