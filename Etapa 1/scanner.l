/*Autor: Leonardo de Souza Augusto - 278998 */

%{
#include "main.c"

int getLineNumber();
int isRunning();

int lineNumber = 1;
int running = 1;

%}

%x COMMENT
%x STRING

%%

linha               { printf("Linhas: %d\n", lineNumber); }
char                { return KW_CHAR; }
int                 { return KW_INT; }
float               { return KW_FLOAT; }

if                  { return KW_IF; }
then                { return KW_THEN; }
else                { return KW_ELSE; }
while               { return KW_WHILE; }
goto                { return KW_GOTO; }
read                { return KW_READ; }
print               { return KW_PRINT; }
return              { return KW_RETURN; }

teste               { printf("Teste bem sucedido!\n"); return TOKEN_ERROR; }
"<="                { return OPERATOR_LE; }
">="                { return OPERATOR_GE; }
"=="                { return OPERATOR_EQ; }
"!="                { return OPERATOR_DIF; }

[0-9]               { return LIT_INTEGER; }
[a-z]               { printf("Caractere %c\n", yytext[0]); return LIT_CHAR; }
[a-z]+              { printf("String %s\n", yytext); return LIT_STRING; }

[.,;:{}()[]<>=_-]    { return yytext[0]; }
[ \t]
"\n"                { lineNumber++; }
"//".*
"/*"                { BEGIN(COMMENT); }
\"                  { BEGIN(STRING); }           
.                   { return TOKEN_ERROR; }

<STRING>\"          { BEGIN(INITIAL); }
<STRING>"\n"        { lineNumber++; return TOKEN_ERROR; }
<STRING>.           
<COMMENT>"*/"       { BEGIN(INITIAL); }
<COMMENT>"\n"       { lineNumber++; }
<COMMENT>.          

%%


int getLineNumber () {
    return lineNumber;
}

int isRunning () {
    return running;
}

int yywrap() {
    running = 0;
    return 1;
}