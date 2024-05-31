/* - - - IFNDEF - - - */ 
#ifndef SERVEUR 
#define SERVEUR

/* - - - HEADER - - -*/
#include "../LIB/all_lib.h"

#include "jeu_serveur.h"

/* - - - Define - - - */
#define   CMD         "serveur"
#define   NB_WORKERS  2

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
DataSpec dataSpec[NB_WORKERS];

/* - - - Prototypes - - - */
void creerCohorteWorkers(void);
int chercherWorkerLibre(void);
void *threadWorker(void *arg);
void sessionClient(int canal);
void remiseAZeroJournal(void);


#endif