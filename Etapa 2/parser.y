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

%%

programa: decl
    ;

decl: dec ',' decl
    |
    ;

dec: KW_INT TK_IDENTIFIER
    | KW_INT TK_IDENTIFIER '(' ')' '{' '}'
    ;






%%

int yyerror (){
    printf("Syntax Error\n");
    exit(3);
}