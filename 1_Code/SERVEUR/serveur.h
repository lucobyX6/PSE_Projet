/* - - - IFNDEF - - - */ 
#ifndef SERVEUR 
#define SERVEUR

/* - - - HEADER - - -*/
#include "../LIB/all_lib.h"

#include "jeu_serveur.h"

/* - - - Define - - - */
#define   CMD         "serveur"
#define   NB_WORKERS  1
#define   NB_PLAYERS_MAX  4

/* - - - Structures - - - */
typedef struct DataSpec_t {
  pthread_t id;               /* identifiant du thread */
  int libre;                  /* indicateur de terminaison */
/* ajouter donnees specifiques après cette ligne */
  int tid;                    /* identifiant logique */
  int canal;                  /* canal de communication */
  sem_t sem;                  /* semaphore de reveil */
} DataSpec;
typedef struct DataThread_t {
  DataSpec spec;              /* donnees specifiques : voir dataspec.h */
  struct DataThread_t *next;  /* si thread suivant, alors son adresse, 
                                 sinon NULL */
} DataThread;

/* - - - Variables globales - - - */
int fdJournal;
DataSpec dataSpec[NB_PLAYERS_MAX][NB_WORKERS];

// Port de connection des joueurs
short tab_port_players[NB_PLAYERS_MAX];
int tab_ecoute_players[NB_PLAYERS_MAX];

// Communication
struct sockaddr_in adrEcoute[NB_PLAYERS_MAX];
struct sockaddr_in adrClient[NB_PLAYERS_MAX];

// Canaux de communication
int canal[NB_PLAYERS_MAX];

int output;
unsigned int lgAdrClient[NB_PLAYERS_MAX];
int workers[NB_PLAYERS_MAX]; // Si une case est à 0, le worker est libre sinon il est occupé

/* - - - Prototypes - - - */
void creerCohorteWorkers(int nb_p);
int chercherWorkerLibre(void);
void *threadWorker(void *arg);
void sessionClient(int canal);
void* Recherche_joueurs(void* arg);


#endif