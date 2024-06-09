/* - - - IFNDEF - - - */ 
#ifndef UTILITIES 
#define UTILITIES


/* - - - HEADER - - -*/
#include "all_lib.h"

/* - - - Prototypes - - - */
void erreur_IO (char *msg);
void erreur ( const char *format, ... );
struct sockaddr_in *resolv (const char *host, const char *service);
char *stringIP (unsigned int entierIP);
void freeResolv (void);

int lireLigne(int fd, char *buffer);
int ecrireLigne(int fd, char *buffer);

int randint(int max);

/* - - - Define - - - */
#define L_MAX 200

#define Connection 'C'

#define Reponse 'R'

#define Grille 'G'

#define Slam 'S'

#define Information 'I'

/* - - - Structures - - - */
static struct addrinfo *static_infos = NULL;

typedef struct _Joueurs
{
  int canal;
  char nom[L_MAX];
  int num_joueur;
}Joueurs;

#endif