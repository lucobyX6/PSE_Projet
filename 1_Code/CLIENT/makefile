Client_start : client.o jeu_client.o utilities.o
	gcc -Wall -Wextra -g client.o jeu_client.o utilities.o -o Client_start

client.o : client.c client.h ../LIB/all_lib.h ../LIB/utilities.h
	gcc -c client.c

jeu_client.o : jeu_client.c jeu_client.h ../LIB/all_lib.h ../LIB/utilities.h
	gcc -c jeu_client.c

utilities.o : ../LIB/utilities.c ../LIB/utilities.h ../LIB/all_lib.h 
	gcc -c ../LIB/utilities.c