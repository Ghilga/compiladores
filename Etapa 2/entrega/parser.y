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

%left OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_DIF '<' '>' 
%left '+' '-'
%left '*' '/'

%{

int yyerror();

%}

%%

program: decl
    ;

decl: 
      dec remainder
    | decfunc remainderfunc
    |
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
      '[' LIT_INTEGER ']' ':' LIT_INTEGER array_values
    | '[' LIT_INTEGER ']' ':' LIT_CHAR array_values
    | '[' LIT_INTEGER ']'
    ;

array_values: 
      LIT_INTEGER array_values
    | LIT_CHAR array_values
    |
    ;

lcmd: 
      cmd ';' lcmd
    | label lcmd
    |
    ;

label:
      TK_IDENTIFIER ':'
    ;

cmd: 
      '{' lcmd '}'
    | TK_IDENTIFIER '=' expr
    | TK_IDENTIFIER '[' expr ']' '=' expr
    | KW_PRINT printargs
    | KW_WHILE expr cmd
    | KW_IF expr KW_THEN if_body
    | KW_GOTO TK_IDENTIFIER
    | KW_RETURN expr
    | 
    ;

if_body:
      cmd KW_ELSE cmd
    | cmd
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