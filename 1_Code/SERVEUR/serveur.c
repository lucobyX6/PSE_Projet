#include "serveur.h"

int main(int argc, char* argv[]) 
{
  short port;
  int ecoute, canal, output;
  struct sockaddr_in adrEcoute, adrClient;
  unsigned int lgAdrClient;
  int numWorkerLibre;
  
  fdJournal = open("journal.log", O_WRONLY|O_CREAT|O_APPEND, 0644);
  if (fdJournal == -1)
    erreur_IO("ouverture journal");

  creerCohorteWorkers();

  if (argc != 2)
    erreur("utilisation: %s port\n", argv[0]);

  port = (short)atoi(argv[1]);

  printf("%s: création d'une socket\n", CMD);
  ecoute = socket (AF_INET, SOCK_STREAM, 0);
  if (ecoute < 0)
    erreur_IO("socket");
  
  adrEcoute.sin_family = AF_INET;
  adrEcoute.sin_addr.s_addr = INADDR_ANY;
  adrEcoute.sin_port = htons(port);
  printf("%s: connection à l'adresse INADDR_ANY sur le port %d\n", CMD, port);
  output = bind (ecoute,  (struct sockaddr *)&adrEcoute, sizeof(adrEcoute));
  if (output < 0)
    erreur_IO("bind");
  
  printf("%s: écoute d'une socket\n", CMD);
  output = listen (ecoute, 5);
  if (output < 0)
    erreur_IO("listen");
  
  while (1) {
    printf("%s: acceptation de la connection\n", CMD);
    canal = accept(ecoute, (struct sockaddr *)&adrClient, &lgAdrClient);
    if (canal < 0)
      erreur_IO("accept");

    printf("%s: adr %s, port %hu\n", CMD,
	      stringIP(ntohl(adrClient.sin_addr.s_addr)), ntohs(adrClient.sin_port));

    while ( (numWorkerLibre = chercherWorkerLibre()) < 0 )
        usleep(100000);

    dataSpec[numWorkerLibre].canal = canal;
  }

  if (close(ecoute) == -1)
    erreur_IO("fermeture ecoute");

  if (close(fdJournal) == -1)
    erreur_IO("fermeture journal");

  return EXIT_SUCCESS;
}

// Création des N workers et des N threads liés
void creerCohorteWorkers(void) {
  int i, ret;

  for (i= 0; i < NB_WORKERS; i++) {
    dataSpec[i].canal = -1;
    dataSpec[i].tid = i;

    ret = pthread_create(&dataSpec[i].id, NULL, threadWorker, &dataSpec[i]);
    if (ret != 0)
      erreur_IO("creation thread");
  }
}

// Retourne le numéro du worker libre trouvé ou -1 si aucun worker de libre
int chercherWorkerLibre(void) {
  int i = 0;

  while (dataSpec[i].canal != -1 && i < NB_WORKERS)
    i++;

  if (i < NB_WORKERS)
    return i;
  else
    return -1;
}

// Création d'un thread pour un worker
void *threadWorker(void *arg) {
  DataSpec *dataTh = (DataSpec *)arg;

  while (1) {
    while (dataTh->canal < 0)
      usleep(100000);
    printf("%s: worker %d reveil\n", CMD, dataTh->tid);

    sessionClient(dataTh->canal);

    dataTh->canal = -1;
    printf("%s: worker %d sommeil\n", CMD, dataTh->tid);
  }

  pthread_exit(NULL);
}

// Session d'un client, utilise un worker 
void sessionClient(int canal) {
  int fin = 0;
  char ligne[L_MAX];
  int lgLue;

  while (!fin) {
    lgLue = lireLigne(canal, ligne);
    if (lgLue < 0)
      erreur_IO("lireLigne");
    else if (lgLue == 0)
      erreur("interruption client\n");

    printf("%s: reception %d octets : \"%s\"\n", CMD, lgLue, ligne);

    if (strcmp(ligne, "fin") == 0) {
        printf("%s: fin client\n", CMD);
        fin = 1;
    }
    else if (strcmp(ligne, "init") == 0) {
      printf("%s: remise a zero journal\n", CMD);
      remiseAZeroJournal();
    }
    else if (ecrireLigne(fdJournal, ligne) != -1) {
        printf("%s: ligne de %d octets ecrite dans journal\n", CMD, lgLue);
    }
    else
      erreur_IO("ecriture journal");
  } // fin while

  if (close(canal) == -1)
    erreur_IO("fermeture canal");
}

// Fermeture et réinitialisation du journal
void remiseAZeroJournal(void) {
  if (close(fdJournal) == -1)
    erreur_IO("raz journal - fermeture");

  fdJournal = open("journal.log", O_WRONLY|O_TRUNC|O_APPEND, 0644);
  if (fdJournal == -1)
    erreur_IO("raz journal - ouverture");
}