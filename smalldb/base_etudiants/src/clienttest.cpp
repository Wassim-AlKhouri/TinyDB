#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// Permet de définir un gestionnaire de signaux pour SIGPIPE,
// ce qui évite une fermeture abrupte du programme à la réception
// du signal (utile si vous avez des opérations de nettoyage à
// faire avant de terminer le programme)
#include <signal.h>

#include "common.hpp"
#include "student.hpp"

int main(void) {
  // Permet que write() retourne 0 en cas de réception
  // du signal SIGPIPE.
  signal(SIGPIPE, SIG_IGN);
  
  //int sock = checked(socket(AF_INET, SOCK_STREAM, 0));
  int sock = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(28772);

  // Conversion de string vers IPv4 ou IPv6 en binaire
  inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

  //checked(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)));
  connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  
  char buffer[1024];
  
  int longueur, i, ret;
   printf(">");
  while (fgets(buffer, 1024, stdin) != NULL) {
      buffer[strlen(buffer)] = '\0';
      longueur = strlen(buffer)+1;
      printf("Envoi...\n");
      write(sock, buffer, longueur);
      char c;
      int i=0;
      size_t test = read(sock,&c,sizeof(char));
      while( c != '\0' && test > 0){
         printf("%c",c);
         i++;
         test = read(sock,&c,sizeof(char));
      }
      if (test == 0){
         printf("Server has disconnected\n");
         exit(0);
      }
      else if(test < (size_t)0){perror("[-]Error while reading in the socket.");} 
      memset(buffer,'\0',strlen(buffer));
      c='A';
      printf("finfi\n");
      printf(">");
   }
  close(sock);
  return 0;
}