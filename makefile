CC=gcc
CFLAGS=-I. -Wall -pedantic -std=c99
DEPS=headers/analyseur_lexical.h headers/util.h headers/symboles.h headers/analyseur_syntaxique.h
OBJ=analyseur_lexical.o analyseur_syntaxique.o util.o premiers.o suivants.o
EXE=syn

all: syn

syn: analyseur_lexical.o analyseur_syntaxique.o util.o premiers.o suivants.o
	$(CC) -o syn $^

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
