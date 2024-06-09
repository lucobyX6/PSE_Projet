/* - - - HEADER - - -*/
#include "../LIB/all_lib.h"

/* - - - IFNDEF - - - */ 
#ifndef JEU_SERVEUR 
#define JEU_SERVEUR


/* - - - DEFINE - - - */
#define NB_Grille 1
#define NB_Questions 24


/* - - - PROTOTYPES - - - */
void choix_grille();
int char_here(char C, char* Str);
void choix_devinettes(char* question ,char* solution, char* q_interdites);


#endif