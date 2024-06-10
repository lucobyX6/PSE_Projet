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

  // Acceuil 
  printf(" - - - SLAM Reloaded - - - \n");

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
  char F_name[L_MAX];
  int* fd_W_Threads = (int*)malloc(nb_joueurs*sizeof(int));

  fd_W_Threads[0] = open("FIFO_R_0", O_CREAT|O_WRONLY|O_TRUNC, 0644);
  fd_W_Threads[1] = open("FIFO_R_1", O_CREAT|O_WRONLY|O_TRUNC, 0644);

  if(nb_joueurs > 2)
  {
    fd_W_Threads[2] = open("FIFO_R_2", O_CREAT|O_WRONLY|O_TRUNC, 0644);
  }

  if(nb_joueurs > 3)
  {
    fd_W_Threads[3] = open("FIFO_R_3", O_CREAT|O_WRONLY|O_TRUNC, 0644);
  }

  pthread_create(&idThreads[a], NULL, session_joueurs, &a);

  pthread_create(&idThreads[b], NULL, session_joueurs, &b);

  if(nb_joueurs > 2)
    pthread_create(&idThreads[c], NULL, session_joueurs, &c);
  
  if(nb_joueurs > 3)
    pthread_create(&idThreads[d], NULL, session_joueurs, &d);

  /* - - - FIFO de communication avec les threads - - - */

  int fd_R = open("FIFO_W", O_RDONLY);

  // Informations joueurs
  Joueurs* J1_info;
  Joueurs* J2_info;
  Joueurs* J3_info;
  Joueurs* J4_info;

  printf("%s : Récupération des informations joueurs\n", CMD);
  recup_data_fichier(1, &nb_joueurs, J1_info);
  recup_data_fichier(2, &nb_joueurs, J2_info);

  if(nb_joueurs > 2)
  {
    recup_data_fichier(3, &nb_joueurs, J3_info);
  }

  if(nb_joueurs > 3)
  {
    recup_data_fichier(4, &nb_joueurs, J4_info);
  }

  char texte[L_MAX];
  int end =0;
  int phase =1;

  // Devinette
  char solution;
  char q_interdit[L_MAX];
  int gagnant =0;

  /* - - - FIN PROGRAMME FONCTIONNEL - - - */
  printf("- - - En construction - - - \n");

  // Fermeture du serveur
  if (close(ecoute) == -1)
    erreur_IO("fermeture ecoute");

  return EXIT_SUCCESS;

  while(1)
  {
    
    /* - - - Phase 1 - Devinette - - - */
    printf("%s : Phase 1 - Devinette\n", CMD);
    // Phase 1
    for(int i=0; i < nb_joueurs; i++)
    {
      write(fd_W_Threads[i],&phase,sizeof(int));
    }

    printf("%s : Envoi de la devinette\n", CMD);
    // Envoi devinette
    choix_devinettes(texte, &solution, q_interdit);
    for(int i=0; i < nb_joueurs; i++)
    {
      write(fd_W_Threads[i],texte,L_MAX*sizeof(char));
    }

    printf("%s : Réponse de la devinette\n", CMD);
    // Réponse des devinettes
    for(int i=0; i < nb_joueurs; i++)
    {
      read(fd_R,texte,L_MAX*sizeof(char));
      if(strcmp(texte,&solution) == 0)
      {
        gagnant = i;
      }
    }

    printf("%s : Réponse du serveur\n", CMD);
    // Réponse serveur
    sprintf(texte, "Le gagnant est le joueur %s", nom_gagnant(gagnant, J1_info, J2_info, J3_info, J4_info));
    for(int i=0; i < nb_joueurs; i++)
    {
      output = write(fd_W_Threads[i],texte,L_MAX*sizeof(char));
      if(output < 0)
      {
        erreur_IO("write");
      }
    }


  };
}

void Attentes_joueurs(char* nom_partie, int nb_joueurs, char* default_joueur)
{
  
  // Fichier de stockage des informations joueurs
  FILE * info_joueurs;
  info_joueurs = fopen("info_joueurs.bin","w+");

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
      fwrite(Nouveau_joueurs, sizeof(Joueurs), 1, info_joueurs);

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
        fseek(fp, 0, SEEK_SET);
        usleep(5000000);
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
  info_joueurs = fopen("info_joueurs.bin","r");
  Joueurs* J_temp = (Joueurs*)malloc(sizeof(Joueurs));
  int* nb_temp = (int*)malloc(sizeof(int));

  fread(nb_temp, sizeof(int), 1, info_joueurs);
  nb_joueurs = nb_temp;
  
  fseek(info_joueurs, (numero-1)*sizeof(Joueurs), SEEK_CUR);
  fread(J_temp, sizeof(Joueurs), 1, info_joueurs);

  J_struct = J_temp;
  fclose(info_joueurs);
}

void* session_joueurs(void* arg)
{
  // Numéro du joueur traité par le thread
  int* i = (int*)arg;

  // Nombre de joueurs total
  int* nb_joueurs = (int*)malloc(sizeof(int));

  // Informations du joueurs
  Joueurs* J_info = (Joueurs*)malloc(sizeof(Joueurs));
  recup_data_fichier(*i, nb_joueurs, J_info);
  
  // FIFO en écriture
  int fd_w = open("FIFO_W", O_CREAT|O_WRONLY|O_TRUNC, 0644);

  // FIFO en lecture
  char FIFO_lecture[L_MAX];
  sprintf(FIFO_lecture, "FIFO_R_%d", *i);
  int fd_r = open(FIFO_lecture, O_RDONLY);

  // Lecture des informations joueurs
  char ligne[L_MAX];
  int lgLue;

  // Lecture des directives serveurs
  char ligne__R_serveur[L_MAX];

  // Envoi des informations vers serveurs 
  char ligne_W_serveur[L_MAX];

  // Phase de jeu donné par le serveur
  int phase;

  int end =0;
  int end_reponse =0;
  int output;
  // Attente des instructions 
  while(!end)
  {
    read(fd_r,&phase,sizeof(int));
    sprintf(ligne, "%d", phase);
    output = ecrireLigne(J_info->canal, ligne);
    if (output == -1) erreur_IO("ecrire ligne");
    
    switch (phase)
    {
      case 1:
        /* - - - Instruction 1 serveur - - -*/
        read(fd_r,ligne__R_serveur,L_MAX*sizeof(char));
        output = ecrireLigne(J_info->canal, ligne__R_serveur);
        if (output == -1) erreur_IO("ecrire ligne");

        /* - - Réponse du client - - */
        lgLue = lireLigne(J_info->canal, ligne);
        if (lgLue == -1) erreur_IO("lire ligne");

        pthread_mutex_lock(&restricted_access);
        write(fd_w,ligne_W_serveur,L_MAX*sizeof(char));
        pthread_mutex_unlock(&restricted_access);

        /* - - - Instruction 2 serveur - - - */
        read(fd_r,ligne__R_serveur,L_MAX*sizeof(char));
        output = ecrireLigne(J_info->canal, ligne__R_serveur);
        if (output == -1) erreur_IO("ecrire ligne");

        break;

      case 2:
        /* - - - Apparition de la grille - - - */

        /* - - - Choix du numéro - - - */

        /* - - - Définition - - -*/

        break;
      case 3:
        /* - - - SLAM ? - - - */

        /* - - - Si oui - - - */
        break;
    }
  }

}