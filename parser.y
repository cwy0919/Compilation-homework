%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen.h"

extern int yylex(void);
extern FILE* yyin;
void yyerror(const char *s);

extern CodeNode *root_code;
CodeNode* concat_code(CodeNode *a, CodeNode *b);
%}



%union {
    int num;
    char *str;
    struct code_node* code;
}


%token <str> IDN ILOCT ILHEX
%token <num> DEC OCT HEX
%token IF THEN ELSE WHILE DO BEG END
%token ADD SUB MUL DIV GT LT EQ GE LE NEQ SLP SRP SEMI

%type <code> program stmt_list stmt expr cond if_stmt


%nonassoc THEN_PREC
%nonassoc ELSE

%left ADD SUB
%left MUL DIV

%%

program:
    stmt_list          { root_code = $1; }
;

stmt_list:
    stmt               { $$ = $1; }
  | stmt stmt_list     { $$ = concat_code($1, $2); }
;

stmt:
    IDN EQ expr SEMI   { $$ = gen_assign_code($1, $3); }
  | if_stmt            { $$ = $1; }
  | WHILE cond DO stmt { $$ = gen_while_code($2, $4); }
  | BEG stmt_list END { $$ = $2; }
;

if_stmt:
    IF cond THEN stmt %prec THEN_PREC   { $$ = gen_if_code($2, $4, NULL); }
  | IF cond THEN stmt ELSE stmt          { $$ = gen_if_code($2, $4, $6); }
;

cond:
    expr GT expr       { $$ = gen_cond_code(OP_GT, $1, $3); }
  | expr LT expr       { $$ = gen_cond_code(OP_LT, $1, $3); }
  | expr EQ expr       { $$ = gen_cond_code(OP_EQ, $1, $3); }
  | expr GE expr       { $$ = gen_cond_code(OP_GE, $1, $3); }
  | expr LE expr       { $$ = gen_cond_code(OP_LE, $1, $3); }
  | expr NEQ expr      { $$ = gen_cond_code(OP_NE, $1, $3); }
;

expr:
    expr ADD expr      { $$ = gen_binop_code(OP_ADD, $1, $3); }
  | expr SUB expr      { $$ = gen_binop_code(OP_SUB, $1, $3); }
  | expr MUL expr      { $$ = gen_binop_code(OP_MUL, $1, $3); }
  | expr DIV expr      { $$ = gen_binop_code(OP_DIV, $1, $3); }
  | SLP expr SRP       { $$ = $2; }
  | IDN                { $$ = gen_var_code($1); }
  | DEC                { $$ = gen_const_code($1); }
  | OCT                { $$ = gen_const_code($1); }
  | HEX                { $$ = gen_const_code($1); }
;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Syntax error: %s\n", s);
}
