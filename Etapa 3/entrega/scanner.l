/*Autor: Leonardo de Souza Augusto - 278998 */

%{
#include "ast.h"
#include "y.tab.h"

int getLineNumber();
int isRunning();

int lineNumber = 1;
int running = 1;

%}

%x COMMENT

%%

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

"<="                { return OPERATOR_LE; }
">="                { return OPERATOR_GE; }
"=="                { return OPERATOR_EQ; }
"!="                { return OPERATOR_DIF; }

[-*.,;:{}()\[\]/<>=+] { return yytext[0]; }
\"([^"\n]|\\\")+\"  { yylval.symbol = hashInsert(yytext, LIT_STRING); return LIT_STRING; }
[0-9]+              { yylval.symbol = hashInsert(yytext, LIT_INTEGER); return LIT_INTEGER; }
\'.\'               { yylval.symbol = hashInsert(yytext, LIT_CHAR); return LIT_CHAR; }
[a-z_-]+            { yylval.symbol = hashInsert(yytext, TK_IDENTIFIER); return TK_IDENTIFIER; }

\n                  { lineNumber++; }
[ \t]
\\\\.*
\\\*                { BEGIN(COMMENT); }        
\"+                 { return TOKEN_ERROR; }
.                   { return TOKEN_ERROR; }             

<COMMENT>[\*][\\]   { BEGIN(INITIAL); }
<COMMENT>"\n"       { lineNumber++; }
<COMMENT>.          

%%

void initMe(){
    hashInit();
}

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