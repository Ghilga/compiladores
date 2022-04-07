// Autor: Leonardo de Souza Augusto - 278998

%{
  #include <stdio.h>
  #include <stdlib.h>
  #include "ast.h"

  int getLineNumber();
  int yyerror();
  int yylex();
  AST *fullAst;

%}

%union{
  HASH_NODE *symbol;
  AST *ast;
}

%token KW_CHAR           
%token KW_INT            
%token KW_FLOAT          

%token KW_IF             
%token KW_THEN           
%token KW_ELSE           
%token KW_WHILE          
%token KW_GOTO           
%token KW_READ           
%token KW_PRINT          
%token KW_RETURN         

%token OPERATOR_LE       
%token OPERATOR_GE       
%token OPERATOR_EQ       
%token OPERATOR_DIF      

%token<symbol> TK_IDENTIFIER     

%token<symbol> LIT_INTEGER   
%token<symbol> LIT_CHAR          
%token<symbol> LIT_STRING        

%token TOKEN_ERROR     


%left OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_DIF '<' '>' 
%left '+' '-'
%left '*' '/'

%type<ast> expr
%type<ast> lcmd
%type<ast> cmd
%type<ast> if_body
%type<ast> decl
%type<ast> array
%type<ast> array_values
%type<ast> label
%type<ast> program
%type<ast> dec
%type<ast> remainder
%type<ast> remainderfunc
%type<ast> decfunc
%type<ast> arglist
%type<ast> remainder_args
%type<ast> arg
%type<ast> decintchar
%type<ast> decfloat
%type<ast> exprlist
%type<ast> printarg
%type<ast> printargs


%%

program: decl { fullAst = $1; }
    ;

decl: 
      dec remainder             { $$ = astCreate(AST_DECL,0,$1,$2,0,0);}
    | decfunc remainderfunc     { $$ = astCreate(AST_FUNC_DECL,0,$1,$2,0,0);}
    |                           { $$ = 0; }
    ;

remainder: 
      ';' decl      { $$ = $2; }
    ;

remainderfunc: 
      decl          { $$ = $1; }
    ;

decfunc:  
      KW_INT TK_IDENTIFIER '(' arglist ')' cmd      { $$ = astCreate(AST_DEC_INTFUNC,$2,$4,$6,0,0); }
    | KW_CHAR TK_IDENTIFIER '(' arglist ')' cmd     { $$ = astCreate(AST_DEC_CHARFUNC,$2,$4,$6,0,0); }
    | KW_FLOAT TK_IDENTIFIER '(' arglist ')' cmd    { $$ = astCreate(AST_DEC_FLOATFUNC,$2,$4,$6,0,0); }
    ;

arglist: 
      arg remainder_args  { $$ = astCreate(AST_ARGLIST,0,$1,$2,0,0); }
    |                     { $$ = 0; }
    ;

remainder_args:
      ',' arg remainder_args    { $$ = astCreate(AST_ARGLIST,0,$2,$3,0,0); }
    |                           { $$ = 0; }
    ;

arg: 
      KW_INT TK_IDENTIFIER      { $$ = astCreate(AST_ARGINT,$2,0,0,0,0); }
    | KW_CHAR TK_IDENTIFIER     { $$ = astCreate(AST_ARGCHAR,$2,0,0,0,0); }
    | KW_FLOAT TK_IDENTIFIER    { $$ = astCreate(AST_ARGFLOAT,$2,0,0,0,0); }
    ;

dec:  KW_INT TK_IDENTIFIER decintchar     { $$ = astCreate(AST_DECINT,$2,$3,0,0,0); }
    | KW_CHAR TK_IDENTIFIER decintchar    { $$ = astCreate(AST_DECCHAR,$2,$3,0,0,0); }
    | KW_FLOAT TK_IDENTIFIER decfloat     { $$ = astCreate(AST_DECFLOAT,$2,$3,0,0,0); }
    ;  

decintchar: 
      ':' LIT_INTEGER     { $$ = astCreateSymbol($2); }
    | ':' LIT_CHAR        { $$ = astCreateSymbol($2); }
    | array               { $$ = $1; }
    ;

decfloat: 
      ':' LIT_INTEGER '/' LIT_INTEGER   { $$ = astCreate(AST_FLOAT,0,astCreateSymbol($2),astCreateSymbol($4),0,0); }
    | array                             { $$ = $1; }
    ;

array:
      '[' LIT_INTEGER ']' ':' LIT_INTEGER array_values    { $$ = astCreate(AST_ARRAY,$2,astCreate(AST_ARR_VALUES,$5,$6,0,0,0),0,0,0); } 
    | '[' LIT_INTEGER ']' ':' LIT_CHAR array_values       { $$ = astCreate(AST_ARRAY,$2,astCreate(AST_ARR_VALUES,$5,$6,0,0,0),0,0,0); }
    | '[' LIT_INTEGER ']'                                 { $$ = astCreateSymbol($2); }
    ;

array_values: 
      LIT_INTEGER array_values    { $$ = astCreate(AST_ARR_VALUES,$1,$2,0,0,0); }
    | LIT_CHAR array_values       { $$ = astCreate(AST_ARR_VALUES,$1,$2,0,0,0); }
    |                             { $$ = 0; }
    ;

label:
      TK_IDENTIFIER ':' { $$ = astCreateSymbol($1); }
    ;

lcmd: 
      cmd ';' lcmd    { $$ = astCreate(AST_LCMD,0,$1,$3,0,0); }
    | label lcmd      { $$ = astCreate(AST_LABEL,0,$1,$2,0,0); }
    |                 { $$ = 0; }
    ;

cmd: 
      '{' lcmd '}'                          { $$ = astCreate(AST_CMD,0,$2,0,0,0);; }
    | TK_IDENTIFIER '=' expr                { $$ = astCreate(AST_ATTR,$1,$3,0,0,0); }
    | TK_IDENTIFIER '[' expr ']' '=' expr   { $$ = astCreate(AST_ATTR,$1,$3,$6,0,0); }
    | KW_PRINT printargs                    { $$ = astCreate(AST_PRINT,0,$2,0,0,0); }
    | KW_WHILE expr cmd                     { $$ = astCreate(AST_WHILE,0,$2,$3,0,0); }
    | KW_IF expr KW_THEN cmd if_body        { $$ = astCreate(AST_IF,0,$2,$4,$5,0); }
    | KW_GOTO TK_IDENTIFIER                 { $$ = astCreate(AST_GOTO,0,astCreateSymbol($2),0,0,0); }
    | KW_RETURN expr                        { $$ = astCreate(AST_RETURN,0,$2,0,0,0); }
    |                                       { $$ = 0; }
    ;

if_body:
      KW_ELSE cmd   { $$ = astCreate(AST_ELSE,0,$2,0,0,0); }
    |               { $$ = 0; }
    ; 

expr: 
      LIT_INTEGER                     { $$ = astCreateSymbol($1); }
    | LIT_CHAR                        { $$ = astCreateSymbol($1); }
    | TK_IDENTIFIER '[' expr ']'      { $$ = astCreate(AST_ARR_ELEMENT,$1,$3,0,0,0); }
    | TK_IDENTIFIER '(' exprlist')'   { $$ = astCreate(AST_FUNC_CALL,$1,$3,0,0,0); } 
    | TK_IDENTIFIER                   { $$ = astCreateSymbol($1); }        
    | KW_READ                         { $$ = 0; }
    | expr '+' expr                   { $$ = astCreate(AST_ADD,0,$1,$3,0,0); }      
    | expr '-' expr                   { $$ = astCreate(AST_SUB,0,$1,$3,0,0); }        
    | expr '*' expr                   { $$ = astCreate(AST_MUL,0,$1,$3,0,0); }
    | expr '/' expr                   { $$ = astCreate(AST_DIV,0,$1,$3,0,0); }
    | expr '<' expr                   { $$ = astCreate(AST_LESS,0,$1,$3,0,0); }
    | expr '>' expr                   { $$ = astCreate(AST_GREATER,0,$1,$3,0,0); }
    | expr OPERATOR_DIF expr          { $$ = astCreate(AST_DIF,0,$1,$3,0,0); }
    | expr OPERATOR_EQ expr           { $$ = astCreate(AST_EQ,0,$1,$3,0,0); }
    | expr OPERATOR_GE expr           { $$ = astCreate(AST_GE,0,$1,$3,0,0); }
    | expr OPERATOR_LE expr           { $$ = astCreate(AST_LE,0,$1,$3,0,0); }
    | '(' expr ')'                    { $$ = $2; }        
    ;

exprlist:
      expr ',' exprlist     { $$ = astCreate(AST_EXPR_LIST,0,$1,$3,0,0); }
    | expr                  { $$ = $1; }
    ;

printargs:
      printarg ',' printargs    { $$ = astCreate(AST_PRINTARGS,0,$1,$3,0,0); }
    | printarg                  { $$ = $1; }
    ;

printarg:
      LIT_STRING    { $$ = astCreateSymbol($1); }
    | expr          { $$ = $1; }
    ;
%%

int yyerror (){
    printf("Syntax Error at line %d\n", getLineNumber());
    exit(3);
}