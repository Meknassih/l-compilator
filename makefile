CC=gcc
CFLAGS=-I. -Wall -pedantic -std=c99
DEPS=headers/analyseur_lexical.h headers/util.h headers/symboles.h headers/analyseur_syntaxique.h
OBJ=analyseur_lexical.o analyseur_syntaxique.o test_yylex.o util.o premiers.o suivants.o
EXE=lex syn

all: lex syn

syn: lex analyseur_syntaxique.o premiers.o suivants.o
	$(CC) -o syn analyseur_lexical.o analyseur_syntaxique.o util.o premiers.o suivants.o

lex: test_yylex.o analyseur_lexical.o util.o
	$(CC) -c test_yylex.o analyseur_lexical.o util.o $(CFLAGS)

test_yylex.o: test_yylex.c #$(DEPS)
	$(CC) $(CFLAGS) -c test_yylex.c

analyseur_lexical.o: analyseur_lexical.c #$(DEPS)
	$(CC) $(CFLAGS) -c analyseur_lexical.c

analyseur_syntaxique.o: analyseur_syntaxique.c #$(DEPS)
	$(CC) $(CFLAGS) -c analyseur_syntaxique.c

util.o: util.c
	$(CC) $(CFLAGS) -c util.c

premiers.o: premiers.c
	$(CC) $(CFLAGS) -c premiers.c

suivants.o: suivants.c
	$(CC) $(CFLAGS) -c suivants.c
clean:
	- rm -f $(OBJ)
	- rm -f $(EXE)
