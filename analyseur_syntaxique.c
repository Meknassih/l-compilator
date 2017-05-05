#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/symboles.h"
#include "headers/util.h"
#include "headers/analyseur_syntaxique.h"
#include "headers/analyseur_lexical.h"
#include "headers/premiers.h"
#include "headers/suivants.h"
#include "headers/syntabs.h"

short int uniteCourante;
char yytext[100];
FILE *yyin;
char nom[100];
char valeur[100];
int XML = 0;

void pg(void) { //Axiome
  affiche_balise_ouvrante("programme",XML);
  odv(); ldf();
  affiche_balise_fermante("programme",XML);
  return;
}

void odv(void)  {
  affiche_balise_ouvrante("optDecVariables",XML);
  if (est_premier(_listeDecVariables_, uniteCourante)) {
    ldv();
    if (uniteCourante == POINT_VIRGULE) {
      nom_token(uniteCourante, nom, valeur);
      affiche_element(nom, valeur, XML);
      uniteCourante = yylex();
    }
  }
  affiche_balise_fermante("optDecVariables",XML);
}

void ldv(void) {
  affiche_balise_ouvrante("listeDecVariables",XML);
  if (est_premier(_declarationVariable_, uniteCourante)) {
    dv(); ldvb();
  } else
    erreur("Erreur de syntaxe 2 ");
  affiche_balise_fermante("listeDecVariables",XML);
}

void ldvb(void) {
  affiche_balise_ouvrante("listeDecVariablesBis",XML);
  if (uniteCourante == VIRGULE) {
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    uniteCourante = yylex();
    if (est_premier(_declarationVariable_, uniteCourante)) {
      dv(); ldvb();
    } else {
      erreur("Déclaration de variable attendue après ','");
    }
  } else if (est_suivant(_listeDecVariablesBis_, uniteCourante)) {
    erreur("Erreur de syntaxe");
  }
  affiche_balise_fermante("listeDecVariablesBis",XML);
}

void dv(void) {
  affiche_balise_ouvrante("declarationVariable",XML);
  if (uniteCourante == ENTIER) {
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    uniteCourante = yylex();
    if (uniteCourante == ID_VAR) {
      nom_token(uniteCourante, nom, valeur);
      affiche_element(nom, valeur, XML);
      uniteCourante = yylex();
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
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    uniteCourante = yylex();
    if (uniteCourante == NOMBRE) {
      nom_token(uniteCourante, nom, valeur);
      affiche_element(nom, valeur, XML);
      uniteCourante = yylex();
      if (uniteCourante == CROCHET_FERMANT) {
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, XML);
        uniteCourante = yylex();
      } else
        erreur("Crochet fermant attendu après indice de tableau");
    } else
      erreur("Indince de tableau attendu après crochet ouvrant");
  }
  affiche_balise_fermante("optTailleTableau",XML);
}

void ldf(void) {
  affiche_balise_ouvrante("listeDecFonctions",XML);
  if (est_premier(_declarationFonction_, uniteCourante)) {
    df(); ldf();
  }
  affiche_balise_fermante("listeDecFonctions",XML);
}

void df(void) {
  affiche_balise_ouvrante("declarationFonction",XML);
  if (uniteCourante == ID_FCT) {
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
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
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    uniteCourante = yylex();
    oldv();
    if (uniteCourante == PARENTHESE_FERMANTE) {
      nom_token(uniteCourante, nom, valeur);
      affiche_element(nom, valeur, XML);
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
  }
  affiche_balise_fermante("optListeDecVariables",XML);
}

void i(void) {
  affiche_balise_ouvrante("instruction",XML);
  if (est_premier(_instructionAffect_, uniteCourante)) {
    iaff();
  } else if (est_premier(_instructionBloc_, uniteCourante)) {
    ib();
  } else if (est_premier(_instructionSi_, uniteCourante)) {
    isi();
  } else if (est_premier(_instructionTantque_, uniteCourante)) {
    itq();
  } else if (est_premier(_instructionAppel_, uniteCourante)) {
    iapp();
  } else if (est_premier(_instructionRetour_, uniteCourante)) {
    iret();
  } else if (est_premier(_instructionEcriture_, uniteCourante)) {
    iecr();
  } else if (est_premier(_instructionVide_, uniteCourante)) {
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
      nom_token(uniteCourante, nom, valeur);
      affiche_element(nom, valeur, XML);
      uniteCourante = yylex();
      if (est_premier(_expression_, uniteCourante)) {
        Exp();
      } else {
        erreur("Expression attendue après '='");
      }
      if (uniteCourante == POINT_VIRGULE) {
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, XML);
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
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    uniteCourante = yylex();
    li();
    if (uniteCourante == ACCOLADE_FERMANTE) {
      nom_token(uniteCourante, nom, valeur);
      affiche_element(nom, valeur, XML);
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
  }
  affiche_balise_fermante("listeInstructions",XML);
}
//USE est_premier() ABOVE THIS COMMENT
void isi(void) {
  affiche_balise_ouvrante("instructionSi",XML);
  if (uniteCourante == SI) {
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    uniteCourante = yylex();
    if (est_premier(_expression_, uniteCourante)) { //?
      Exp();
      if (uniteCourante == ALORS) {
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, XML);
        uniteCourante = yylex();
        ib(); osinon();
      } else
        erreur("ALORS manquant");
    } else
      erreur("Erreur de syntaxe");
  } else
    erreur("Erreur de syntaxe");

  affiche_balise_fermante("instructionSi",XML);
}

void osinon(void) {
  affiche_balise_ouvrante("optSinon",XML);
  if (uniteCourante == SINON) {
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    uniteCourante = yylex();
    ib();
  }
  affiche_balise_fermante("optSinon",XML);
}

void itq(void) {
  affiche_balise_ouvrante("instructionTantque",XML);
  if (uniteCourante == TANTQUE) {
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    uniteCourante = yylex();
    Exp();
    if (uniteCourante == FAIRE) {
      nom_token(uniteCourante, nom, valeur);
      affiche_element(nom, valeur, XML);
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
      nom_token(uniteCourante, nom, valeur);
      affiche_element(nom, valeur, XML);
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
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    uniteCourante = yylex();
    if (est_premier(_expression_, uniteCourante)) {
      Exp();
      if (uniteCourante == POINT_VIRGULE) {
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, XML);
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
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    uniteCourante = yylex();
    if (uniteCourante == PARENTHESE_OUVRANTE) {
      nom_token(uniteCourante, nom, valeur);
      affiche_element(nom, valeur, XML);
      uniteCourante = yylex();
      if (est_premier(_expression_, uniteCourante)) {
        Exp();
        if (uniteCourante == PARENTHESE_FERMANTE) {
          nom_token(uniteCourante, nom, valeur);
          affiche_element(nom, valeur, XML);
          uniteCourante = yylex();
          if (uniteCourante == POINT_VIRGULE) {
            nom_token(uniteCourante, nom, valeur);
            affiche_element(nom, valeur, XML);
            uniteCourante = yylex();
          } else
            erreur("';' manquant");
        } else
          erreur("')' manquant");
      } else
        erreur("Erreur de syntaxe : expression attendue");
    } else
      erreur("'(' manquant");
  } else
    erreur("Erreur de syntaxe");

  affiche_balise_fermante("instructionEcriture",XML);
}

void ivide(void) {
  affiche_balise_ouvrante("instructionVide",XML);
  if (uniteCourante == POINT_VIRGULE) {
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
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
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    uniteCourante = yylex();
    if (est_premier(_conjonction_, uniteCourante)) {
      Conj(); expB();
    } else {
      erreur("Conjonction attendue après '|'");
    }
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
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    uniteCourante = yylex();
    if (est_premier(_comparaison_, uniteCourante)) {
      comp(); conjB();
    } else {
      erreur("Comparaison attendue après '&'");
    }
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
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    uniteCourante = yylex();
    if (est_premier(_expArith_, uniteCourante)) {
      e(); compB();
    } else {
      erreur("Expression arithmétique attendue après '=' ou '<'");
    }
  }
  affiche_balise_fermante("comparaisonBis",XML);
}

void e(void) {
  affiche_balise_ouvrante("expArith",XML);
  if (est_premier(_terme_, uniteCourante)) {
    t(); eB();
  } else {
    erreur("Erreur de syntaxe");
  }
  affiche_balise_fermante("expArith",XML);
}

void eB(void) {
  affiche_balise_ouvrante("expArithBis",XML);
  if (uniteCourante == PLUS || uniteCourante == MOINS) {
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    uniteCourante = yylex();
    if (est_premier(_terme_, uniteCourante)) {
      t(); eB();
    } else {
      erreur("Terme attendu après '+' ou '-'");
    }
  }
  affiche_balise_fermante("expArithBis",XML);
}

void t(void) {
  affiche_balise_ouvrante("terme",XML);
  if (est_premier(_negation_, uniteCourante)) {
    neg(); tB();
  } else {
    erreur("Erreur de syntaxe");
  }
  affiche_balise_fermante("terme",XML);
}

void tB(void) {
  affiche_balise_ouvrante("termeBis",XML);
  if (uniteCourante == FOIS || uniteCourante == DIVISE) {
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    uniteCourante = yylex();
    if (est_premier(_negation_, uniteCourante)) {
      neg(); tB();
    } else {
      erreur("Terme attendu après '*' ou '/'");
    }
  }
  affiche_balise_fermante("termeBis",XML);
}

void neg(void) {
  affiche_balise_ouvrante("negation",XML);
  if (uniteCourante == NON) {
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
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

n_exp *f(void) {
  n_exp *S2 = NULL;
  n_appel *S1 = NULL;
  n_var *S3 = NULL;
  char *fct = "facteur";
  affiche_balise_ouvrante(fct,XML);
  if (uniteCourante == PARENTHESE_OUVRANTE) {
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    uniteCourante = yylex();
    if (est_premier(_expression_, uniteCourante)) {
      S2 = Exp();
      if (uniteCourante == PARENTHESE_FERMANTE) {
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, XML);
        uniteCourante = yylex();
      } else {
        erreur("')' attendue après expression");
      }
    } else {
      erreur("Expression attendue après '('");
    }
  } else if (uniteCourante == NOMBRE) {
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    S2 = cree_n_exp_entier(valeur);
    uniteCourante = yylex();
  } else if (est_premier(_appelFct_, uniteCourante)) {
    S1 = appf();
    S2 = cree_n_exp_appel(S1);
  } else if (est_premier(_var_, uniteCourante)) {
    S3 = var();
    S2 = cree_n_exp_var(S3);
  } else if (uniteCourante == LIRE) {
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    uniteCourante = yylex();
    if (uniteCourante == PARENTHESE_OUVRANTE) {
      nom_token(uniteCourante, nom, valeur);
      affiche_element(nom, valeur, XML);
      uniteCourante = yylex();
      if (uniteCourante == PARENTHESE_FERMANTE) {
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, XML);
        S2 = cree_n_exp_lire();
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
  affiche_balise_fermante(fct,XML);
  return S2;
}

n_var *var(void) {
  n_var *SS = NULL;
  n_exp *S3 = NULL;
  char *fct = "var";
  affiche_balise_ouvrante(fct,XML);
  if (uniteCourante == ID_VAR) {
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    uniteCourante = yylex();
    if (est_premier(_optIndice_, uniteCourante)) {
      S3 = oind();
      SS = cree_n_var_indicee(nom, S3);
    } else if (est_suivant(_var_, uniteCourante)) {
      SS = cree_n_var_simple(nom);
    }
  }
  affiche_balise_fermante(fct,XML);
  return SS;
}

n_exp *oind(void) {
  n_exp *S2 = NULL;
  char *fct = "optIndice";
  affiche_balise_ouvrante(fct,XML);
  if (uniteCourante == CROCHET_OUVRANT) {
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    uniteCourante = yylex();
    if (est_premier(_expression_, uniteCourante)) {
      S2 = Exp();
      if (uniteCourante == CROCHET_FERMANT) {
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, XML);
        uniteCourante = yylex();
      } else {
        erreur("']' attendu après expression");
      }
    } else {
      erreur("Expression attendue après '['");
    }
  }
  affiche_balise_fermante(fct,XML);
  return S2;
}

n_appel *appf(void) {
  n_appel *SS = NULL;
  n_l_exp *S2 = NULL;
  char *nom_fct;
  char *fct = "appelFct";
  affiche_balise_ouvrante(fct,XML);
  if (uniteCourante == ID_FCT) {
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    strcpy(nom_fct, nom);
    uniteCourante = yylex();
    if (uniteCourante == PARENTHESE_OUVRANTE) {
      nom_token(uniteCourante, nom, valeur);
      affiche_element(nom, valeur, XML);
      uniteCourante = yylex();
      if (est_premier(_listeExpressions_, uniteCourante)) {
        S2 = lexp();
        if (uniteCourante == PARENTHESE_FERMANTE) {
          nom_token(uniteCourante, nom, valeur);
          affiche_element(nom, valeur, XML);
          uniteCourante = yylex();
          SS = cree_n_appel(nom_fct, S2);
        } else {
          erreur("')' attendue");
        }
      } else {
        erreur("')' attendue");
      }
    } else {
      erreur("'(' attendue");
    }
  } else {
    erreur("Erreur de syntaxe");
  }
  affiche_balise_fermante(fct,XML);
  return SS;
}

n_l_exp lexp(void) {
  n_l_exp *SS = NULL;
  n_exp *S2 = NULL;
  n_l_exp *S3 = NULL;
  char *fct="listeExpressions";
  affiche_balise_ouvrante(fct,XML);
  if (est_premier(_expression_, uniteCourante)) {
    S2 = Exp(); S3 = lexpB();
    SS = cree_n_l_exp(S2, S3);
  } else if(!est_suivant(_listeExpressions_, uniteCourante)) {
    erreur("Erreur de syntaxe");
  }
  affiche_balise_fermante(fct,XML);
  return SS;
}

n_l_exp *lexpB(void) {
  n_l_exp *SS = NULL;
  n_l_exp *S2 = NULL;
  n_exp *S3 = NULL;
  char *fct="listeExpressionsBis";
  affiche_balise_ouvrante(fct,XML);
  if (uniteCourante == VIRGULE) {
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    uniteCourante = yylex();
    if (est_premier(_expression_, uniteCourante)) {
      S2 = Exp(); S3 = lexpB();
      SS = cree_n_l_exp(S2, S3);
    } else {
      erreur("Expression attendue après ','");
    }
  } else if(!est_suivant(_listeExpressionsBis_, uniteCourante)) {
    erreur("Erreur de syntaxe");
  }
  return SS;
  affiche_balise_fermante(fct,XML);
}

int main (int argc, char **argv) {
  int op1 = 0;
  if (argc > 2) {
    op1 = 1;
    if (strcmp(argv[1],"-l") == 0) {
      yyin = fopen(argv[2], "r");
      if (yyin == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier %s\n", argv[2]);
        exit(1);
      }
      test_yylex_internal(yyin);
      return 0;
    } else if (strcmp(argv[1],"-s") == 0) {
      XML = 1;
    } else {
      fprintf(stderr, "Arguments incorrects\n");
      exit(1);
    }
  }

  if (op1) {
    yyin = fopen(argv[2], "r");
  } else {
    yyin = fopen(argv[1], "r");
  }

  if (yyin == NULL) {
    fprintf(stderr, "Impossible d'ouvrir le fichier %s\n", argv[1]);
    exit(1);
  }
  initialise_premiers();
  initialise_suivants();

  uniteCourante=yylex();
  pg();
  if (!op1) //Pour ne pas interférer avec la sortie XML
    printf("SYN: Analyse syntaxique terminée avec succès\n");

  return 0;
}
