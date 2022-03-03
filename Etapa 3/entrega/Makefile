# Autor: Leonardo de Souza Augusto - 278998 


etapa3: clean main.o y.tab.o lex.yy.o decompiler.o hash.o ast.o
	gcc main.o lex.yy.o y.tab.o decompiler.o hash.o ast.o -o etapa3

main.o: main.c
	gcc -c main.c
hash.o: hash.c
	gcc -c hash.c
ast.o: ast.c
	gcc -c ast.c
lex.yy.o: lex.yy.c
	gcc -c lex.yy.c
y.tab.o: y.tab.c
	gcc -c y.tab.c
decompiler.o: decompiler.c
	gcc -c decompiler.c

y.tab.c: parser.y
	yacc -d parser.y

lex.yy.c: scanner.l 
	lex scanner.l 

entrega:
	tar cvzf etapa3.tgz .

clean: 
	rm -f etapa3 lex.yy.c y.tab.c y.tab.h etapa3.tgz *.o