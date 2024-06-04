#include "serveur.h"

int main(int argc, char* argv[]) 
{
  short port;

  // Nombre d'argument correct ?
  if (argc != 2)
    erreur("Utilisation: %s port machine\n", argv[0]);

  // Nombre de joueurs
  int nb_players = 0;
  int nb_players_actifs =0;

  while(nb_players < 2 || nb_players > 4)
  {
    printf("Choisissez le nombre de joueurs : ");
    scanf(" %d", &nb_players);
    if(nb_players < 2 || nb_players > 4)
      printf("Le nombre de joueurs doit être contenu entre 2 et 4\n");
  }

  char nom_partie[L_MAX];
  printf("Choisissez le nom de la partie : ");
  scanf(" %s", &nom_partie);

  // Ports de communication des clients
  printf("%s: assignation des ports de communication\n", CMD);
  port = (short)atoi(argv[1]);
  printf("Port de communication %d : ", port);

  // Création de la socket de communication
  printf("%s: création des sockets\n", CMD);
  ecoute = socket(AF_INET, SOCK_STREAM, 0);
  if (ecoute < 0)
    erreur_IO("socket");
  
  // Libére les adresses cibles 
  int e =1;
  setsockopt(ecoute, SOL_SOCKET, SO_REUSEADDR, &e, sizeof(e));

  // Adresse de communciation

  adrEcoute.sin_family = AF_INET;
  adrEcoute.sin_addr.s_addr = INADDR_ANY;
  adrEcoute.sin_port = htons(port);
  
  printf("%s: connection à l'adresse INADDR_ANY sur le port %d\n", CMD, port);
  output = bind (ecoute,  (struct sockaddr *)&adrEcoute, sizeof(adrEcoute));
  if (output < 0)
    erreur_IO("bind");
  
  // Écoute de plusieurs sockets
  printf("%s: écoute de la socket\n", CMD);
  output = listen(ecoute, 5);
  if (output < 0)
    erreur_IO("listen");

  // Initialisation des noms des joueurs
  for(int j=0; j<nb_players ;j++)
  {
    strcpy(nom_J[j], default_player);
  }

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

  // Salon d'attente des joueurs
  int p = fork();
  if (p == 0) 
  {
    Attentes_joueurs(nom_partie, &nb_players, &nb_players_actifs);
  }
  else 
  {
    wait(NULL);
  }

  // Création du worker pour chaque client
  creerCohorteWorkers(nb_players);

  // Lancement du jeu
  while (1) 
  {
    printf("FIN");
  }

  // Fermeture du serveur
  if (close(ecoute) == -1)
    erreur_IO("fermeture ecoute");

  return EXIT_SUCCESS;
}

// Création des N workers et des N threads liés
void creerCohorteWorkers(int nb_p) 
{
  int ret;
  for(int i=0; i<nb_p; i++)
{
  dataSpec.canal = -1;
  dataSpec.tid = i;
  ret = pthread_create(&dataSpec.id, NULL, threadWorker, &dataSpec);
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
  
  // Connection des joueurs
  canal = accept(ecoute, (struct sockaddr *)&adrClient, &lgAdrClient);

  if (canal < 0)
    erreur_IO("accept");

  dataSpec.canal = canal;

  end = 0;
  int lgLue;
  char ligne[L_MAX];
  char ligne_connection[L_MAX];
  sprintf(ligne_connection, "%d_connection", *(i+1));

  while(!end)
  {
    lgLue = lireLigne(canal, ligne);
    if (lgLue < 0)
      erreur_IO("lireLigne");
  
    if(strcmp(ligne, ligne_connection))  
    {
      pthread_exit(NULL);
    }
  }
}

void Attentes_joueurs(char* nom_partie, int* nb_players, int* nb_players_actifs)
{
  
  FILE *fp = fopen("./SERVEUR/salle_attente.txt", "w");

  char *args[] = {"watch", "-n", "5", "cat", "./SERVEUR/salle_attente.txt", NULL};
  int p = fork();
  if(p ==0)
    execvp("watch", args);
  else
  {

    int end = 0;

    while(!end)
    {
      fprintf(fp, "< - - -[Salon %s]- - - >\n", nom_partie);
      for(int i =0; i < *nb_players; i++)
      { 
        fprintf(fp, "Joueur %d : %s avec le file descriptor %d\n", i+1, nom_J[i], ecoute);
      }

      fflush(fp);

      if(*nb_players_actifs == *nb_players)
      {
        end = 1;
      }
      fseek(fp, 0, SEEK_SET);
    }
  
  }

  fclose(fp);
  pthread_exit(NULL);
}