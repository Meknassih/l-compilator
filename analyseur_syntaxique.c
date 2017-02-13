#include <stdio.h>
#include <stdlib.h>
#include "headers/symboles.h"
#include "headers/util.h"
#include "headers/analyseur_syntaxique.h"
#include "headers/analyseur_lexical.h"

short int uniteCourante;
char yytext[100];
FILE *yyin;
char nom[100];
char valeur[100];
int const XML = 1;

void pg(void) { //Axiome
  affiche_balise_ouvrante("programme",XML);
  if (uniteCourante == ENTIER) {
    uniteCourante == yylex();
    odv();
    ldf();
  } else if (uniteCourante == ID_FCT) {
    uniteCourante == yylex();
    ldf();
  } else if (uniteCourante == FIN)
    return;
  else
    erreur("Erreur de syntaxe");
  affiche_balise_fermante("programme",XML);
  return;
}

void odv(void)  {
  affiche_balise_ouvrante("optDecVariables",XML);
  if (uniteCourante == ENTIER) {
    uniteCourante == yylex();
    ldv();
    uniteCourante == yylex();
    if (uniteCourante != POINT_VIRGULE)
      erreur("';' manquant");
    uniteCourante == yylex();
  } else if (!est_suivant(_optDecVariables_, uniteCourante))
    erreur("Erreur de syntaxe");
  affiche_balise_fermante("optDecVariables",XML);
}

int main (int argc, char **argv) {
  yyin = fopen(argv[1], "r");
  if (yyin == NULL) {
    fprintf(stderr, "Impossible d'ouvrir le fichier %s\n", argv[1]);
    exit(1);
  }
  uniteCourante=yylex();
  pg();
  printf("SYN: Analyse syntaxique terminée avec succès\n");
  return 0;
}
