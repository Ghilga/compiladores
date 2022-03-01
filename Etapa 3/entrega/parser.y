%{
  #include <stdio.h>
  #include <stdlib.h>
  #include "ast.h"
  int getLineNumber();
  int yyerror();
  int yylex();

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
%token LIT_CHAR          
%token LIT_STRING        

%token TOKEN_ERROR     


%left OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_DIF '<' '>' 
%left '+' '-'
%left '*' '/'

%type<ast> expr
%type<ast> lcmd
%type<ast> cmd
%type<ast> if_body
%type<ast> decl
%type<ast> array_values
%%

program: decl
    ;

decl: 
      dec remainder
    | decfunc remainderfunc
    |                           { $$ = 0; }
    ;

remainder: 
      ';' decl
    ;
remainderfunc: 
      decl
    ;

decfunc:  
      KW_INT TK_IDENTIFIER '(' arglist ')' cmd
    | KW_CHAR TK_IDENTIFIER '(' arglist ')' cmd
    | KW_FLOAT TK_IDENTIFIER '(' arglist ')' cmd
    ;

arglist: 
      arg remainder_args
    | arg
    |
    ;

remainder_args:
      ',' arg remainder_args
    | ',' arg
    ;

arg: 
      KW_INT TK_IDENTIFIER
    | KW_CHAR TK_IDENTIFIER
    | KW_FLOAT TK_IDENTIFIER
    ;

dec:  KW_INT TK_IDENTIFIER decintchar
    | KW_CHAR TK_IDENTIFIER decintchar
    | KW_FLOAT TK_IDENTIFIER decfloat
    ;  

decintchar: 
      ':' LIT_INTEGER {}
    | ':' LIT_CHAR
    | array
    ;

decfloat: 
      ':' LIT_INTEGER '/' LIT_INTEGER
    | array
    ;

array:
      '[' LIT_INTEGER ']' ':' LIT_INTEGER array_values
    | '[' LIT_INTEGER ']' ':' LIT_CHAR array_values
    | '[' LIT_INTEGER ']'
    ;

array_values: 
      LIT_INTEGER array_values
    | LIT_CHAR array_values
    |                     { $$ = 0; }
    ;

label:
      TK_IDENTIFIER ':'
    ;

lcmd: 
      cmd ';' lcmd    { $$ = astCreate(AST_LCMD,0,$1,$3,0,0); }
    | label lcmd
    |                 { $$ = 0; }
    ;

cmd: 
      '{' lcmd '}'
    | TK_IDENTIFIER '=' expr    { astPrint($3, 0); }
    | TK_IDENTIFIER '[' expr ']' '=' expr
    | KW_PRINT printargs
    | KW_WHILE expr cmd
    | KW_IF expr KW_THEN cmd if_body
    | KW_GOTO TK_IDENTIFIER
    | KW_RETURN expr
    |                           { $$ = 0; }
    ;

if_body:
      KW_ELSE cmd
    |                           { $$ = 0; }
    ; 

expr: 
      LIT_INTEGER       {$$ = astCreate(AST_SYMBOL,$1,0,0,0,0);}
    | LIT_CHAR          
    | TK_IDENTIFIER '[' expr ']' 
    | TK_IDENTIFIER '(' exprlist')' 
    | TK_IDENTIFIER     {$$ = astCreate(AST_SYMBOL,$1,0,0,0,0);}        
    | KW_READ                   
    | expr '+' expr     {$$ = astCreate(AST_ADD,0,$1,$3,0,0);}      
    | expr '-' expr     {$$ = astCreate(AST_SUB,0,$1,$3,0,0);}        
    | expr '*' expr             
    | expr '/' expr             
    | expr '<' expr             
    | expr '>' expr            
    | expr OPERATOR_DIF expr
    | expr OPERATOR_EQ expr
    | expr OPERATOR_GE expr
    | expr OPERATOR_LE expr
    | '(' expr ')'      {$$ = $2;}        
    ;

exprlist:
      expr ',' exprlist
    | expr
    ;

printargs:
      printarg ',' printargs
    | printarg
    ;

printarg:
      LIT_STRING
    | expr
    ;
%%

int yyerror (){
    printf("Syntax Error at line %d\n", getLineNumber());
    exit(3);
}