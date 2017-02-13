#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "headers/symboles.h"
#include "headers/analyseur_lexical.h"
#include "headers/util.h"

#define YYTEXT_MAX 100
#define is_num(c)(('0' <= (c)) && ((c) <= '9'))
#define is_maj(c)(('A' <= (c)) && ((c) <= 'Z'))
#define is_min(c)(('a' <= (c)) && ((c) <= 'z'))
#define is_alpha(c)(is_maj(c) || is_min(c) || (c) == '_' || (c) == '$')
#define is_alphanum(c)(is_num((c)) || is_alpha((c)))

extern FILE *yyin;

char *tableMotsClefs[] = {
  "SI", "SINON", "ALORS", "TANTQUE", "FAIRE", "ENTIER", "RETOUR", "LIRE", "ECRIRE"
};

int codeMotClefs[] = {
  SI, SINON, ALORS, TANTQUE, FAIRE, ENTIER, RETOUR, LIRE, ECRIRE
};

char yytext[YYTEXT_MAX];
int yyleng;
int nbMotsClefs = 9;
/* Compter les lignes pour afficher les messages d'erreur avec numero ligne */
int nb_ligne = 1;

/*******************************************************************************
 * Fonction qui ignore les espaces et commentaires.
 * Renvoie -1 si arrivé à la fin du fichier, 0 si tout va bien
 ******************************************************************************/
int mangeEspaces()
{
  char c = fgetc(yyin);
  int comment = 0;
  while( comment || (c == ' ') || (c == '\n') || (c == '\t') || (c == '#' ) ) {
    if( c == '#' ) {
        comment = 1;
    }
    if( c == '\n' ) {
      nb_ligne++;
      comment = 0;
    }
    c = fgetc(yyin);
  }
  if ( feof(yyin) ) {
    return -1;
  }
  ungetc(c, yyin);
  return 0;
}

/*******************************************************************************
 * Lit un caractère et le stocke dans le buffer yytext
 ******************************************************************************/
char lireCar(void)
{
  yytext[yyleng++] = fgetc(yyin);
  yytext[yyleng] = '\0';
  return yytext[yyleng - 1];
}

/*******************************************************************************
 * Remet le dernier caractère lu au buffer clavier et enlève du buffer yytext
 ******************************************************************************/
void delireCar()
{
  char c;
  c = yytext[yyleng - 1];
  yytext[--yyleng] = '\0';
  ungetc(c, yyin);
}

/*******************************************************************************
 * Fonction auxiliaire à yylex permettant de détecter les symboles à plusieurs
 * caractères (mot-clefs, instructions de controle,...)
 ******************************************************************************/
int lireMotclef(void) { //FINIR else DELIRECAR() COMME entier / FINIR GESTION ERR APR tantque
  char c;
  int i;

  c = lireCar();
  //------------SI + SINON
  if (c=='s') {
    c = lireCar();
    if (c=='i') {
      c = lireCar();
      if (c==' ' || c=='(') {
        delireCar();
        return SI;
      } else if (c=='n') {
        c = lireCar();
        if (c=='o') {
          c=lireCar();
          if (c=='n') {
            c=lireCar();
            if (c==' ' || c=='(' || c=='\n') {
              delireCar();
              return SINON;
            } else if (!is_alphanum(c)) {
	      erreur("LEX: ' ', '(' ou retour attendu après 'sinon'");
	    }
          }
        }
      } else if (!is_alphanum(c)) {
	erreur("LEX: ' ' ou '(' attendu après 'si'");
      }
    }
  }

  //------------ALORS
  if (c=='a') {
    c = lireCar();
    if (c=='l') {
      c = lireCar();
      if (c=='o') {
        c = lireCar();
        if (c=='r') {
          c = lireCar();
          if (c=='s') {
            c = lireCar();
            if (c==' ' || c=='{' || c=='\n') {
              delireCar();
              return ALORS;
            } else if (!is_alphanum(c)) {
	      erreur("LEX: ' ', '{' ou retour attendu après 'alors'");
	    }
          }
        }
      }
    }
  }

  //------------TANTQUE
  if (c=='t') {
    c = lireCar();
    if (c=='a') {
      c = lireCar();
      if (c=='n') {
        c = lireCar();
        if (c=='t') {
          c = lireCar();
          if (c=='q') {
            c = lireCar();
            if (c=='u') {
              c = lireCar();
              if (c=='e') {
                c = lireCar();
                if (c==' ' || c=='(') {
                  delireCar();
                  return TANTQUE;
                } else if (!is_alphanum(c)) {
		  erreur("LEX: ' ' ou '(' attendu après 'tantque'");
		}
              }
            }
          }
        }
      }
    }
  }

  //------------FAIRE
  if (c=='f') {
    c = lireCar();
    if (c=='a') {
      c = lireCar();
      if (c=='i') {
        c = lireCar();
        if (c=='r') {
          c = lireCar();
          if (c=='e') {
            c = lireCar();
            if (c==' ' || c=='{' || c=='\n') {
              delireCar();
              return FAIRE;
            } else if (!is_alphanum(c)) {
	      erreur("LEX: ' ', '{' ou retour attendu après 'faire'");
	    }
          }
        }
      }
    }
  }

  //------------ENTIER
  if (c=='e') {
    c = lireCar();
    if (c=='n') {
      c = lireCar();
      if (c=='t') {
        c = lireCar();
        if (c=='i') {
          c = lireCar();
          if (c=='e') {
            c = lireCar();
            if (c=='r') {
              c = lireCar();
              if (c==' ') {
                delireCar();
                return ENTIER;
              } else if (!is_alphanum(c)) {
		erreur("LEX: ' ' attendu après 'entier'");
	      } else {
		for (i=0;i<7;i++)
		  delireCar();
		c = lireCar();
	      }
            } else {
	      for (i=0;i<6;i++)
		delireCar();
	      c = lireCar();
	    }
          } else {
	    for (i=0;i<5;i++)
	      delireCar();
	    c = lireCar();
	  }
        } else {
	  for (i=0;i<4;i++)
	    delireCar();
	  c = lireCar();
	}
      } else {
	for (i=0;i<3;i++)
	  delireCar();
	c = lireCar();
      }
    } else {
      for (i=0;i<2;i++)
	delireCar();
      c = lireCar();
    }
  }

  //------------ECRIRE
  if (c=='e') {
    c = lireCar();
    if (c=='c') {
      c = lireCar();
      if (c=='r') {
        c = lireCar();
        if (c=='i') {
          c = lireCar();
          if (c=='r') {
            c = lireCar();
            if (c=='e') {
              c = lireCar();
              if (c==' ' || c=='(') {
                delireCar();
                return ECRIRE;
              } else if (!is_alphanum(c)) {
		erreur("LEX: ' ' ou '(' attendu après 'ecrire'");
	      }
            }
          }
        }
      }
    }
  }

  //------------LIRE
  if (c=='l') {
    c = lireCar();
    if (c=='i') {
      c = lireCar();
      if (c=='r') {
        c = lireCar();
        if (c=='e') {
          c = lireCar();
          if (c==' ' || c=='(') {
            delireCar();
            return LIRE;
          } else if (!is_alphanum(c)) {
	    erreur("LEX: ' ' ou '(' attendu après 'lire'");
	  }
        }
      }
    }
  }

  //------------NOMBRE
  if (is_num(c)) {
    while (is_num(c)) {
      i++;
      c = lireCar();
    }
    if (is_alphanum(c)) {
      erreur("LEX: symbole non reconnu (une fonction ne peut pas commencer par un numéro)");
    }
    delireCar();
    return NOMBRE;
  }

  //------------ID_VAR
  if (c=='$') {
    while (is_alphanum(c)) {
        i++;
        c = lireCar();
    }
    delireCar();
    return ID_VAR;
  }

  //------------ID_FCT
  if (isalpha(c) || c=='_') {
    while (is_alphanum(c)) {
      i++;
      c = lireCar();
    }
    delireCar();
    return ID_FCT;
  }

  //------------FIN
  if (feof(yyin)) {
    return FIN;
  }

  //------------aucun
  erreur("Symbole non reconnu");
  return -1;
}

/*******************************************************************************
 * Fonction principale de l'analyseur lexical, lit les caractères de yyin et
 * renvoie les tokens sous forme d'entier. Le code de chaque unité est défini
 * dans symboles.h sinon (mot clé, idententifiant, etc.). Pour les tokens de
 * type ID_FCT, ID_VAR et NOMBRE la valeur du token est dans yytext, visible
 * dans l'analyseur syntaxique.
 ******************************************************************************/
int yylex(void)
{
  char c;
  yytext[yyleng = 0] = '\0';
  mangeEspaces();
  c = lireCar();
  switch (c) {
  case ';':
    return POINT_VIRGULE;
    break;
  case '+':
    return PLUS;
    break;
  case '-':
    return MOINS;
    break;
  case '*':
    return FOIS;
    break;
  case '/':
    return DIVISE;
    break;
  case '(':
    return PARENTHESE_OUVRANTE;
    break;
  case ')':
    return PARENTHESE_FERMANTE;
    break;
  case '[':
    return CROCHET_OUVRANT;
    break;
  case ']':
    return CROCHET_FERMANT;
    break;
  case '{':
    return ACCOLADE_OUVRANTE;
    break;
  case '}':
    return ACCOLADE_FERMANTE;
    break;
  case '=':
    return EGAL;
    break;
  case '<':
    return INFERIEUR;
    break;
  case '&':
    return ET;
    break;
  case '|':
    return OU;
    break;
  case '!':
    return NON;
    break;
  case ',':
    return VIRGULE;
    break;
  case '\n':
    return RETOUR;
    break;
  default:
    delireCar();
    return lireMotclef();
  }
}

/*******************************************************************************
 * Fonction auxiliaire appelée par l'analyseur syntaxique tout simplement pour
 * afficher des messages d'erreur et l'arbre XML
 ******************************************************************************/
void nom_token( int token, char *nom, char *valeur ) {
  int i;

  strcpy( nom, "symbole" );
  if(token == POINT_VIRGULE) strcpy( valeur, "POINT_VIRGULE");
  else if(token == PLUS) strcpy(valeur, "PLUS");
  else if(token == MOINS) strcpy(valeur, "MOINS");
  else if(token == FOIS) strcpy(valeur, "FOIS");
  else if(token == DIVISE) strcpy(valeur, "DIVISE");
  else if(token == PARENTHESE_OUVRANTE) strcpy(valeur, "PARENTHESE_OUVRANTE");
  else if(token == PARENTHESE_FERMANTE) strcpy(valeur, "PARENTHESE_FERMANTE");
  else if(token == CROCHET_OUVRANT) strcpy(valeur, "CROCHET_OUVRANT");
  else if(token == CROCHET_FERMANT) strcpy(valeur, "CROCHET_FERMANT");
  else if(token == ACCOLADE_OUVRANTE) strcpy(valeur, "ACCOLADE_OUVRANTE");
  else if(token == ACCOLADE_FERMANTE) strcpy(valeur, "ACCOLADE_FERMANTE");
  else if(token == EGAL) strcpy(valeur, "EGAL");
  else if(token == INFERIEUR) strcpy(valeur, "INFERIEUR");
  else if(token == ET) strcpy(valeur, "ET");
  else if(token == OU) strcpy(valeur, "OU");
  else if(token == NON) strcpy(valeur, "NON");
  else if(token == FIN) strcpy(valeur, "FIN");
  else if(token == VIRGULE) strcpy(valeur, "VIRGULE");

  else if( token == ID_VAR ) {
    strcpy( nom, "id_variable" );
    strcpy( valeur, yytext );
  }
  else if( token == ID_FCT ) {
    strcpy( nom, "id_fonction" );
    strcpy( valeur, yytext );
  }
  else if( token == NOMBRE ) {
    strcpy( nom, "nombre" );
    strcpy( valeur, yytext );
  }
  else {
    strcpy( nom, "mot_clef" );
    for(i=0; i < nbMotsClefs; i++){
      if( token ==  codeMotClefs[i] ){
        strcpy( valeur, tableMotsClefs[i] );
        break;
      }
    }
  }
}
/*******************************************************************************
 * Fonction auxil appelée par le compilateur en mode -l, pour tester l'analyseur
 * lexical et, étant donné un programme en entrée, afficher la liste des tokens.
 ******************************************************************************/

void test_yylex_internal(FILE *yyin) {
  int uniteCourante;
  char nom[100];
  char valeur[100];
  do {
    uniteCourante = yylex();
    nom_token( uniteCourante, nom, valeur );
    printf("%s\t%s\t%s\n", yytext, nom, valeur);
  } while (uniteCourante != FIN);
}
