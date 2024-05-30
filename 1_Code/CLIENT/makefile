Client_start : client.o jeu_client.o
	gcc -Wall -Wextra -g client.o jeu_client.o -o Client_start

client.o : client.c client.h ../LIB/all_lib.h 
	gcc -c client.c

jeu_client.o : jeu_client.c jeu_client.h ../LIB/all_lib.h 
	gcc -c jeu_client.c