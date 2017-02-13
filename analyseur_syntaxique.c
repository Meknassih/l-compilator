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

void E(void) { //Axiome
  affiche_balise_ouvrante("expArith",1);

  T();
  Eprime();

  affiche_balise_fermante("expArith",1);
  return;
}

void Eprime(void) { //Addition
  affiche_balise_ouvrante("termeBis",1);

  if (uniteCourante==PLUS) {
    indent();
      affiche_xml_texte("+\n");

    uniteCourante=yylex();
    E();

    affiche_balise_fermante("termeBis",1);
    return;
  } else if (uniteCourante==POINT_VIRGULE||uniteCourante==PARENTHESE_FERMANTE||uniteCourante==FOIS||uniteCourante==DIVISE||uniteCourante==MOINS||uniteCourante==FIN) {
    affiche_balise_fermante("termeBis",1);
    return;
  } else {
    erreur("SYN: Expression ou fin d'expression attendues");
  }
}

void T(void) { //Dérive en F ou T'
  affiche_balise_ouvrante("expArithBis",1);

  F();
  Tprime();

  affiche_balise_fermante("expArithBisfacteur",1);
  return;
}

void Tprime(void) { //Multiplication
  affiche_balise_ouvrante("facteurBis",1);

  if (uniteCourante==FOIS) {
    indent();
      affiche_xml_texte("*\n");

    uniteCourante=yylex();
    T();

    affiche_balise_fermante("facteurBis",1);
    return;
  } else if (uniteCourante==POINT_VIRGULE||uniteCourante==PARENTHESE_FERMANTE||uniteCourante==PLUS||uniteCourante==DIVISE||uniteCourante==MOINS||uniteCourante==FIN) {
    affiche_balise_fermante("facteurBis",1);
    return;
  } else {
    nom_token( uniteCourante, nom, valeur );
    printf("SYN: %s\t%s\t%s\n", yytext, nom, valeur);
    erreur("SYN: Expression ou fin d'expression attendues");
  }
}

void F(void) { //Nombre ou expression parenthésée
  //affiche_balise_ouvrante("expArith",1);
  if (uniteCourante==PARENTHESE_OUVRANTE) {
    indent();
      affiche_xml_texte("(\n");

    uniteCourante=yylex();
    E();
    if (uniteCourante==PARENTHESE_FERMANTE) {
      indent();
      affiche_xml_texte(")\n");
      affiche_balise_fermante("expArith",1);

      uniteCourante=yylex();
      return;
    } else {
      erreur("SYN: ')' attendue en fin d'expression");
    }
  } else if (uniteCourante==NOMBRE) {
    indent();
    affiche_xml_texte(yytext); printf("\n");
    affiche_balise_fermante("expArith",1);

    uniteCourante=yylex();
    return;
  } else {
    nom_token( uniteCourante, nom, valeur );
    printf("SYN: %s\t%s\t%s\n", yytext, nom, valeur);
    erreur("SYN: (expression) ou nombre attendus");
  }
}

int main (int argc, char **argv) {
  yyin = fopen(argv[1], "r");
  if (yyin == NULL) {
    fprintf(stderr, "Impossible d'ouvrir le fichier %s\n", argv[1]);
    exit(1);
  }
  uniteCourante=yylex();
  E();
  printf("SYN: Analyse syntaxique terminée avec succès\n");
  return 0;
}
