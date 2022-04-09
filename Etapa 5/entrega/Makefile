# Autor: Leonardo de Souza Augusto - 278998 
FLAGS = -g
SRC = main.c lex.yy.c y.tab.c decompiler.c hash.c ast.c semantic.c tacs.c

etapa: clean lex.yy.c y.tab.c
	gcc $(FLAGS) $(SRC) -o etapa5

y.tab.c: parser.y
	yacc -d parser.y

lex.yy.c: scanner.l 
	lex scanner.l 

test:
	./etapa5 testes/sample.txt testes/out1.txt
	./etapa5 testes/out1.txt testes/out2.txt
	diff testes/out1.txt testes/out2.txt

entrega:
	tar cvzf etapa5.tgz .

clean: 
	rm -f etapa5 lex.yy.c y.tab.c y.tab.h etapa5.tgz *.o testes/out*.txt
