MALS_RIQUIER_VINCENT : main.o fonctions.o 
	gcc -Wall -Wextra -g main.o fonctions.o -o Ex1

main.o : main.c main.h
	gcc -c main.c

fonctions.o : fonctions.c fonctions.h main.h
	gcc -c fonctions.c