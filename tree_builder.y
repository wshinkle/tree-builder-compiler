%{
#include <stdio.h>
#include <stdlib.h>
#include "parse_tree.h"
#include "tree_node.h"
extern int yylex();
extern int yyparse();

extern FILE* yyin;
void yyerror(const char* s);
extern ParseTree tree;
%}

%union {
    char* string;
    int integer;
    TreeNode* node;
}

%type <string> name isachildof
%type <integer> weight
%type <node> node
%token <string> IDENTIFIER STRING
%token <integer> INTEGER
%token BUILDNODE FOR IN LBRACKET RBRACKET LBRACE RBRACE SEMICOLON COLON EQUALS PLUS NAME WEIGHT ISACHILDOF ERROR

%%

program:
    | program statement
    ;

statement:
    buildnode_statement
    | for_statement
    ;

buildnode_statement:
    BUILDNODE LBRACE node RBRACE SEMICOLON
    {
        tree.addNode($3);

    }
    ;

node:
    name weight
    {$$ = new TreeNode($1, $2);}
    | name weight isachildof
    {$$ = new TreeNode($1, $2, $3);}
    ;

name:
    NAME EQUALS STRING SEMICOLON
    {$$ = $3;}
    ;
weight:
    WEIGHT EQUALS INTEGER SEMICOLON
    {$$ = $3;}
    ;
isachildof:
    ISACHILDOF STRING SEMICOLON
    {$$ = $2;}
    ;



for_statement:
    FOR IDENTIFIER IN LBRACKET INTEGER COLON INTEGER RBRACKET LBRACE statements RBRACE SEMICOLON
    {

    }
    ;


statements:
    | statements statement
    ;

%%
#include "tree_node.h"
void yyerror(const char* s) {
    fprintf(stderr, "Parse error: %s\n", s);
    exit(1);
}

int main(int argc, const char * argv[]) {
    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            fprintf(stderr, "Cannot open file: %s\n", argv[1]);
            return 1;
        }
        yyin = file;
    }

    yyparse();

    if (yyin != stdin) {
        fclose(yyin);
    }
    tree.printTree();
    return 0;
}