#include "client.h"

#define CMD "client"

int main(int argc, char* argv[]) 
{
    int output =0;
    int sock =0;
    struct sockaddr_in *adrServ;
    int end =0;

    char prompt[L_MAX];
    char Nom[L_MAX-4];

    // Nombre d'argument correct ?
    if (argc != 3)
        erreur("Utilisation: %s port machine\n", argv[0]);

    // Création de la socket
    //printf("%s: création d'une socket\n", CMD);
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        erreur_IO("socket");

    // Résolution des conflits
    //printf("%s: resolution DNS pour %s, port %s\n", CMD, argv[1], argv[2]);
    adrServ = resolv(argv[1], argv[2]);
    if (adrServ == NULL)
        erreur("adresse %s port %s inconnus\n", argv[1], argv[2]);

    // Connection sur le port serveur
    //printf("%s: adr %s, port %hu\n", CMD,stringIP(ntohl(adrServ->sin_addr.s_addr)),ntohs(adrServ->sin_port));

    // Connection à la socket
    //printf("%s: connection à la socket\n", CMD);
    output = connect(sock, (struct sockaddr *)adrServ, sizeof(struct sockaddr_in));
    if (output < 0)
        erreur_IO("connect");

    // Connection au serveur avec pseudonyme
    printf("Quel est votre nom ?\n");
    printf("Nom : ");
    scanf(" %s", Nom);

    sprintf(prompt, "C_%s", Nom);

    output = ecrireLigne(sock, prompt);
    if (output == -1) erreur_IO("ecrire ligne");


    printf("-------------------------------------------------------------------------------------------------------\n");
    printf("███████╗██╗      █████╗ ███╗   ███╗    ██████╗ ███████╗██╗      ██████╗  █████╗ ██████╗ ███████╗██████╗\n");
    printf("██╔════╝██║     ██╔══██╗████╗ ████║    ██╔══██╗██╔════╝██║     ██╔═══██╗██╔══██╗██╔══██╗██╔════╝██╔══██╗\n");
    printf("███████╗██║     ███████║██╔████╔██║    ██████╔╝█████╗  ██║     ██║   ██║███████║██║  ██║█████╗  ██║  ██║\n");
    printf("╚════██║██║     ██╔══██║██║╚██╔╝██║    ██╔══██╗██╔══╝  ██║     ██║   ██║██╔══██║██║  ██║██╔══╝  ██║  ██║\n");
    printf("███████║███████╗██║  ██║██║ ╚═╝ ██║    ██║  ██║███████╗███████╗╚██████╔╝██║  ██║██████╔╝███████╗██████╔╝\n");
    printf("╚══════╝╚══════╝╚═╝  ╚═╝╚═╝     ╚═╝    ╚═╝  ╚═╝╚══════╝╚══════╝ ╚═════╝ ╚═╝  ╚═╝╚═════╝ ╚══════╝╚═════╝\n");
    printf("-------------------------------------------------------------------------------------------------------\n");

    printf("Voici les rêgles du jeu : \n");
    printf("Phase 1: Questions\n");
    printf("1. Une question sera posée, la réponse est une lettre.\n");
    printf("2. Celui qui trouve la réponse le plus vite remporte la phase 1 et passe en phase 2.\n");
    printf("Phase 2: Mots croisés\n");
    printf("3. La lettre devinée est révélée dans la grille de mots croisés.\n");
    printf("4. Si la lettre est présente dans plusieurs mots, le gagnant de la phase précédente choisit le mot à deviner.\n");
    printf("5. Le gagnant de la phase 1 doit deviner le mot à l'aide des lettres présentes et d'un court indice.\n");
    printf("6. S'il devine le mot, il gagne autant de points qu'il y a de lettres dans le mot deviné.\n");
    printf("7. A ce moment là, n'importe quel joueur peut Slammer, pour rentrer dans la phase 3, sinon retour à la phase 1 ou fin de la manche si tous les mots sont devinés.\n");
    printf("Phase 3: SLAM\n");
    printf("8. Lorsqu'un joueur déclenche le SLAM, il doit tenter de deviner tous les mots incomplets à la suite.\n");
    printf("9. Il n'a pas accès à la définition des mots.\n");
    printf("10. S'il se trompe sur l'un des mots, il perd la partie, retour à la phase 1 pour les autres joueurs.\n");
    printf("11. Comme à la phase 2, il gagne autant de points qu'il y a de lettres dans les mots devinés.\n");
    printf("Autre :\n");
    printf("11. Le gagnant est celui qui a le plus de points une fois la grille complétée.\n");


    printf("- - - En construction - - - ");
    if (close(sock) == -1)
        erreur_IO("close socket");

    return EXIT_SUCCESS;
    

    char phase[L_MAX];
    while (!end) {
        
        output = lireLigne(sock, phase);
        if (output == -1) erreur_IO("lire ligne");

        if(strcmp(phase, "1"))
        {
                /* - - Réception 1 - - */
                output = lireLigne(sock, prompt);
                if (output == -1) erreur_IO("lire ligne");

                printf("%s", prompt);
                
                /* - - Réponse - -*/
                
                // Acquisition de la réponse
                printf("Réponse : \n");
                scanf("%s", prompt);

                // Envoi de la réponse
                output = ecrireLigne(sock, prompt);
                if (output == -1) erreur_IO("ecrire ligne");

                /* - - Réception 2 - - */
                output = lireLigne(sock, prompt);
                if (output == -1) erreur_IO("lire ligne");

                printf("%s", prompt);
        }
        if(strcmp(phase, "2"))
        {

        }
        if(strcmp(phase, "3"))
        {

        }

    }
}