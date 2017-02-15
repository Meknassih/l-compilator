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
 * Fonction principale de l'analyseur lexical, lit les caractères de yyin et
 * renvoie les tokens sous forme d'entier. Le code de chaque unité est défini
 * dans symboles.h sinon (mot clé, idententifiant, etc.). Pour les tokens de
 * type ID_FCT, ID_VAR et NOMBRE la valeur du token est dans yytext, visible
 * dans l'analyseur syntaxique.
 ******************************************************************************/
int yylex(void)
{
  char c;
	int i;
	yytext[yyleng = 0] = '\0';
	mangeEspaces();
	c = lireCar();

	//Reconnaissance symboles simples
	if (c==';')
		return POINT_VIRGULE;
	else if (c=='+')
		return PLUS;
	else if (c=='-')
		return MOINS;
	else if (c=='*')
		return FOIS;
	else if (c=='/')
		return DIVISE;
	else if (c=='(')
		return PARENTHESE_OUVRANTE;
	else if (c==')')
		return PARENTHESE_FERMANTE;
	else if (c=='[')
		return CROCHET_OUVRANT;
	else if (c==']')
		return CROCHET_FERMANT;
	else if (c=='{')
		return ACCOLADE_OUVRANTE;
	else if (c=='}')
		return ACCOLADE_FERMANTE;
	else if (c=='=')
		return EGAL;
	else if (c=='<')
		return INFERIEUR;
	else if (c=='&')
		return ET;
	else if (c=='|')
		return OU;
	else if (c=='!')
		return NON;
	else if (c==',')
		return VIRGULE;
	else if (c==EOF)
		return FIN;

	//Reconnaissance mots-clefs
	//SI SINON
	if (c=='s') {
		c = lireCar();
		if (c=='i') {
			c = lireCar();
			if (c==' ') {
				delireCar();
				return SI;
			} else if (c=='n'){
				c = lireCar();
				if (c=='o') {
					c = lireCar();
					if (c=='n') {
						c = lireCar();
						if (!is_alphanum(c)) {
							delireCar();
							return SINON;
						}
					}
				}
			}
		}
	}
	//ALORS
	if (c=='a') {
		c = lireCar();
		if (c=='l') {
			c = lireCar();
			if (c=='o') {
				c = lireCar();
				if (c=='r'){
					c = lireCar();
					if (c=='s') {
						c = lireCar();
						if (!is_alphanum(c)) {
							delireCar();
							return ALORS;
						}
					}
				}
			}
		}
	}
	//TANTQUE
	if (c=='t') {
		c = lireCar();
		if (c=='a') {
			c = lireCar();
			if (c=='n') {
				c = lireCar();
				if (c=='t'){
					c = lireCar();
					if (c=='q') {
						c = lireCar();
						if (c=='u') {
							c = lireCar();
							if (c=='e') {
								c = lireCar();
								if (c==' ') {
									delireCar();
									return TANTQUE;
								}
							}
						}
					}
				}
			}
		}
	}
	//FAIRE
	if (c=='f') {
		c = lireCar();
		if (c=='a') {
			c = lireCar();
			if (c=='i') {
				c = lireCar();
				if (c=='r'){
					c = lireCar();
					if (c=='e') {
						c = lireCar();
						if (!is_alphanum(c)) {
							delireCar();
							return FAIRE;
						}
					}
				}
			}
		}
	}
	//ENTIER ECRIRE
	if (c=='e') {
		c = lireCar();
		if (c=='n') {
			c = lireCar();
			if (c=='t') {
				c = lireCar();
				if (c=='i'){
					c = lireCar();
					if (c=='e') {
						c = lireCar();
						if (c=='r') {
							c = lireCar();
							if (c==' ') {
								delireCar();
								return ENTIER;
							}
						}
					}
				}
			}
		} else if (c=='c') {
			c = lireCar();
			if (c=='r') {
				 c = lireCar();
				if (c=='i'){
					c = lireCar();
					if (c=='r') {
						c = lireCar();
						if (c=='e') {
							c = lireCar();
							if (c=='(') {
								delireCar();
								return ECRIRE;
							}
						}
					}
				}
			}
		}
	}
	//RETOUR
	if (c=='r') {
		c = lireCar();
		if (c=='e') {
			c = lireCar();
			if (c=='t') {
				c = lireCar();
				if (c=='o'){
					c = lireCar();
					if (c=='u') {
						c = lireCar();
						if (c=='r') {
							c = lireCar();
							if (c==' ' || ';') {
								delireCar();
								return RETOUR;
							}
						}
					}
				}
			}
		}
	}
	//LIRE
	if (c=='l') {
		c = lireCar();
		if (c=='i') {
			c = lireCar();
			if (c=='r') {
				c = lireCar();
				if (c=='e'){
					c = lireCar();
					if (c=='(') {
            delireCar();
						return LIRE;
					}
				}
			}
		}
	}
	//ID_VAR
	if (c=='$') {
		do {
			c = lireCar();
			i++;
		} while (is_alphanum(c));

		if (yyleng>1) {
			delireCar();
			return ID_VAR;
		}
		else
			erreur("Variable identifier must be at least 1 character long");
	}
	//ID_FCT
	if (is_maj(c) || is_min(c)) {
		do {
			c = lireCar();
		} while (is_alphanum(c));

		if (yyleng<=101) { //+1 pour '\0' et +1 pour le dernier caractère qu'on va délire de suite
			delireCar();
			return ID_FCT;
		} else
			erreur("Function identifier must be less than 99 characters");
	}
	if (c=='(') { //cas où l'identifier est un seul caractère
		if (yyleng>0 && yyleng<=101) {
			delireCar();
			return ID_FCT;
		} else
			erreur("Function identifier must be less than 99 characters");
	}
	//NOMBRE
	if (is_num(c)) {
		do {
			c = lireCar();
		} while (is_num(c));

		if (is_alphanum(c))
			erreur("An identifier cannot start with a number");
		else {
			delireCar();
			return NOMBRE;
		}
	}

/*else {
	do {
		c = lireCar();
	} while (c!='(' && c!='=' && c!=EOF);

	if (c=='(')
		return ID_FCT;
	else if (c=='=')
		return ID_VAR;
	else if (c==EOF)
		erreur_1s("EOF before end of token ", &car);
}*/
	printf("value : %s\n", yytext);
	erreur_1s("Invalid token",yytext);
  return -1;
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
