#include "jeu_serveur.h"

void choix_grille(int* taille_grille, int ** grille, int* taille_def, char** def)
{
    // Choix de la grille de jeu
    int num_grille = randint(NB_Grille);

    // Récupération de la grille
    char nom_fichier[L_MAX];
    sprintf(nom_fichier, "./SERVEUR/JEU_txt/grille_%d.txt", num_grille);
    
    FILE* f_grille = fopen(nom_fichier, "r");
    fscanf(f_grille, "%d\n", &taille_grille[0]);
    fscanf(f_grille, "%d\n", &taille_grille[1]);

    for(int i=0; i<taille_grille[0]; i++)
    {
        for(int j=0; j<taille_grille[1]; j++)
        {
            fscanf(f_grille, "%d", grille[i][j]);
        }
    }
    
    fclose(f_grille);

    // Récupération des définitions
    sprintf(nom_fichier, "./SERVEUR/JEU_txt/def_%d.txt", num_grille);
    
    FILE* f_def = fopen(nom_fichier, "r");
    fscanf(f_def, " %d\n", taille_def);

    for(int i=0; i<*taille_def; i++)
    {
            fscanf(f_def, "%s", def[i]);
    }
    
    fclose(f_def);


}

void choix_devinettes(char* question ,char* solution, int* q_interdites)
{

}