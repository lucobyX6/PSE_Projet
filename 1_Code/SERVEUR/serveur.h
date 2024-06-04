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
DataSpec dataSpec;

// Port de connection des joueurs
int ecoute;

// Communication
struct sockaddr_in adrEcoute;
struct sockaddr_in adrClient;

// Canaux de communication
int canal;

int output;
unsigned int lgAdrClient;

/* -> Joueurs <- */
int joueurs_actifs[NB_PLAYERS_MAX]; // Si une case est à 0, le worker est libre sinon il est occupé

// Nom des joueurs
char default_player[L_MAX] = "Pas de joueur";
char nom_J[4][L_MAX];

/* - - - Prototypes - - - */
void creerCohorteWorkers(int nb_p);
int chercherWorkerLibre(void);
void *threadWorker(void *arg);
void sessionClient(int canal);

void* Recherche_joueurs(void* arg);
void Attentes_joueurs(char* nom_partie, int* nb_players, int* nb_players_actifs);


#endif