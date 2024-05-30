Serveur_start : serveur.o jeu_serveur.o
	gcc -Wall -Wextra -g serveur.o jeu_serveur.o -o Serveur_start

serveur.o : serveur.c serveur.h jeu_serveur.h ../LIB/all_lib.h 
	gcc -c serveur.c

jeu_serveur.o : jeu_serveur.c jeu_serveur.h ../LIB/all_lib.h 
	gcc -c jeu_serveur.c