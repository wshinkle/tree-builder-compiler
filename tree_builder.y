%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse_tree.h"
#include "tree_node.h"
extern int yylex();
extern int yyparse();
extern FILE* yyin;
void yyerror(const char* s);
ParseTree tree;
#define YYDEBUG 1
%}

%union {
    char* string;
    int integer;
    class TreeNode* node;
    class Expr* expr;
}

%type <string> name isachildof
%type <integer> weight
%type <node> node
%type <expr> expr
%token <string> IDENTIFIER STRING
%token <integer> INTEGER
%token BUILDNODE FOR IN LBRACKET RBRACKET LBRACE RBRACE SEMICOLON COLON EQUALS PLUS NAME WEIGHT ISACHILDOF ERROR

%%

statements:
    | statements statement
    {}
    ;


statement:
    buildnode_statement
    //{printf("Statement\n");}
    | for_statement
    ;

buildnode_statement:
    BUILDNODE LBRACE node RBRACE SEMICOLON
    {   
        //printf("Build Node\n");
        tree.addNode($3);
        tree.printTree();

    }
    ;

node:
    name weight
    {
        $$ = new TreeNode($1, $2);
        printf("Root Node -> Name: %s Weight: %d \n", $1, $2);
    
    }
    | name weight isachildof
    {
        $$ = new TreeNode($1, $2, $3);
        printf("Child Node -> Name: %s Weight: %d Child Of: %s\n", $1, $2, $3);
        
    }
    ;
expr:
    STRING
    {
        $$ = new StringExpr($1);
    }
    | expr PLUS expr
    {
        $$ = new ConcatExpr($1, $3);
    }
    | INTEGER
    {
        $$ = new IntExpr($1);
    }
    ;

name:
    NAME EQUALS expr SEMICOLON
    {   
        string temp = $3->eval();
        $$ = strdup(temp.c_str());
        
    }
    ;

weight:
    WEIGHT EQUALS expr SEMICOLON
    {
        string temp = $3->eval();
        $$ = stoi(temp.c_str());
    }
    ;
isachildof:
    ISACHILDOF EQUALS STRING SEMICOLON
    {
        $$ = $3;
        //printf("Is a child of: %s\n", $3);
    }
    ;



for_statement:
    FOR IDENTIFIER IN LBRACKET INTEGER COLON INTEGER RBRACKET LBRACE statements RBRACE
    {   
        string var = $2;

        printf("Made it to the for loop\n");

        for (int i = $5; i <= $7; i++) {
            // Execute the statements in $8
            printf("For loop: %d\n", i);
        }
    }
    
    ;




%%
void yyerror(const char* s) {
    fprintf(stderr, "Parse error: %s\n", s);
    exit(1);
}


int main(int argc, const char * argv[]) {
    yydebug = 0;
    if (argc > 1) {
        FILE *file = fopen(argv[1], "r");
        if (!file) {
            fprintf(stderr, "Cannot open file: %s\n", argv[1]);
            return 1;
        }
        printf("Parsing file: %s\n", argv[1]);
        yyin = file;
        
    }

    yyparse();
    tree.printTree();
    tree.printNodes();
    if (yyin != stdin) {
        fclose(yyin);
    }
    return 0;
}