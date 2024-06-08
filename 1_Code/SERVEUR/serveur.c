#include "serveur.h"

int main(int argc, char* argv[]) 
{
  /* - - - Variables - - - */
  
  // Sortie d'erreur
  int output;

  // Nom par défauts des joueurs
  char default_joueur[L_MAX] = "Pas de joueur";

  // Nombre de joueurs
  int nb_joueurs = 0;

  // Nom de la partie
  char nom_partie[L_MAX];

  // Nombre d'argument correct ?
  if (argc != 2)
    erreur("Utilisation: %s port machine\n", argv[0]);

  // Choix du nombre de joueurs 
  while(nb_joueurs < 2 || nb_joueurs > 4)
  {
    printf("Choisissez le nombre de joueurs : ");
    scanf(" %d", &nb_joueurs);
    if(nb_joueurs < 2 || nb_joueurs > 4)
      printf("Le nombre de joueurs doit être contenu entre 2 et 4\n");
  }

  // Id des threads joueurs
  pthread_t* idThreads = (pthread_t*)malloc(nb_joueurs*sizeof(pthread_t));

  // Choix du nom de la partie
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
  
  // Libére l'adresse cible
  printf("%s: Libération de l'adresse cible\n", CMD);
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
    erreur_IO("listen");;

  // Salon d'attente des joueurs & de recherches
  printf("%s: salle d'attente \n", CMD);
  Attentes_joueurs(nom_partie,nb_joueurs,default_joueur);

  // Création de la cohorte de session_joueurs
  pthread_create(&idThreads[a], NULL, session_joueurs, NULL);

  pthread_create(&idThreads[b], NULL, session_joueurs, NULL);

  if(nb_joueurs > 2)
    pthread_create(&idThreads[c], NULL, session_joueurs, NULL);
  
  if(nb_joueurs > 3)
    pthread_create(&idThreads[d], NULL, session_joueurs, NULL);

  while(1)usleep(1);

  // Fermeture du serveur
  if (close(ecoute) == -1)
    erreur_IO("fermeture ecoute");

  return EXIT_SUCCESS;
}

void Attentes_joueurs(char* nom_partie, int nb_joueurs, char* default_joueur)
{
  
  // Fichier de stockage des informations joueurs
  FILE * info_joueurs;
  info_joueurs = fopen("info_joueurs","w+");

  fwrite(&nb_joueurs, sizeof(int), 1, info_joueurs);
  
  // Fichier qui contient la salle d'attente des joueurs
  FILE *fp = fopen("./SERVEUR/salle_attente.txt", "w");

  char *args[] = {"watch", "-n", "1", "cat", "./SERVEUR/salle_attente.txt", NULL};
  
  // Initialisation des noms des joueurs
  printf("%s: initialisation des noms des joueurs\n", CMD);
  char nom_J[4][L_MAX];
  int canal_J[4];
  for(int i =0; i < nb_joueurs; i++)
  {
    strcpy(nom_J[i], default_joueur);
  }
  for(int i =0; i < nb_joueurs; i++)
  {
    canal_J[i] = -1;
  }
  
  
  int nb_joueurs_actifs =0;
  int end =0;
  int pid=0;
  // Lecture
  int lgLue;
  char ligne[L_MAX];
  char ligne_old[L_MAX];
  int canal_temp =0;

  printf("%s: fork\n", CMD);
  int p = fork();
  if(p ==0)
  {
    // Affichage des joueurs
    execvp("watch", args);
    exit(0);
  }
  else
  {
    while(!end)
    {
      strcpy(ligne_old, ligne);
      
      fp=freopen(NULL,"w",fp);
      // Mise à jour du salon
      fprintf(fp, "< - - -[Salon %s]- - - >\n", nom_partie);
      for(int i =0; i < nb_joueurs; i++)
      { 
        fprintf(fp, "Joueur %d : %s sur le canal %d\n", i+1, nom_J[i], canal_J[i]);
      }
      fflush(fp);
      fseek(fp, 0, SEEK_SET);

      // Recherche des joueurs
      canal_temp = accept(ecoute, (struct sockaddr *)&adrClient, &lgAdrClient);
      if (canal_temp < 0)
        erreur_IO("accept");
      
      Joueurs* Nouveau_joueurs = (Joueurs*)malloc(sizeof(Joueurs));
      Nouveau_joueurs->canal = canal_temp;

      // Lire une ligne de la socket
      lgLue = lireLigne(Nouveau_joueurs->canal, ligne);
      if (lgLue < 0)
      erreur_IO("lireLigne");

      Nouveau_joueurs->num_joueur = nb_joueurs_actifs+1;

      // On retire les parties fonctionnelles du message pour ne garder que le pseudonyme
      decoupe_message(Nouveau_joueurs->nom,ligne);

      // Sauvegarde des informations
      fwrite(Nouveau_joueurs, sizeof(Nouveau_joueurs), 1, info_joueurs);

      // Pour l'affichage local
      strcpy(nom_J[Nouveau_joueurs->num_joueur-1], Nouveau_joueurs->nom);
      canal_J[Nouveau_joueurs->num_joueur-1] = Nouveau_joueurs->canal;

      nb_joueurs_actifs++;

      if(nb_joueurs_actifs >= nb_joueurs)
      {
        fp=freopen(NULL,"w",fp);
        // Mise à jour du salon
        fprintf(fp, "< - - -[Salon %s]- - - >\n", nom_partie);
        for(int i =0; i < nb_joueurs; i++)
        { 
          fprintf(fp, "Joueur %d : %s sur le canal %d\n", i+1, nom_J[i], canal_J[i]);
        }
        fflush(fp);
        fseek(fp, 0, SEEK_SET);
        
        end =1;
      }

    }
  
  }
  // FIN
  kill(p, SIGTERM);
  fclose(fp);
  fclose(info_joueurs);
  printf("%s: Début de la partie dans 3\n", CMD);
  usleep(1000000);
  printf("%s: Début de la partie dans 2\n", CMD);
  usleep(1000000);
  printf("%s: Début de la partie dans 1\n", CMD);
  usleep(1000000);
  printf("%s: --> Début de la partie\n", CMD);
  pthread_exit(NULL);

}

int identification_message(char* message)
{
  switch(message[2]) {
        case Connection:
            return 0;
            break;
        case Reponse:
            return 1;
            break;
        case Grille:
            return 2;
            break;
        case Slam:
            return 3;
            break;
        default:
            return -1;
    }
  
}

void decoupe_message(char* output, char* message)
{
  for(int i=2; i<strlen(message); i++)
  {
    output[i-2] = message[i];
  }
}

void recup_data_fichier(int numero, int* nb_joueurs, Joueurs* J_struct)
{
  FILE * info_joueurs;
  info_joueurs = fopen("info_joueurs","r");

  fread(nb_joueurs, sizeof(int), 1, info_joueurs);
  for(int i=0; i<numero-1; i++)  
  {
    fread(J_struct, sizeof(Joueurs), 1, info_joueurs);
  }

  fclose(info_joueurs);
}

void* session_joueurs(void* arg)
{
  // Numéro du joueur traité par le thread
  int* i = (int*)arg;

  // Nombre de joueurs total
  int* nb_joueurs;

  // Informations du joueurs
  Joueurs* J_info;
  
  // FIFO en écriture
  char FIFO_ecriture[L_MAX];
  sprintf(FIFO_ecriture, "FIFO_W_%d", *i);
  int output = open(FIFO_ecriture, O_CREAT|O_WRONLY|O_TRUNC, 0644);

  // FIFO en lecture
  char FIFO_lecture[L_MAX];
  sprintf(FIFO_lecture, "FIFO_R_%d", *i);
  output = open(FIFO_lecture, O_RDONLY);

  recup_data_fichier(*i, nb_joueurs, J_info);
}