# Autor: Leonardo de Souza Augusto - 278998 
FLAGS = -g
SRC = main.c lex.yy.c y.tab.c decompiler.c hash.c ast.c semantic.c

etapa: clean lex.yy.c y.tab.c
	gcc $(FLAGS) $(SRC) -o etapa4

etapa4: clean main.o y.tab.o lex.yy.o decompiler.o hash.o ast.o semantic.o
	gcc main.o lex.yy.o y.tab.o decompiler.o hash.o ast.o semantic.o -o etapa4

main.o: main.c
	gcc -c main.c
semantic.o: semantic.c
	gcc -c semantic.c
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
	tar cvzf etapa4.tgz .

clean: 
	rm -f etapa4 lex.yy.c y.tab.c y.tab.h etapa4.tgz *.o