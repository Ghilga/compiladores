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

%token TK_IDENTIFIER     

%token LIT_INTEGER       
%token LIT_CHAR          
%token LIT_STRING        

%token TOKEN_ERROR       

%{

int yyerror();

%}

%%

program: decl
    ;

decl: 
      dec resto
    | decfunc restofunc
    ;

resto: 
      ';' dec resto
    |
    ;
restofunc: 
      decl
    ;


decfunc:  
      KW_INT TK_IDENTIFIER '(' arglist ')' body
    | KW_CHAR TK_IDENTIFIER '(' arglist ')' body
    | KW_FLOAT TK_IDENTIFIER '(' arglist ')' body
    ;

arglist: 
      arg ',' arglist
    | arg
    |
    ;

arg: 
      KW_INT TK_IDENTIFIER
    | KW_CHAR TK_IDENTIFIER
    | KW_FLOAT TK_IDENTIFIER
    ;

dec:  KW_INT TK_IDENTIFIER decintchar
    | KW_CHAR TK_IDENTIFIER decintchar
    | KW_FLOAT TK_IDENTIFIER decfloat
    |
    ;  

decintchar: 
      ':' LIT_INTEGER 
    | ':' LIT_CHAR
    | array
    ;

decfloat: 
      ':' LIT_INTEGER '/' LIT_INTEGER
    | array
    ;

array:
      '[' LIT_INTEGER ']' ':' LIT_INTEGER arrayvalues
    | '[' LIT_INTEGER ']' ':' LIT_CHAR arrayvalues
    | '[' LIT_INTEGER ']'
    ;

arrayvalues: 
      LIT_INTEGER arrayvalues
    | LIT_CHAR arrayvalues
    |
    ;  

body: 
      '{' lcmd '}'
    ;

lcmd: 
      cmd ';' lcmd
    |
    ;

cmd: 
      TK_IDENTIFIER '=' expr
    | TK_IDENTIFIER '[' expr ']' '=' expr
    | KW_PRINT printargs
    | KW_WHILE expr body
    | KW_IF expr
    //TODO IF, THEN, ELSE, GOTO, RETURN
    ;

expr: 
      LIT_INTEGER
    | LIT_CHAR
    | TK_IDENTIFIER '[' expr ']'
    | TK_IDENTIFIER '(' exprlist')'
    | TK_IDENTIFIER
    | KW_READ
    | expr '+' expr
    | expr '-' expr
    | expr '*' expr
    | expr '/' expr
    | expr '<' expr
    | expr '>' expr
    | expr OPERATOR_DIF expr
    | expr OPERATOR_EQ expr
    | expr OPERATOR_GE expr
    | expr OPERATOR_LE expr
    | '(' expr ')'
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