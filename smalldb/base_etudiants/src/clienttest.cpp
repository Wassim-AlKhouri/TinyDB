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
  //char null;
  //size_t test = 1;

  while (fgets(buffer, 1024, stdin) != NULL) {
      //test = recv(sock, &null, sizeof(char),MSG_PEEK) ;
      longueur = strlen(buffer) + 1;
      printf("Envoi...\n");
      write(sock, buffer, strlen(buffer) + 1);
      char c;
      int i=0;
      //char buff[1024];
      while(recv(sock,&c,sizeof(char),0) > 0 && c != '\0'){
         printf("%c",c);
         //printf("pas fini\n");
         //buff[i] = c;
         i++;
      }
      //buff[i] = '\0';
      //printf("%s\n",buff);
      printf("finfi\n\n");
      //checked(write(sock, buffer, strlen(buffer) + 1));
      /*
      int type;
      read(sock,&type,sizeof(int));
      switch (type)
      {
      case 0:
         // In case of error (syntax,...)
         size_t len;
         char buff[1024];
         read(sock,&len,sizeof(size_t));
         read(sock,&buff,len);
         printf("%s\n",buff);
         break;
      case 1:
         // select
         int l;
         read(sock,&l,sizeof(int));
         for (int i = 0; i < l; i++)
         {
            student_t s;
            char buff[1024];
            read(sock, &s, sizeof(student_t));
            student_to_str(buff,&s,1024);
            printf("%s\n",buff);
         }
         break;
      
      default:
         perror("Unknown type");
         exit(1);
         break;
      }

      student_t s;
      i = 0;
      read(sock, &s, sizeof(student_t));
      char buff[1024];
      student_to_str(buff,&s,1024);
      printf("%s\n",buff);
      */
   }
  close(sock);
  return 0;
}