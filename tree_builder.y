/*
Authors: Wyatt Shinkle and Zach Naymik
Date: 4/18/2024
Description: This is the parser for the build language. It will take in a file and parse it into a parse tree. 
*/


%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse_tree.h"
extern int yylex();
extern int yyparse();
extern FILE* yyin;
void yyerror(const char* s);
#define YYDEBUG 1
%}

%start start_var

%union {
    char* string;
    int integer;
    class expression* expr;
    class int_expression *int_ptr;
    class statement *s_ptr;
    class compound_statement *c_ptr;
    struct string_list *string_list;
}


%type <expr> expr name isachildof
%type <c_ptr> prog start_var 
%type <s_ptr> statement for_statement node build_statement
%type <int_ptr> weight int_expr
%type <string_list> string_list
%token <string> IDENTIFIER STRING
%token <integer> INTEGER
%token BUILDNODE FOR IN LBRACKET RBRACKET LBRACE RBRACE SEMICOLON COLON EQUALS PLUS NAME WEIGHT ISACHILDOF ERROR COMMA

%%
start_var : prog { // At this point, the 
                   // the program is done --- let's evaluate the
                   // program
                   map<string,string> my_sym_tab;
                   ParseTree tree;
                   $$= $1;
                   $1->evaluate_statement(my_sym_tab, tree);
                   tree.printTree();
}

prog: statement  prog {$$ = new compound_statement($1,$2);}
    | {$$ = NULL;}
    ;



statement:
    build_statement
    {
        //printf("Build Statement\n");
        $$ = $1;
    }
    | for_statement
    {
        //printf("For Statement\n");
        $$ = $1;
    }
    ;

build_statement:
    BUILDNODE LBRACE node RBRACE SEMICOLON
    {
        $$ = $3;
    }
    ;

node:
    name weight
    {   

        $$ = new build_statement($1, $2);
        //printf("Root Node -> Name: %s Weight: %d \n", $1, $2);
    
    }
    | name weight isachildof
    {
        $$ = new build_statement($1, $2, $3);
        //printf("Child Node -> Name: %s Weight: %d Child Of: %s\n", $1, $2, $3);
        
    }
    ;
expr:
    STRING
    {
        $$ = new string_constant($1);
    }
    /* | INTEGER
    {
        $$ = new int_constant($1);
    } */
    | IDENTIFIER
    {
        $$ = new variable($1);
    }
    | expr PLUS expr
    {   
        $$ = new concat_op($1, $3);
    }
    ;

int_expr:
    INTEGER
    {
        $$ = new int_constant($1);
    }
    | IDENTIFIER
    {
        $$ = new int_variable($1);
    }
    | int_expr PLUS int_expr
    {
        $$ = new add_op($1, $3);
    }
    ;
name:
    NAME EQUALS expr SEMICOLON
    {   
        
        $$ = $3;
        
    }
    ;

weight:
    WEIGHT EQUALS int_expr SEMICOLON
    {
        $$ = $3;
    }
    ;
isachildof:
    ISACHILDOF EQUALS expr SEMICOLON
    {
        $$ = $3;
        //printf("Is a child of: %s\n", $3);
    }
    ;



for_statement:
    FOR IDENTIFIER IN LBRACKET INTEGER COLON INTEGER RBRACKET LBRACE prog RBRACE
    {   
        //printf("For Statement\n");
        $$ = new for_statement($2, $5, $7, $10);
    }
    | FOR IDENTIFIER IN LBRACKET string_list RBRACKET LBRACE prog RBRACE
    {
        //printf("For Statement\n");
        $$ = new for_in_statement($2, $5, $8);
    }
    
    ;

string_list
    : IDENTIFIER
    {
        $$ = new string_list();
        $$->string = $1;
        $$->next = NULL;
    }
    | string_list COMMA IDENTIFIER
    {
        string_list* new_node = new string_list();
        new_node->string = $3;
        new_node->next = $1;
        $$ = new_node;
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
        //printf("Parsing file: %s\n", argv[1]);
        yyin = file;
        
    }

    yyparse();
    if (yyin != stdin) {
        fclose(yyin);
    }
    return 0;
}