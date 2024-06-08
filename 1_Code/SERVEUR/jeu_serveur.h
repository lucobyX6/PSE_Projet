/* - - - HEADER - - -*/
#include "../LIB/all_lib.h"

/* - - - IFNDEF - - - */ 
#ifndef JEU_SERVEUR 
#define JEU_SERVEUR


/* - - - DEFINE - - - */
#define NB_Grille 1
#define NB_Questions 24


/* - - - PROTOTYPES - - - */
void choix_grille(int* taille_grille, int ** grille, int* taille_def, char** def);
void choix_devinettes(char* question ,char* solution, int* q_interdites);


#endif