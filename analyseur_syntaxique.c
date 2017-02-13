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

void ldv(void) {
  affiche_balise_ouvrante("listeDecVariables",XML);
  if (uniteCourante == ENTIER) {
    uniteCourante = yylex();
    dv(); ldvb();
  } else
    erreur("Erreur de syntaxe");
  affiche_balise_fermante("listeDecVariables",XML);
}

void ldvb(void) {
  affiche_balise_ouvrante("listeDecVariablesBis",XML);
  if (uniteCourante == VIRGULE) {
    uniteCourante == yylex();
    dv(); ldvb();
  } else if (!est_suivant(_listeDecVariablesBis_, uniteCourante)) {
    erreur("Erreur de syntaxe");
  }
  affiche_balise_fermante("listeDecVariablesBis",XML);
}

void dv(void) {
  affiche_balise_ouvrante("declarationVariable",XML);
  if (uniteCourante == ENTIER) {
    uniteCourante == yylex();
    if (uniteCourante == ID_VAR) {
      uniteCourante == yylex();
      ott();
    } else {
      erreur("Identifiant de variable attendu après ENTIER");
    }
  } else {
    erreur("Erreur de syntaxe");
  }
  affiche_balise_fermante("declarationVariable",XML);
}

void ott(void) {
  affiche_balise_ouvrante("optTailleTableau",XML);
  if (uniteCourante == CROCHET_OUVRANT) {
    uniteCourante == yylex();
    if (uniteCourante == NOMBRE) {
      uniteCourante == yylex();
      if (uniteCourante == CROCHET_FERMANT) {
        uniteCourante == yylex();
      } else
        erreur("Crochet fermant attendu après indice de tableau");
    } else
      erreur("Indince de tableau attendu après crochet ouvrant");
  } else if (!est_suivant(_optTailleTableau_, uniteCourante)) {
    erreur("Erreur de syntaxe");
  }
  affiche_balise_fermante("optTailleTableau",XML);
}

void ldf(void) {
  affiche_balise_ouvrante("listeDecFonctions",XML);
  if (uniteCourante == ID_FCT) {
    uniteCourante = yylex();
    df(); ldf();
  } else if (!est_suivant(_listeDecFonctions_, uniteCourante)) {
    erreur("Erreur de syntaxe");
  }
  affiche_balise_fermante("listeDecFonctions",XML);
}

void df(void) {
  affiche_balise_ouvrante("declarationFonction",XML);
  if (uniteCourante == ID_FCT) {
    uniteCourante = yylex();
    lp(); odv(); ib();
  } else
    erreur("Erreur de syntaxe");
    affiche_balise_fermante("declarationFonction",XML);
}

void lp(void) {
  affiche_balise_ouvrante("listeParam",XML);
  if (uniteCourante == PARENTHESE_OUVRANTE) {
    uniteCourante = yylex();
    oldv();
    if (uniteCourante == PARENTHESE_FERMANTE) {
      uniteCourante = yylex();
    } else
      erreur("Parenthèse fermante attendue à la fin de la liste de paramètres");
  } else
    erreur("Erreur de syntaxe");
  affiche_balise_fermante("listeParam",XML);
}

void oldv(void) {
  affiche_balise_ouvrante("optListeDecVariables",XML);
  if (uniteCourante == ENTIER) {
    uniteCourante = yylex();
    ldv();
  } else if (!est_suivant(_optListeDecVariables_, uniteCourante)) {
    erreur("Erreur de syntaxe");
  }
  affiche_balise_fermante("optListeDecVariables",XML);
}

void i(void) {
  affiche_balise_ouvrante("instruction",XML);
  if (uniteCourante == ID_VAR) {
    uniteCourante = yylex();
    iaff();
  } else if (uniteCourante == ACCOLADE_OUVRANTE) {
    uniteCourante = yylex();
    ib();
  } else if (uniteCourante == SI) {
    uniteCourante = yylex();
    isi();
  } else if (uniteCourante == TANTQUE) {
    uniteCourante = yylex();
    itq();
  } else if (uniteCourante == ID_FCT) {
    uniteCourante = yylex();
    iapp();
  } else if (uniteCourante == RETOUR) {
    uniteCourante = yylex();
    iret();
  } else if (uniteCourante == ECRIRE) {
    uniteCourante = yylex();
    iecr();
  } else if (uniteCourante == POINT_VIRGULE) {
    uniteCourante = yylex();
    ivide();
  } else {
    erreur("Erreur de syntaxe");
  }
  affiche_balise_fermante("instruction",XML);
}

void iaff(void) {
  affiche_balise_ouvrante("instructionAffect",XML);
  if (uniteCourante == ID_VAR) {
    uniteCourante = yylex();
    var();
    if (uniteCourante == EGAL) {
      uniteCourante = yylex();
      exp();
      if (uniteCourante == POINT_VIRGULE) {
        uniteCourante = yylex();
      } else
        erreur("';' manquant");
    } else {
      erreur("Erreur de syntaxe");
    }
  } else {
    erreur("Erreur de syntaxe");
  }
  affiche_balise_fermante("instructionAffect",XML);
}

void ib(void) {
  affiche_balise_ouvrante("instructionBloc",XML);
  if (uniteCourante == ACCOLADE_OUVRANTE) {
    uniteCourante = yylex();
    li();
    if (uniteCourante == ACCOLADE_FERMANTE) {
      uniteCourante = yylex();
    } else {
      erreur("Accolade fermante manquante");
    }
  } else {
    erreur("Accolade ouvrante manquante");
  }
  affiche_balise_fermante("instructionBloc",XML);
}

void li(void) {
  affiche_balise_ouvrante("listeInstructions",XML);
  if (uniteCourante == ID_VAR || uniteCourante == ACCOLADE_OUVRANTE || uniteCourante == SI || uniteCourante == TANTQUE || uniteCourante == ID_FCT || uniteCourante == RETOUR || uniteCourante == ECRIRE || uniteCourante == POINT_VIRGULE) {
    i(); li();
  } else if (!est_suivant(_listeInstructions_, uniteCourante)) {
    erreur("Erreur de syntaxe");
  }
  affiche_balise_fermante("listeInstructions",XML);
}
//USE est_premier() ABOVE THIS COMMENT
void isi(void) {
  affiche_balise_ouvrante("listeInstructions",XML);
  if (uniteCourante == SI) {
    uniteCourante = yylex();
    if (est_premier(_expression_, uniteCourante)) { //?
      exp();
      if (uniteCourante == ALORS) {
        uniteCourante = yylex();
        ib(); osinon();
      } else
        erreur("ALORS manquant");
    } else
      erreur("Erreur de syntaxe");
  } else
    erreur("Erreur de syntaxe");

  affiche_balise_fermante("listeInstructions",XML);
}

void osinon(void) {
  affiche_balise_ouvrante("optSinon",XML);
  if (uniteCourante == SINON) {analyseur_syntaxique.c
analyseur_syntaxique.c
analyseur_syntaxique.c
analyseur_syntaxique.c
analyseur_syntaxique.c
analyseur_syntaxique.c
analyseur_syntaxique.c

    uniteCourante = yylex();
    ib();
  } else if (!est_suivant(_optSinon_, uniteCourante)) {
    erreur("Erreur de syntaxe");
  }
  affiche_balise_fermante("optSinon",XML);
}

void itq(void) {
  affiche_balise_ouvrante("instructionTantque",XML);
  if (uniteCourante == TANTQUE) {
    uniteCourante = yylex();
    exp();
    if (uniteCourante == FAIRE)
      uniteCourante = yylex();
      ib();
    ) else
      erreur("FAIRE manquant");
  } else
    erreur("Erreur de syntaxe");

  affiche_balise_fermante("instructionTantque",XML);
}

void iapp(void) {
  affiche_balise_ouvrante("instructionAppel",XML);
  if (est_premier(_appelFct_, uniteCourante)) {
    appf();
    if (uniteCourante == POINT_VIRGULE) {
      uniteCourante = yylex();
    } else
      erreur("';' manquant");
  } else {
    erreur("Erreur de syntaxe");
  }
  affiche_balise_fermante("instructionAppel",XML);
}

void iret(void) {
  affiche_balise_ouvrante("instructionRetour",XML);
  if (uniteCourante == RETOUR) {
    uniteCourante = yylex();
    if (est_premier(_expression_, uniteCourante)) {
      exp();
      if (uniteCourante == POINT_VIRGULE) {
        uniteCourante = yylex();
      } else {
        erreur("';' manquant");
      }
    } else
      erreur("Erreur de syntaxe");
  } else
    erreur("Erreur de syntaxe");
  affiche_balise_fermante("instructionRetour",XML);
}

void iecr(void) {
  affiche_balise_ouvrante("instructionEcriture",XML);
  if (uniteCourante == ECRIRE) {
    uniteCourante = yylex();
    if (uniteCourante == PARENTHESE_OUVRANTE) {
      uniteCourante = yylex();
      if (est_premier(_expression_, uniteCourante)) {
        exp();
        if (uniteCourante == PARENTHESE_FERMANTE) {
          uniteCourante = yylex();
          if (uniteCourante == POINT_VIRGULE) {
            uniteCourante = yylex();
          } else
            erreur("';' manquant");
        } else
          erreur("')' manquant");
      } else
        erreur("Erreur de syntaxe");
    } else
      erreur("'(' manquant");
  } else
    erreur("Erreur de syntaxe");

  affiche_balise_fermante("instructionEcriture",XML);
}

void ivide(void) {
  affiche_balise_ouvrante("instructionVide",XML);
  if (uniteCourante == POINT_VIRGULE) {
    uniteCourante = yylex();
  } else
    erreur("Erreur de syntaxe");

  affiche_balise_fermante("instructionVide",XML);
}

void exp(void) {
  affiche_balise_ouvrante("expression",XML);
  if (est_premier(_conjonction_, uniteCourante)) {
    conj(); expB();
  } else
    erreur("Erreur de syntaxe");
  affiche_balise_fermante("expression",XML);
}

void expB(void) {
  affiche_balise_ouvrante("expressionBis",XML);
  if (uniteCourante == OU) {
    uniteCourante = yylex();
    if (est_premier(_conjonction_, uniteCourante)) {
      conj(); expB();
    } else {
      erreur("Conjonction attendue après '|'");
    }
  } else if (!est_suivant(_expressionBis_, uniteCourante)) {
    erreur("Erreur de syntaxe");
  }
  affiche_balise_fermante("expressionBis",XML);
}

void conj(void) {
  affiche_balise_ouvrante("conjonction",XML);
  if (est_premier(_comparaison_, uniteCourante)) {
    comp(); conjB();
  } else
    erreur("Erreur de syntaxe");
  affiche_balise_fermante("conjonction",XML);
}

void conjB(void) {
  affiche_balise_ouvrante("conjonctionBis",XML);
  if (uniteCourante == ET) {
    uniteCourante = yylex();
    if (est_premier(_comparaison_, uniteCourante)) {
      comp(); conjB();
    } else {
      erreur("Comparaison attendue après '&'");
    }
  } else if (!est_suivant(_expressionBis_, uniteCourante)) {
    erreur("Erreur de syntaxe");
  }
  affiche_balise_fermante("conjonctionBis",XML);
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
