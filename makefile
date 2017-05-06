CC=gcc
CFLAGS=-I. -Wall -pedantic -std=gnu99
DEPS=headers/analyseur_lexical.h headers/util.h headers/symboles.h headers/analyseur_syntaxique.h syntabs.h affiche_arbre_abstrait.h
OBJ=analyseur_lexical.o analyseur_syntaxique.o util.o premiers.o suivants.o syntabs.o affiche_arbre_abstrait.o
EXE=syn

all: syn

syn: analyseur_lexical.o analyseur_syntaxique.o util.o premiers.o suivants.o syntabs.o affiche_arbre_abstrait.o
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

syntabs.o: syntabs.c
	$(CC) $(CFLAGS) -c syntabs.c

affiche_arbre_abstrait.o: affiche_arbre_abstrait.c
	$(CC) $(CFLAGS) -c affiche_arbre_abstrait.c

clean:
	- rm -f $(OBJ)
	- rm -f $(EXE)
