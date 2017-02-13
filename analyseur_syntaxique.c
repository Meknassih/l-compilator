#include <stdio.h>
#include <stdlib.h>
#include "headers/symboles.h"
#include "headers/util.h"
#include "headers/analyseur_syntaxique.h"
#include "headers/analyseur_lexical.h"
#include "headers/premiers.h"
#include "headers/suivants.h"

short int uniteCourante;
char yytext[100];
FILE *yyin;
char nom[100];
char valeur[100];
int const XML = 1;

void pg(void) { //Axiome
  affiche_balise_ouvrante("programme",XML);
  if (est_premier(_optDecVariables_, uniteCourante)) {
    odv();
    ldf();
  } else if (est_premier(_listeDecFonctions_, uniteCourante)) {
    ldf();
  } else if (!est_suivant(_programme_, uniteCourante)) {
    erreur("Erreur de syntaxe");
  }
  affiche_balise_fermante("programme",XML);
  return;
}

void odv(void)  {
  affiche_balise_ouvrante("optDecVariables",XML);
  if (est_premier(_listeDecVariables_, uniteCourante)) {
    ldv();
    if (uniteCourante == POINT_VIRGULE)
      uniteCourante = yylex();
  } else if (!est_suivant(_optDecVariables_, uniteCourante))
    erreur("Erreur de syntaxe");
  affiche_balise_fermante("optDecVariables",XML);
}

void ldv(void) {
  affiche_balise_ouvrante("listeDecVariables",XML);
  if (est_premier(_declarationVariable_, uniteCourante)) {
    dv(); ldvb();
  } else
    erreur("Erreur de syntaxe");
  affiche_balise_fermante("listeDecVariables",XML);
}

void ldvb(void) {
  affiche_balise_ouvrante("listeDecVariablesBis",XML);
  if (uniteCourante == VIRGULE) {
    uniteCourante = yylex();
    if (est_premier(_declarationVariable_, uniteCourante)) {
      dv(); ldvb();
    } else {
      erreur("Déclaration de variable attendue après ','");
    }
  } else if (!est_suivant(_listeDecVariablesBis_, uniteCourante)) {
    erreur("Erreur de syntaxe");
  }
  affiche_balise_fermante("listeDecVariablesBis",XML);
}

void dv(void) {
  affiche_balise_ouvrante("declarationVariable",XML);
  printf("UC=%i yytext=%s valeur=%s\n", uniteCourante, yytext, valeur);
  if (uniteCourante == ENTIER) {
    uniteCourante = yylex();
    if (uniteCourante == ID_VAR) {
      uniteCourante = yylex();
      if (est_premier(_optTailleTableau_, uniteCourante)) {
        ott();
      } else if (!est_suivant(_declarationVariable_, uniteCourante)) {
        erreur("Erreur de syntaxe : n'est pas suivant");
      }
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
    uniteCourante = yylex();
    if (uniteCourante == NOMBRE) {
      uniteCourante = yylex();
      if (uniteCourante == CROCHET_FERMANT) {
        uniteCourante = yylex();
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
  if (est_premier(_declarationFonction_, uniteCourante)) {
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
    if (est_premier(_listeParam_, uniteCourante)) {
      lp(); odv(); ib();
    } else {
      erreur("Liste de paramètres attendus après identifiant de fonction");
    }
  } else
    erreur("Erreur de syntaxe");
    affiche_balise_fermante("declarationFonction",XML);
}

void lp(void) {
  affiche_balise_ouvrante("listeParam",XML);
  if (uniteCourante == PARENTHESE_OUVRANTE) {
    uniteCourante = yylex();
    if (est_premier(_optListeDecVariables_, uniteCourante)) {
      oldv();
    } else if (!est_suivant(_optListeDecVariables_, uniteCourante)) {
      erreur("Erreur de syntaxe");
    }
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
  if (est_premier(_listeDecVariables_, uniteCourante)) {
    ldv();
  } else if (!est_suivant(_optListeDecVariables_, uniteCourante)) {
    erreur("Erreur de syntaxe");
  }
  affiche_balise_fermante("optListeDecVariables",XML);
}

void i(void) {
  affiche_balise_ouvrante("instruction",XML);
  if (est_premier(_instructionAffect_, uniteCourante)) {
    uniteCourante = yylex();
    iaff();
  } else if (est_premier(_instructionBloc_, uniteCourante)) {
    uniteCourante = yylex();
    ib();
  } else if (est_premier(_instructionSi_, uniteCourante)) {
    uniteCourante = yylex();
    isi();
  } else if (est_premier(_instructionTantque_, uniteCourante)) {
    uniteCourante = yylex();
    itq();
  } else if (est_premier(_instructionAppel_, uniteCourante)) {
    uniteCourante = yylex();
    iapp();
  } else if (est_premier(_instructionRetour_, uniteCourante)) {
    uniteCourante = yylex();
    iret();
  } else if (est_premier(_instructionEcriture_, uniteCourante)) {
    uniteCourante = yylex();
    iecr();
  } else if (est_premier(_instructionVide_, uniteCourante)) {
    uniteCourante = yylex();
    ivide();
  } else {
    erreur("Erreur de syntaxe");
  }
  affiche_balise_fermante("instruction",XML);
}

void iaff(void) {
  affiche_balise_ouvrante("instructionAffect",XML);
  if (est_premier(_var_, uniteCourante)) {
    var();
    if (uniteCourante == EGAL) {
      uniteCourante = yylex();
      if (est_premier(_expression_, uniteCourante)) {
        Exp();
      } else {
        erreur("Expression attendue après '='");
      }
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
    if (est_premier(_listeInstructions_, uniteCourante)) {
      li();
    } else {
      erreur("Erreur de syntaxe");
    }
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
  if (est_premier(_instruction_, uniteCourante)) {
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
      Exp();
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
  if (uniteCourante == SINON) {
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
    Exp();
    if (uniteCourante == FAIRE) {
      uniteCourante = yylex();
      ib();
    } else
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
      Exp();
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
        Exp();
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

void Exp(void) {
  affiche_balise_ouvrante("expression",XML);
  if (est_premier(_conjonction_, uniteCourante)) {
    Conj(); expB();
  } else
    erreur("Erreur de syntaxe");
  affiche_balise_fermante("expression",XML);
}

void expB(void) {
  affiche_balise_ouvrante("expressionBis",XML);
  if (uniteCourante == OU) {
    uniteCourante = yylex();
    if (est_premier(_conjonction_, uniteCourante)) {
      Conj(); expB();
    } else {
      erreur("Conjonction attendue après '|'");
    }
  } else if (!est_suivant(_expressionBis_, uniteCourante)) {
    erreur("Erreur de syntaxe");
  }
  affiche_balise_fermante("expressionBis",XML);
}

void Conj(void) {
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

void comp(void) {
  affiche_balise_ouvrante("comparaison",XML);
  if (est_premier(_expArith_, uniteCourante)) {
    e(); compB();
  } else
    erreur("Erreur de syntaxe");
  affiche_balise_fermante("comparaison",XML);
}

void compB(void) {
  affiche_balise_ouvrante("comparaisonBis",XML);
  if (uniteCourante == EGAL || uniteCourante == INFERIEUR) {
    uniteCourante = yylex();
    if (est_premier(_expArith_, uniteCourante)) {
      e(); compB();
    } else {
      erreur("Expression arithmétique attendue après '=' ou '<'");
    }
  } else if (!est_suivant(_comparaisonBis_, uniteCourante)) {
    erreur("Erreur de syntaxe");
  }
  affiche_balise_fermante("comparaisonBis",XML);
}

void e(void) {
  affiche_balise_ouvrante("expArith",XML);
  if (est_premier(_terme_, uniteCourante)) {
    uniteCourante = yylex();
    t(); eB();
  } else {
    erreur("Erreur de syntaxe");
  }
  affiche_balise_fermante("expArith",XML);
}

void eB(void) {
  affiche_balise_ouvrante("expArithBis",XML);
  if (uniteCourante == PLUS || uniteCourante == MOINS) {
    uniteCourante = yylex();
    if (est_premier(_terme_, uniteCourante)) {
      t(); eB();
    } else {
      erreur("Terme attendu après '+' ou '-'");
    }
  } else if (!est_suivant(_expArithBis_, uniteCourante)) {
    erreur("Erreur de syntaxe");
  }
  affiche_balise_fermante("expArithBis",XML);
}

void t(void) {
  affiche_balise_ouvrante("terme",XML);
  if (est_premier(_negation_, uniteCourante)) {
    uniteCourante = yylex();
    neg(); tB();
  } else {
    erreur("Erreur de syntaxe");
  }
  affiche_balise_fermante("terme",XML);
}

void tB(void) {
  affiche_balise_ouvrante("termeBis",XML);
  if (uniteCourante == FOIS || uniteCourante == DIVISE) {
    uniteCourante = yylex();
    if (est_premier(_negation_, uniteCourante)) {
      neg(); tB();
    } else {
      erreur("Terme attendu après '*' ou '/'");
    }
  } else if (!est_suivant(_termeBis_, uniteCourante)) {
    erreur("Erreur de syntaxe");
  }
  affiche_balise_fermante("termeBis",XML);
}

void neg(void) {
  affiche_balise_ouvrante("negation",XML);
  if (uniteCourante == NON) {
    uniteCourante = yylex();
    if (est_premier(_negation_, uniteCourante)) {
      neg();
    } else {
      erreur("Negation attendue après '!'");
    }
  } else if (est_premier(_facteur_, uniteCourante)) {
    f();
  } else {
    erreur("Erreur de syntaxe");
  }
  affiche_balise_fermante("negation",XML);
}

void f(void) {
  affiche_balise_ouvrante("facteur",XML);
  if (uniteCourante == PARENTHESE_OUVRANTE) {
    uniteCourante = yylex();
    if (est_premier(_expression_, uniteCourante)) {
      Exp();
      if (uniteCourante == PARENTHESE_FERMANTE) {
        uniteCourante = yylex();
      } else {
        erreur("')' attendue après expression");
      }
    } else {
      erreur("Expression attendue après '('");
    }
  } else if (uniteCourante == NOMBRE) {
    uniteCourante = yylex();
  } else if (est_premier(_appelFct_, uniteCourante)) {
    appf();
  } else if (est_premier(_var_, uniteCourante)) {
    var();
  } else if (uniteCourante == LIRE) {
    uniteCourante = yylex();
    if (uniteCourante == PARENTHESE_OUVRANTE) {
      uniteCourante = yylex();
      if (uniteCourante == PARENTHESE_FERMANTE) {
        uniteCourante = yylex();
      } else {
        erreur("')' attendue après '('");
      }
    } else {
      erreur("'(' attendue après LIRE");
    }
  } else {
    erreur("Erreur de syntaxe");
  }
  affiche_balise_fermante("facteur",XML);
}

void var(void) {
  affiche_balise_ouvrante("var",XML);
  if (uniteCourante == ID_VAR) {
    uniteCourante = yylex();
    if (est_premier(_optIndice_, uniteCourante)) {
      oind();
    } else if (!est_suivant(_var_, uniteCourante)) {
      erreur("Erreur de syntaxe");
    }
  }
  affiche_balise_fermante("var",XML);
}

void oind(void) {
  affiche_balise_ouvrante("optIndice",XML);
  if (uniteCourante == CROCHET_OUVRANT) {
    uniteCourante = yylex();
    if (est_premier(_expression_, uniteCourante)) {
      Exp();
      if (uniteCourante == CROCHET_FERMANT) {
        uniteCourante = yylex();
      } else {
        erreur("']' attendu après expression");
      }
    } else {
      erreur("Expression attendue après '['");
    }
  } else if (!est_suivant(_optIndice_, uniteCourante)) {
    erreur("Erreur de syntaxe");
  }
  affiche_balise_fermante("optIndice",XML);
}

void appf(void) {
  affiche_balise_ouvrante("appelFct",XML);
  if (uniteCourante == ID_FCT) {
    uniteCourante = yylex();
    if (uniteCourante == PARENTHESE_OUVRANTE) {
      uniteCourante = yylex();
      if (est_premier(_listeExpressions_, uniteCourante)) {
        lexp();
        if (uniteCourante == PARENTHESE_FERMANTE) {
          uniteCourante = yylex();
        } else {
          erreur("')' attendue");
        }
      } else {
        erreur("Erreur de syntaxe");
      }
    } else {
      erreur("'(' attendue");
    }
  } else {
    erreur("Erreur de syntaxe");
  }
  affiche_balise_fermante("appelFct",XML);
}

void lexp(void) {
  affiche_balise_ouvrante("listeExpressions",XML);
  if (est_premier(_expression_, uniteCourante)) {
    Exp(); lexpB();
  } else if(!est_suivant(_listeExpressions_, uniteCourante)) {
    erreur("Erreur de syntaxe");
  }
  affiche_balise_fermante("listeExpressions",XML);
}

void lexpB(void) {
  affiche_balise_ouvrante("listeExpressionsBis",XML);
  if (uniteCourante == VIRGULE) {
    uniteCourante = yylex();
    if (est_premier(_expression_, uniteCourante)) {
      Exp(); lexpB();
    } else {
      erreur("Expression attendue après ','");
    }
  } else if(!est_suivant(_listeExpressionsBis_, uniteCourante)) {
    erreur("Erreur de syntaxe");
  }

  affiche_balise_fermante("listeExpressionsBis",XML);
}

int main (int argc, char **argv) {
  int i,j;

  yyin = fopen(argv[1], "r");
  if (yyin == NULL) {
    fprintf(stderr, "Impossible d'ouvrir le fichier %s\n", argv[1]);
    exit(1);
  }
  initialise_premiers();
  initialise_suivants();

  printf("suivant(%i, %i)?%i\n",_declarationVariable_, VIRGULE, est_suivant(_declarationVariable_, VIRGULE));
  for(i=0; i <= NB_NON_TERMINAUX; i++) {
    for(j=0; j <= NB_TERMINAUX; j++)
      printf("%i ",suivants[i][j]);
    printf("\n");
  }


  uniteCourante=yylex();
  pg();
  printf("SYN: Analyse syntaxique terminée avec succès\n");
  return 0;
}
