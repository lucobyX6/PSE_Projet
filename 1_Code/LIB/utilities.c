#include "utilities.h"

/* Ce fichier contient les fonctions du dossier PSE, qui sont utiles à la mise en place du projet */

void erreur_IO (char* msg) 
{
    perror (msg);
    exit (EXIT_FAILURE);
}

void erreur ( const char *format, ... ) 
{
  va_list liste_arg;
  fflush (stdout);
  fprintf (stderr, "[ERREUR] ");
  va_start (liste_arg, format);
  vfprintf (stderr, format, liste_arg);
  va_end (liste_arg);
  fflush (stderr);
  exit (EXIT_FAILURE);
}

struct sockaddr_in *resolv (const char *host, const char *service) 
{
  int code;
  struct addrinfo hints;

  memset ( &hints, 0, sizeof (struct addrinfo) );

  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  freeResolv();

  code = getaddrinfo ( host, service, &hints, &static_infos );
  if ( code == 0)
    return (struct sockaddr_in *) static_infos->ai_addr;
  else {
    fprintf ( stderr, "Erreur: %s\n", gai_strerror(code) );
    return NULL;
  }
}

char *stringIP (unsigned int entierIP) 
{
  struct in_addr ia;
  ia.s_addr = htonl (entierIP); 
  return inet_ntoa(ia);
}

void freeResolv (void) 
{
  if (static_infos != NULL) {
    freeaddrinfo (static_infos);
  }
}

int lireLigne(int fd, char *buffer) 
{
  ssize_t nbLus;
  int i;
  char car;

  if (buffer == NULL) {
    errno = EINVAL;
    return -1;
  }

  i = 0;
  while (i < L_MAX - 1) {
    nbLus = read(fd, &car, 1);   
    if (nbLus == -1) {
      return -1;
    }      
    else if (nbLus == 0) {
      buffer[i] = '\0';
      return 0;
    }
    else if (car == '\n') {
      buffer[i] = '\0';
      i++;
      return i;
    }
    buffer[i] = car;
    i++;      
  }
  
  buffer[i] = '\0';
  return L_MAX;
}

int ecrireLigne(int fd, char *buffer) 
{
  char *position;
  int taille, ecr, nbecr;

  position = strchr(buffer, '\n');
  if (position == NULL) {
    taille = strlen(buffer);
    if (taille >= L_MAX - 1) {
      errno = EINVAL;
      return -1;
    }
    buffer[taille++] = '\n';
    buffer[taille] = '\0';
  }
  else {
    taille = position - buffer + 1;
  }

  nbecr = taille;

  while (taille > 0) {
    ecr = write(fd, buffer, taille);
    if (ecr == -1) {
      return -1;
    }
    taille -= ecr;
  }
  return nbecr;
}