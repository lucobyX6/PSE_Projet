/* - - - IFNDEF - - - */ 
#ifndef SERVEUR 
#define SERVEUR

/* - - - HEADER - - -*/
#include "../LIB/all_lib.h"

#include "jeu_serveur.h"

/* - - - Define - - - */
#define   CMD         "serveur"
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

/* - - Joueurs - - */
int joueurs_actifs[NB_PLAYERS_MAX]; // Si une case est à 0, le worker est libre sinon il est occupé

// Communication
struct sockaddr_in adrEcoute;
struct sockaddr_in adrClient;
unsigned int lgAdrClient; 
DataSpec dataSpec;

// Port de communication
short port;
int ecoute;

// Threads & FIFO
int a = 0;
int b = 1;
int c = 2;
int d = 3;

pthread_mutex_t restricted_access = PTHREAD_MUTEX_INITIALIZER;

/* - - - Prototypes - - - */

//Fonction de gestion de la salle d'attente (configuration, attente des joueurs, ajout des nouveaux joueurs, etc...)
void Attentes_joueurs(char* nom_partie, int nb_joueurs, char* default_joueur);

//Fonction pour gérer les échnager serveur/client dans les différentes phases du jeu.
void* session_joueurs(void* arg);

//Fonction pour découper la partie texte dans le protocole de communication
void decoupe_message(char* output, char* message);

//Fonction pour récupérer les données stockées dans info_joueurs.bin
void recup_data_fichier(int numero, int* nb_joueurs, Joueurs* J_struct);


#endif