#ifndef __LIRE_UNITE__
#define __LIRE_UNITE__

#include "stdio.h"

int mangeEspaces();
char lireCar(void);
void delireCar();
int lireMotclef(void);
int yylex(void);
void nom_token( int token, char *nom, char *valeur );
void test_yylex_internal( FILE *yyin );

#endif
