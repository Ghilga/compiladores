# Autor: Leonardo de Souza Augusto - 278998 


etapa2: clean y.tab.c lex.yy.c hash.c main.c
	gcc lex.yy.c -o etapa2 

y.tab.c: parser.y
	yacc parser.y -d

lex.yy.c: scanner.l 
	lex scanner.l 

clean: 
	rm -f etapa2 lex.yy.c y.tab.c y.tab.h