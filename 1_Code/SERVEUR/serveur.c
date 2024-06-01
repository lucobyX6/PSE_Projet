#include "serveur.h"

int main(int argc, char* argv[]) 
{

  // Nombre de joueurs
  int nb_players;
  while(nb_players > 4 || nb_players < 2)
  {
    printf("Choisissez le nombre de joueurs : ");
    scanf(" %d", &nb_players);
    if(nb_players > 4 || nb_players < 2)
      printf("Le nombre de joueurs doit être contenu entre 2 et 4\n");
  }

  char nom_partie[L_MAX];
  printf("Choisissez le nom de la partie : ");
  scanf(" %s", &nom_partie);

  // Ports de communication des clients
  printf("%s: assignation des ports de communication\n", CMD);
  for(int i =0; i < nb_players; i++)
  {
    printf("Entrez le port %d : ", i);
    scanf(" %d", &tab_port_players[i]);
  }

  // Création du worker pour chaque client
  creerCohorteWorkers(nb_players);

  // Création des sockets de communication
  printf("%s: création des sockets\n", CMD);
  for(int i =0; i < nb_players; i++)
  {
    tab_ecoute_players[i] = socket(AF_INET, SOCK_STREAM, 0);
    if (tab_ecoute_players[i] < 0)
      erreur_IO("socket");
  }

  // Libére les adresses cibles 
  int e =1;
  for(int i =0; i < nb_players; i++)
  {
    setsockopt(tab_ecoute_players[i], SOL_SOCKET, SO_REUSEADDR, &e, sizeof(e));
  }

  // Adresse de communciation
  for(int i =0; i < nb_players; i++)
  {
    adrEcoute[i].sin_family = AF_INET;
    adrEcoute[i].sin_addr.s_addr = INADDR_ANY;
    adrEcoute[i].sin_port = htons(tab_port_players[i]);
    printf("%s: connection à l'adresse INADDR_ANY sur le port %d\n", CMD, tab_port_players[i]);
    output = bind (tab_ecoute_players[i],  (struct sockaddr *)&adrEcoute[i], sizeof(adrEcoute[i]));
    if (output < 0)
      erreur_IO("bind");
  }
  
  // Écoute de plusieurs sockets
  printf("%s: écoute des sockets\n", CMD);
  for(int i =0; i < nb_players; i++)
  {
    output = listen(tab_ecoute_players[i], 5);
    if (output < 0)
      erreur_IO("listen");
  }

  // Nom des joueurs
  char nom_J1[L_MAX] = "Pas de joueur";
  char nom_J2[L_MAX] = "Pas de joueur";
  char nom_J3[L_MAX] = "Pas de joueur";
  char nom_J4[L_MAX] = "Pas de joueur";

  // Recherche des joueurs
  pthread_t idThread_1;
  pthread_t idThread_2;
  pthread_t idThread_3;
  pthread_t idThread_4;

  int a = 0;
  pthread_create(&idThread_1, NULL, Recherche_joueurs, &a);

  int b = 1;
  pthread_create(&idThread_2, NULL, Recherche_joueurs, &b);

  if(nb_players > 2)
  {
    int c = 2;
    pthread_create(&idThread_3, NULL, Recherche_joueurs, &c);
  }

  if(nb_players > 3)
  {
    int d = 3;
    pthread_create(&idThread_4, NULL, Recherche_joueurs, &d);
  }

  while (1) 
  {
    // Salon de jeu
    printf("< - - -[Salon %s]- - - >\n", nom_partie);
    printf("Joueur 1 : %s sur le port %d\n", nom_J1, tab_port_players[0]);
    printf("Joueur 2 : %s sur le port %d\n", nom_J2, tab_port_players[1]);

    if(nb_players > 2)
      printf("Joueur 3 : %s sur le port %d\n", nom_J3, tab_port_players[2]);
    if(nb_players > 3)
      printf("Joueur 4 : %s sur le port %d\n", nom_J4, tab_port_players[3]);
    printf("\n");
    usleep(5000000);
  }

  // Fermeture du serveur
  for(int i =0; i<nb_players; i++)
  {
    if (close(tab_ecoute_players[i]) == -1)
      erreur_IO("fermeture ecoute");
  }

  return EXIT_SUCCESS;
}

// Création des N workers et des N threads liés
void creerCohorteWorkers(int nb_p) 
{
  int ret;
  for(int i=0; i<nb_p; i++)
{
  dataSpec[i][0].canal = -1;
  dataSpec[i][0].tid = i;
  ret = pthread_create(&dataSpec[i][0].id, NULL, threadWorker, &dataSpec[i][0]);
  if (ret != 0)
    erreur_IO("creation thread");
}

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
    else if (ecrireLigne(fdJournal, ligne) != -1) {
        printf("%s: ligne de %d octets ecrite dans journal\n", CMD, lgLue);
    }
    else
      erreur_IO("ecriture journal");
  } // fin while

  if (close(canal) == -1)
    erreur_IO("fermeture canal");
}

// Recherche de joueurs
void* Recherche_joueurs(void* arg)
{
  int end = 0;
  int* i;
  i = (int*)arg;
  while(!end)
  {
    // Connection des joueurs
    canal[*i] = accept(tab_ecoute_players[*i], (struct sockaddr *)&adrClient[*i], &lgAdrClient[*i]);
    printf("Thread %d: acceptation de la connection\n", *i);

    if (canal[*i] < 0)
      erreur_IO("accept");

    printf("%s: adr %s, port %hu\n", CMD,
	  stringIP(ntohl(adrClient[*i].sin_addr.s_addr)), ntohs(adrClient[*i].sin_port));

    // Attente d'un worker libre
    while (workers[*i] == 1)
        usleep(1);

    dataSpec[*i][0].canal = canal[*i];
    workers[*i] = 1;
  }
}