# Autor: Leonardo de Souza Augusto - 278998 


etapa2: lex.yy.c hash.c main.c
	gcc lex.yy.c -o etapa2 

lex.yy.c: scanner.l y.tab.c y.tab.h
	lex scanner.l 

y.tab.c: parser.y
	yacc parser.y -d

clean:
	rm etapa2 lex.yy.c y.tab.c y.tab.h
