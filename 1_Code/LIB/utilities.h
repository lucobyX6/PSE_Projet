/* - - - IFNDEF - - - */ 
#ifndef JEU_CLIENT 
#define JEU_CLIENT


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

/* - - - Define - - - */
#define L_MAX 200

/* - - - Structures - - - */
static struct addrinfo *static_infos = NULL;

#endif