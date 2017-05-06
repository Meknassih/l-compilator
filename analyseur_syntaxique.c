#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/symboles.h"
#include "headers/util.h"
#include "headers/syntabs.h"
#include "headers/analyseur_syntaxique.h"
#include "headers/analyseur_lexical.h"
#include "headers/premiers.h"
#include "headers/suivants.h"
#include "headers/affiche_arbre_abstrait.h"

short int uniteCourante;
char yytext[100];
FILE *yyin;
char nom[100];
char valeur[100];
int XML = 0;
int ABS = 0;

n_prog * pg(void) { //Axiome
  n_l_dec * S1 = NULL;
  n_l_dec * S2 = NULL;
  n_prog * SS = NULL;

  affiche_balise_ouvrante("programme",XML);
  S1 = odv();
  S2 = ldf();
  SS = cree_n_prog(S1, S2);

  affiche_balise_fermante("programme",XML);
  return SS;
}

n_l_dec * odv(void)  {
  n_l_dec * SS = NULL;
  affiche_balise_ouvrante("optDecVariables",XML);

  if (est_premier(_listeDecVariables_, uniteCourante)) {
    SS = ldv();
    if (uniteCourante == POINT_VIRGULE) {
      nom_token(uniteCourante, nom, valeur);
      affiche_element(nom, valeur, XML);
      uniteCourante = yylex();
    }
  }

  affiche_balise_fermante("optDecVariables",XML);
  return SS;
}

n_l_dec * ldv(void) {
  n_dec * S1 = NULL;
  n_l_dec * S2 = NULL;
  n_l_dec * SS = NULL;
  affiche_balise_ouvrante("listeDecVariables",XML);

  if (est_premier(_declarationVariable_, uniteCourante)) {
    S1 = dv();
    S2 = ldvb();
    SS = cree_n_l_dec(S1, S2);

  } else
    erreur("Erreur de syntaxe");

  affiche_balise_fermante("listeDecVariables",XML);
  return SS;

}

n_l_dec * ldvb(void) {
  n_l_dec * SS = NULL;
  n_dec * S1 = NULL;
  n_l_dec * S2 = NULL;
  affiche_balise_ouvrante("listeDecVariablesBis",XML);

  if (uniteCourante == VIRGULE) {
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    uniteCourante = yylex();
    if (est_premier(_declarationVariable_, uniteCourante)) {
      S1 = dv();
      S2 = ldvb();
      SS = cree_n_l_dec(S1, S2);
    } else {
      erreur("Déclaration de variable attendue après ','");
    }
  } else if (est_suivant(_listeDecVariablesBis_, uniteCourante)) {
    erreur("Erreur de syntaxe");
  }

  affiche_balise_fermante("listeDecVariablesBis",XML);
  return SS;
}

n_dec * dv(void) { // a revoir avec hamza
  n_dec * SS = NULL;
  int t = 0;
  char *nom_var;

  affiche_balise_ouvrante("declarationVariable",XML);

  if (uniteCourante == ENTIER) {
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    uniteCourante = yylex();
    if (uniteCourante == ID_VAR) {
      nom_token(uniteCourante, nom, valeur);
      affiche_element(nom, valeur, XML);
      nom_var = strdup(valeur);
      uniteCourante = yylex();
      t = ott();
      if(est_premier(uniteCourante, _optTailleTableau_))
        SS = cree_n_dec_tab(nom_var, t);
      else
        SS = cree_n_dec_var(nom_var);
    } else {
      erreur("Identifiant de variable attendu après ENTIER");
    }
  } else {
    erreur("Erreur de syntaxe");
  }

  affiche_balise_fermante("declarationVariable",XML);
  return SS;

}

int ott(void) { // a revoir avec hamza
  int t = 0;
  affiche_balise_ouvrante("optTailleTableau",XML);

  if (uniteCourante == CROCHET_OUVRANT) {
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    uniteCourante = yylex();
    if (uniteCourante == NOMBRE) {
      nom_token(uniteCourante, nom, valeur);
      affiche_element(nom, valeur, XML);
      t = atoi(yytext);
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
  return t;
}

n_l_dec * ldf(void) {
	n_l_dec * SS = NULL;
	n_dec * S1 = NULL;
	n_l_dec * S2 = NULL;
  affiche_balise_ouvrante("listeDecFonctions",XML);

  if (est_premier(_declarationFonction_, uniteCourante)) {
    S1 = df();
    S2 = ldf();
    SS = cree_n_l_dec(S1, S2);
  }

  affiche_balise_fermante("listeDecFonctions",XML);
  return SS;
}

n_dec * df(void) {
  n_dec *SS = NULL;
  n_l_dec *S1 = NULL;
  n_l_dec *S2 = NULL;
  n_instr *S3 = NULL;
  affiche_balise_ouvrante("declarationFonction",XML);

  if (uniteCourante == ID_FCT) {
    char *fonction = malloc(sizeof(char));
    strcpy (fonction, yytext);
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    uniteCourante = yylex();
    if (est_premier(_listeParam_, uniteCourante)) {
      S1 = lp();
  		S2 = odv();
  		S3 = ib();
  		SS = cree_n_dec_fonc(fonction, S1, S2, S3);
    } else {
      erreur("Liste de paramètres attendus après identifiant de fonction");
    }
  } else
    erreur("Erreur de syntaxe");

  affiche_balise_fermante("declarationFonction",XML);
  return SS;
}

n_l_dec * lp(void) {
  n_l_dec * SS = NULL;
  affiche_balise_ouvrante("listeParam",XML);

  if (uniteCourante == PARENTHESE_OUVRANTE) {
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    uniteCourante = yylex();
    SS = oldv();
    if (uniteCourante == PARENTHESE_FERMANTE) {
      nom_token(uniteCourante, nom, valeur);
      affiche_element(nom, valeur, XML);
      uniteCourante = yylex();
    } else
      erreur("Parenthèse fermante attendue à la fin de la liste de paramètres");
  } else
    erreur("Erreur de syntaxe");

  affiche_balise_fermante("listeParam",XML);
  return SS;
}

n_l_dec * oldv(void) {
  n_l_dec *SS = NULL;
  affiche_balise_ouvrante("optListeDecVariables",XML);

  if (est_premier(_listeDecVariables_, uniteCourante)) {
    SS = ldv();
  }

  affiche_balise_fermante("optListeDecVariables",XML);
  return SS;
}

n_instr * i(void) {
  n_instr * SS = NULL;
  affiche_balise_ouvrante("instruction",XML);

  if (est_premier(_instructionAffect_, uniteCourante)) {
    SS = iaff();
  } else if (est_premier(_instructionBloc_, uniteCourante)) {
    SS = ib();
  } else if (est_premier(_instructionSi_, uniteCourante)) {
    SS = isi();
  } else if (est_premier(_instructionTantque_, uniteCourante)) {
    SS = itq();
  } else if (est_premier(_instructionAppel_, uniteCourante)) {
    SS = iapp();
  } else if (est_premier(_instructionRetour_, uniteCourante)) {
    SS = iret();
  } else if (est_premier(_instructionEcriture_, uniteCourante)) {
    SS = iecr();
  } else if (est_premier(_instructionVide_, uniteCourante)) {
    SS = ivide();
  } else {
    erreur("Erreur de syntaxe");
  }

  affiche_balise_fermante("instruction",XML);
  return SS;
}

n_instr * iaff(void) {
  n_instr *SS = NULL;
  n_var *S1 = NULL;
  n_exp *S2 = NULL;
  affiche_balise_ouvrante("instructionAffect",XML);

  if (est_premier(_var_, uniteCourante)) {
    S1 = var();
    if (uniteCourante == EGAL) {
      nom_token(uniteCourante, nom, valeur);
      affiche_element(nom, valeur, XML);
      uniteCourante = yylex();
      if (est_premier(_expression_, uniteCourante)) {
        S2 = Exp();
      } else {
        erreur("Expression attendue après '='");
      }
      if (uniteCourante == POINT_VIRGULE) {
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, XML);
        uniteCourante = yylex();
        SS = cree_n_instr_affect(S1, S2);
      } else
        erreur("';' manquant");
    } else {
      erreur("Erreur de syntaxe");
    }
  } else {
    erreur("Erreur de syntaxe");
  }

  affiche_balise_fermante("instructionAffect",XML);
  return SS;
}

n_instr * ib(void) {
  n_instr *SS = NULL;
  n_l_instr *S1 = NULL;
  affiche_balise_ouvrante("instructionBloc",XML);

  if (uniteCourante == ACCOLADE_OUVRANTE) {
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    uniteCourante = yylex();
    S1 = li();
    if (uniteCourante == ACCOLADE_FERMANTE) {
      nom_token(uniteCourante, nom, valeur);
      affiche_element(nom, valeur, XML);
      uniteCourante = yylex();
      SS = cree_n_instr_bloc(S1);
    } else {
      erreur("Accolade fermante manquante");
    }
  } else {
    erreur("Accolade ouvrante manquante");
  }

  affiche_balise_fermante("instructionBloc",XML);
  return SS;
}

n_l_instr * li(void) {
  n_l_instr *SS = NULL;
  n_instr *S1 = NULL;
  n_l_instr *S2 = NULL;
  affiche_balise_ouvrante("listeInstructions",XML);

  if (est_premier(_instruction_, uniteCourante)) {
    S1 = i();
    S2 = li();
    SS = cree_n_l_instr(S1, S2);
  }

  affiche_balise_fermante("listeInstructions",XML);
  return SS;
}

//USE est_premier() ABOVE THIS COMMENT
n_instr * isi(void) {
  n_instr * SS = NULL;
  n_exp * S1 = NULL;
  n_instr * S2 = NULL;
  n_instr * S3 = NULL;
  affiche_balise_ouvrante("instructionSi",XML);

  if (uniteCourante == SI) {
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    uniteCourante = yylex();
    if (est_premier(_expression_, uniteCourante)) { //?
      S1 = Exp();
      if (uniteCourante == ALORS) {
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, XML);
        uniteCourante = yylex();
        S2 = ib();
        S3 = osinon();
        SS = cree_n_instr_si(S1, S2, S3);
      } else
        erreur("ALORS manquant");
    } else
      erreur("Erreur de syntaxe");
  } else
    erreur("Erreur de syntaxe");

  affiche_balise_fermante("instructionSi",XML);
  return SS;
}

n_instr * osinon(void) {
  n_instr * SS = NULL;
  affiche_balise_ouvrante("optSinon",XML);

  if (uniteCourante == SINON) {
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    uniteCourante = yylex();
    SS = ib();
  }

  affiche_balise_fermante("optSinon",XML);
  return SS;
}

n_instr * itq(void) {
  n_instr *SS = NULL;
  n_exp *S1 = NULL;
  n_instr *S2 = NULL;
  affiche_balise_ouvrante("instructionTantque",XML);

  if (uniteCourante == TANTQUE) {
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    uniteCourante = yylex();
    S1 = Exp();
    if (uniteCourante == FAIRE) {
      nom_token(uniteCourante, nom, valeur);
      affiche_element(nom, valeur, XML);
      uniteCourante = yylex();
      S2 = ib();
      SS = cree_n_instr_tantque(S1, S2);
    } else
      erreur("FAIRE manquant");
  } else
    erreur("Erreur de syntaxe");

  affiche_balise_fermante("instructionTantque",XML);
  return SS;

}

n_instr * iapp(void) {
  n_instr * SS = NULL;
  n_appel * S1 = NULL;
  affiche_balise_ouvrante("instructionAppel",XML);

  if (est_premier(_appelFct_, uniteCourante)) {
    S1 = appf();
    if (uniteCourante == POINT_VIRGULE) {
      nom_token(uniteCourante, nom, valeur);
      affiche_element(nom, valeur, XML);
      uniteCourante = yylex();
      SS = cree_n_instr_appel(S1);
    } else
      erreur("';' manquant");
  } else {
    erreur("Erreur de syntaxe");
  }

  affiche_balise_fermante("instructionAppel",XML);
  return SS;
}

n_instr * iret(void) {
  n_instr *SS = NULL;
  n_exp *S1 = NULL;
  affiche_balise_ouvrante("instructionRetour",XML);

  if (uniteCourante == RETOUR) {
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    uniteCourante = yylex();
    if (est_premier(_expression_, uniteCourante)) {
      S1 = Exp();
      if (uniteCourante == POINT_VIRGULE) {
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, XML);
        uniteCourante = yylex();
        SS = cree_n_instr_retour(S1);
      } else {
        erreur("';' manquant");
      }
    } else
      erreur("Erreur de syntaxe");
  } else
    erreur("Erreur de syntaxe");

  affiche_balise_fermante("instructionRetour",XML);
  return SS;
}

n_instr * iecr(void) {
  n_instr *SS = NULL;
  n_exp *S1 = NULL;
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
        S1 = Exp();
        if (uniteCourante == PARENTHESE_FERMANTE) {
          nom_token(uniteCourante, nom, valeur);
          affiche_element(nom, valeur, XML);
          uniteCourante = yylex();
          if (uniteCourante == POINT_VIRGULE) {
            nom_token(uniteCourante, nom, valeur);
            affiche_element(nom, valeur, XML);
            uniteCourante = yylex();
            SS = cree_n_instr_ecrire(S1);
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
  return SS;
}

n_instr * ivide(void) {
  n_instr *SS = NULL;
  affiche_balise_ouvrante("instructionVide",XML);

  if (uniteCourante == POINT_VIRGULE) {
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    uniteCourante = yylex();
    SS = cree_n_instr_vide();
  } else
    erreur("Erreur de syntaxe");

  affiche_balise_fermante("instructionVide",XML);
  return SS;
}

n_exp * Exp(void) { // a voir avec hamza
  n_exp *S1 = NULL;
  n_exp *S2 = NULL;
  affiche_balise_ouvrante("expression",XML);

  if (est_premier(_conjonction_, uniteCourante)) {
    S1 = Conj();
    S2 = expB(S1);
  } else
    erreur("Erreur de syntaxe");

  affiche_balise_fermante("expression",XML);
  if(S2 == NULL)
    return S1;
  return S2;
}

n_exp * expB(n_exp *herite) { // a voir avec hamza
  n_exp * S1 = NULL;
  n_exp * SC = NULL; //child
  n_exp * SS = NULL;
  affiche_balise_ouvrante("expressionBis",XML);

  if (uniteCourante == OU) {
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    uniteCourante = yylex();

    if (est_premier(_conjonction_, uniteCourante)) {
      S1 = Conj();
      SC = cree_n_exp_op(ou, herite, S1);
      SS = expB(SC);
    } else {
      erreur("Conjonction attendue après '|'");
    }
  } else if(est_suivant(uniteCourante, _expressionBis_)){
  		SS = NULL;
  }

  affiche_balise_fermante("expressionBis",XML);
  if(SS == NULL)
    return SC;
  return SS;
}

n_exp * Conj(void) {
  n_exp *S1 = NULL;
  n_exp *S2 = NULL;
  affiche_balise_ouvrante("conjonction",XML);

  if (est_premier(_comparaison_, uniteCourante)) {
    S1 = comp();
    S2 = conjB(S1);
  } else
    erreur("Erreur de syntaxe");

  affiche_balise_fermante("conjonction",XML);
  if(S2 == NULL)
    return S1;
  return S2;
}

n_exp * conjB(n_exp *herite) {
  n_exp * S1 = NULL;
  n_exp * SC = NULL; // child
  n_exp * SS = NULL;

  affiche_balise_ouvrante("conjonctionBis",XML);
  if (uniteCourante == ET) {
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    uniteCourante = yylex();
    if (est_premier(_comparaison_, uniteCourante)) {
      S1 = comp();
      SC = cree_n_exp_op(et, SC, S1);
      SS = conjB(SC);
    } else {
      erreur("Comparaison attendue après '&'");
    }
  } else if(est_suivant(uniteCourante, _conjonctionBis_)){
  		SS = herite;
  	}

  affiche_balise_fermante("conjonctionBis",XML);
  if(SS == NULL)
    return SC;
  return SS;
}

n_exp * comp(void) { // A revoir avec hamza
  n_exp * S1 = NULL;
  n_exp * S2 = NULL;
  affiche_balise_ouvrante("comparaison",XML);

  if (est_premier(_expArith_, uniteCourante)) {
    S1 = e();
    S2 = compB(S1);
  } else
    erreur("Erreur de syntaxe");

  affiche_balise_fermante("comparaison",XML);
  if(S2 == NULL)
    return S1;
  return S2;
}

n_exp * compB(n_exp *herite) { // A revoir avec hamza
  n_exp * S1 = NULL;
  n_exp * SC = NULL;
  n_exp * SS = NULL;
  affiche_balise_ouvrante("comparaisonBis",XML);

  if (uniteCourante == EGAL) {
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    uniteCourante = yylex();
    if (est_premier(_expArith_, uniteCourante)) {
      S1 = e();
      SC = cree_n_exp_op(egal,herite,S1);
      SS = compB(SC);
    } else {
      erreur("Expression arithmétique attendue après '=' ou '<'");
    }
  } else if (uniteCourante == INFERIEUR) {
      nom_token(uniteCourante, nom, valeur);
      affiche_element(nom, valeur, XML);
      uniteCourante = yylex();
      if (est_premier(_expArith_, uniteCourante)) {
        S1 = e();
        SC = cree_n_exp_op(inf,herite,S1);
        SS = compB(SC);
      } else {
        erreur("Expression arithmétique attendue après '=' ou '<'");
      }
    } else if(est_suivant(uniteCourante, _comparaisonBis_)){
    		SS = herite;
    	}

  affiche_balise_fermante("comparaisonBis",XML);
  if(SS == NULL)
    return SC;
  return SS;
}

n_exp * e(void) {
  n_exp *S1 = NULL;
  n_exp *S2 = NULL;
  affiche_balise_ouvrante("expArith",XML);
  if (est_premier(_terme_, uniteCourante)) {
    S1 = t();
    S2 = eB(S1);
  } else {
    erreur("Erreur de syntaxe");
  }

  affiche_balise_fermante("expArith",XML);
  if(S2 == NULL)
    return S1;
  return S2;
}

n_exp * eB(n_exp *herite){ // voir avec hamza
  n_exp *SS = NULL;
  n_exp *S1 = NULL;
  n_exp *SC =NULL;
  affiche_balise_ouvrante("expArithBis",XML);

  if (uniteCourante == PLUS) {
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    uniteCourante = yylex();
    if (est_premier(_terme_, uniteCourante)) {
      S1 = t();
      SC = cree_n_exp_op(plus, herite, S1);
      SS = eB(SC);
    } else {
      erreur("Terme attendu après '+' ou '-'");
    }
  } else if (uniteCourante == MOINS) {
      nom_token(uniteCourante, nom, valeur);
      affiche_element(nom, valeur, XML);
      uniteCourante = yylex();
      if (est_premier(_terme_, uniteCourante)) {
        S1 = t();
        SC = cree_n_exp_op(moins, herite, S1);
        SS = eB(SC);
      } else {
        erreur("Terme attendu après '+' ou '-'");
      }
    } else if (est_suivant(uniteCourante, _expArithBis_)){
  		SS = herite;
  	}

  affiche_balise_fermante("expArithBis",XML);
  if(SS == NULL)
    return SC;
  return SS;
}

n_exp * t(void) {
  n_exp *S1 = NULL;
  n_exp *S2 = NULL;
  affiche_balise_ouvrante("terme",XML);

  if (est_premier(_negation_, uniteCourante)) {
    S1 = neg();
    S2 = tB(S1);
  } else {
    erreur("Erreur de syntaxe");
  }

  affiche_balise_fermante("terme",XML);
  if(S2==NULL)
    return S1;
  return S2;
}

n_exp * tB(n_exp * herite) {
  n_exp *S1 = NULL;
  n_exp *SC = NULL;
  n_exp *SS = NULL;
  affiche_balise_ouvrante("termeBis",XML);

  if (uniteCourante == FOIS) {
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    uniteCourante = yylex();
    if (est_premier(_negation_, uniteCourante)) {
      S1 = neg();
      SC = cree_n_exp_op(fois, herite, S1);
      SS = tB(SC);
    } else {
      erreur("Terme attendu après '*' ou '/'");
    }
  } else if (uniteCourante == DIVISE) {
      nom_token(uniteCourante, nom, valeur);
      affiche_element(nom, valeur, XML);
      uniteCourante = yylex();
      if (est_premier(_negation_, uniteCourante)) {
        S1 = neg();
        SC = cree_n_exp_op(divise, herite, S1);
        SS = tB(SC);
      } else {
        erreur("Terme attendu après '*' ou '/'");
      }
    } else if(est_suivant(uniteCourante, _termeBis_)){
    		SS = herite;
    	}

  affiche_balise_fermante("termeBis",XML);
  if(SS == NULL)
    return SC;
  return SS;
}

n_exp * neg(void) { // a voir avec hamza
  n_exp *S1 = NULL;
  n_exp *S2 = NULL;
  affiche_balise_ouvrante("negation",XML);

  if (uniteCourante == NON) {
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    uniteCourante = yylex();
    if (est_premier(_negation_, uniteCourante)) {
      S1 = neg();
      S2 = cree_n_exp_op(non, S1, NULL);
    } else {
      erreur("Negation attendue après '!'");
    }
  } else if (est_premier(_facteur_, uniteCourante)) {
    S2 = f();
  } else {
    erreur("Erreur de syntaxe");
  }

  affiche_balise_fermante("negation",XML);
  if(S2 == NULL)
    return S1;
  return S2;
}


// Functions faite par Hamza
n_exp *f(void) {
  n_exp *SS = NULL;
  n_exp *S1 = NULL;
  n_appel *S3 = NULL;
  n_var *S4 = NULL;
  affiche_balise_ouvrante("facteur",XML);

  if (uniteCourante == PARENTHESE_OUVRANTE) {
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    uniteCourante = yylex();
    if (est_premier(_expression_, uniteCourante)) {
      S1 = Exp();
      if (uniteCourante == PARENTHESE_FERMANTE) {
        nom_token(uniteCourante, nom, valeur);
        affiche_element(nom, valeur, XML);
        uniteCourante = yylex();
        SS = S1;
      } else {
        erreur("')' attendue après expression");
      }
    } else {
      erreur("Expression attendue après '('");
    }
  } else if (uniteCourante == NOMBRE) {
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    uniteCourante = yylex();
    SS = cree_n_exp_entier(atoi(valeur)); // debug1

  } else if (est_premier(_appelFct_, uniteCourante)) {
    S3 = appf();
    SS = cree_n_exp_appel(S3);
  } else if (est_premier(_var_, uniteCourante)) {
    S4 = var();
    SS = cree_n_exp_var(S4);
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
        uniteCourante = yylex();
        SS = cree_n_exp_lire();
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
  return SS;
}

n_var *var(void) {
  n_var *SS = NULL;
  n_exp *S1 = NULL;
  char *nom_var;

  affiche_balise_ouvrante("var",XML);
  if (uniteCourante == ID_VAR) {
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    nom_var = strdup(valeur);
    uniteCourante = yylex();
    S1 = oind();
    if (est_premier(_optIndice_, uniteCourante)) {
      SS = cree_n_var_indicee(nom_var, S1);
    } else if (est_suivant(_var_, uniteCourante)) {
      SS = cree_n_var_simple(nom_var);
    }
  }
  affiche_balise_fermante("var",XML);
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
  char *nom_fct = NULL;
  affiche_balise_ouvrante("appelFct",XML);
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
  affiche_balise_fermante("appelFct",XML);
  return SS;
}

//refaite
n_l_exp * lexp(void) {
  n_exp *S1 = NULL;
  n_l_exp *S2 = NULL;
  affiche_balise_ouvrante("listeExpressions",XML);

  if (est_premier(_expression_, uniteCourante)) {
    S1 = Exp();
    S2 = lexpB(S1);
  } else if(!est_suivant(uniteCourante, _listeExpressions_)) {
    erreur("Erreur de syntaxe");
  }

  affiche_balise_fermante("listeExpressions",XML);
  return S2;
}

n_l_exp * lexpB(n_exp *herite) {
  n_l_exp *SS = NULL;
  n_exp *S1 = NULL;
  n_l_exp *S2 = NULL;
  affiche_balise_ouvrante("listeExpressionsBis",XML);

  if (uniteCourante == VIRGULE) {
    nom_token(uniteCourante, nom, valeur);
    affiche_element(nom, valeur, XML);
    uniteCourante = yylex();
    S1 = Exp();
    S2 = lexpB(S1);
    SS = cree_n_l_exp(S1, S2);
  } else if(est_suivant(_listeExpressionsBis_, uniteCourante)) {
    SS = cree_n_l_exp(herite,NULL);
  } else {
      erreur("Erreur de syntaxe");
  }

  affiche_balise_fermante("listeExpressionsBis",XML);
  return SS;
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
    } else if (strcmp(argv[1],"-a") == 0) {
      ABS = 1;
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

  if (!op1) //Pour ne pas interférer avec la sortie XML
    printf("SYN: Analyse syntaxique terminée avec succès\n");
  else{
    if(ABS)
      affiche_n_prog(pg());
    if(XML)
      pg();
  }

  return 0;
}
