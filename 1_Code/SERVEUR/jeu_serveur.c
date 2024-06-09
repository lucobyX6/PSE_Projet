#include "jeu_serveur.h"

void choix_grille()
{
    // Grille
    int* taille_grille;
    char caractere;

    // Définition
    int taille_def;
    char def[L_MAX];

    // Choix de la grille de jeu
    int num_grille = randint(NB_Grille);

    // Récupération de la grille
    char nom_fichier[L_MAX];
    sprintf(nom_fichier, "./SERVEUR/JEU_txt/grille_%d.txt", num_grille);
    
    // Fichier de lecture
    FILE* f_grille = fopen(nom_fichier, "r");

    // Fichier d'écriture
    FILE* f_grille_w = fopen("grille_jeu", "w");

    fscanf(f_grille, "%d\n", &taille_grille[0]);
    fscanf(f_grille, "%d\n", &taille_grille[1]);
    fprintf(f_grille_w, "%d\n", taille_grille[0]);
    fprintf(f_grille_w, "%d\n", taille_grille[1]);

    for(int i=0; i<taille_grille[0]; i++)
    {
        for(int j=0; j<taille_grille[1]; j++)
        {
            fscanf(f_grille, "%c", &caractere);
            fprintf(f_grille_w,"%c", &caractere);
        }
        fprintf(f_grille_w,"\n");
    }
    
    fclose(f_grille);

    // Récupération des définitions
    sprintf(nom_fichier, "./SERVEUR/JEU_txt/def_%d.txt", num_grille);
    
    // Fichier de lecture
    FILE* f_def = fopen(nom_fichier, "r");
    fscanf(f_def, " %d\n", taille_def);

    // Fichier d'écriture
    FILE* f_def_w = fopen("def_jeu", "w");
    fprintf(f_def_w, " %d\n", taille_def);

    for(int i=0; i<taille_def; i++)
    {
            fscanf(f_def, "%s", def[i]);
            fprintf(f_def_w, " %s\n", def[i]);
    }
    
    fclose(f_def);


}

int char_here(char C, char* Str)
{
    for(int i=0; i<strlen(Str); i++)
    {
        if(Str[i] == C)
            return 1;
    }
    
    return 0;
}

void choix_devinettes(char* question ,char* solution, char* q_interdites)
{   
    int num_lettre = randint(23);
    *solution = num_lettre+65;
    
    while(char_here(*solution, q_interdites) == 1)
    {
        num_lettre = randint(23);
        *solution = num_lettre+65;
    }

    FILE* f_devinettes = fopen("devinettes_pour_lettres.txt", "r");
    char ligne[L_MAX];

    for(int i=0; i<num_lettre; i++)
    {
        fscanf(f_devinettes, "%s", ligne);
    }

    strcpy(question, ligne);

    q_interdites[strlen(q_interdites)] = *solution;
    q_interdites[strlen(q_interdites)] = '\0';


}