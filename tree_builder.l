%{
#include "y.tab.h"
#include <stdlib.h>
%}

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
[a-zA-Z_][a-zA-Z0-9_]* { yylval.string = strdup(yytext); return IDENTIFIER; }
[0-9]+         { yylval.string = strdup(yytext); return INTEGER; }
\"[^"]*\"      { yylval.string = strdup(yytext + 1, yyleng - 2); return STRING; }
[ \t\n]        { /* ignore whitespace */ }
.              { yyerror("Unknown character: %s", yytext); }

%%

int yywrap() {
    return 1;
}