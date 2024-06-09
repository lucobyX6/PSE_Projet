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
    printf("BLABLA\n");
    

    char phase[L_MAX];
    while (!end) {
        
        output = lireLigne(sock, phase);
        if (output == -1) erreur_IO("lire ligne");
        printf("%s", phase);

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

    if (close(sock) == -1)
        erreur_IO("close socket");

  return EXIT_SUCCESS;
}