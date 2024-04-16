%{
#include <stdio.h>
#include <stdlib.h>
#include "tree_node.h"
#include "parse_tree.h"
extern int yylex();
extern int yyparse();
extern FILE* yyin;
void yyerror(const char* s);
%}

%union {
    char* string;
    int integer;
}

%token <string> IDENTIFIER STRING
%token <integer> INTEGER
%token BUILDNODE FOR IN LBRACKET RBRACKET LBRACE RBRACE SEMICOLON COLON EQUALS PLUS NAME WEIGHT ISACHILDOF

%%

program:
    | program statement
    ;

statement:
    buildnode_statement
    | for_statement
    ;

buildnode_statement:
    BUILDNODE LBRACE buildnode_attributes RBRACE SEMICOLON
    ;

buildnode_attributes:
    | buildnode_attributes buildnode_attribute
    ;

buildnode_attribute:
    NAME EQUALS STRING SEMICOLON
    | WEIGHT EQUALS INTEGER SEMICOLON
    | ISACHILDOF EQUALS STRING SEMICOLON
    ;

for_statement:
    FOR IDENTIFIER IN LBRACKET range RBRACKET LBRACE statements RBRACE SEMICOLON
    ;

range:
    INTEGER COLON INTEGER
    ;

statements:
    | statements statement
    ;

%%

void yyerror(const char* s) {
    fprintf(stderr, "Parse error: %s\n", s);
    exit(1);
}

int main(void) {
    yyparse();
    return 0;
}