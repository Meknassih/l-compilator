#include"symboles.h"
#include"premiers.h"

void initialise_premiers(void){
  int i,j;

  // Initialiser toutes les cases du tableau Ã  0
  for(i=0; i <= NB_NON_TERMINAUX; i++)
    for(j=0; j <= NB_TERMINAUX; j++)
      premiers[i][j] = 0;

  // Exemple
  premiers[_programme_][ENTIER] = 1;

  // TODO: ComplÃ©ter le tableau premiers ici
}

int est_premier(int non_terminal, int terminal)
{
  return premiers[non_terminal][terminal];
}
