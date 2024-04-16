//Project 2 - Tree Builder Compiler
//Wyatt Shinkle and Zach Naymik
//CS4100

//this is template code for now

%{
#include <stdio.h>
%}

%token INTEGER

%%

program:
    program statement '\n'
    |
    ;

statement:
    'print' INTEGER { printf("%d\n", $2); }
    ;

%%

int yyerror(char *s) {
    fprintf(stderr, "error: %s\n", s);
    return 0;
}

int main(void) {
    yyparse();
    return 0;
}