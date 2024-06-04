#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]) {

    char *args[] = {"watch", "-n", "5", "cat", "Tx.txt", NULL};
    fork();
    execvp("watch", args);

    return 0;
}
