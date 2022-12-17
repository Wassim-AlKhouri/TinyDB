#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>

#include "common.hpp"
#include "student.hpp"

int main(int argc, char const* argv[]) {
   // Check usage
   if(argc != 2){printf("Wrong usage.\ntry sdb_client [<IP_adress_of_server>]\n");exit(1);}
   const char* IP = argv[1];
   // Blocks SIGPIPE
   signal(SIGPIPE, SIG_IGN);
   int sock = safe_socket(AF_INET, SOCK_STREAM, 0);
   struct sockaddr_in serv_addr;
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_port = htons(28772);
   inet_pton(AF_INET, IP, &serv_addr.sin_addr); 
   safe_connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
   char buffer[1024];
   int longueur;
   while (fgets(buffer, 1024, stdin) != NULL) {
      buffer[strlen(buffer)] = '\0';
      longueur = strlen(buffer)+1;
      safe_write(sock, buffer, longueur);
      char c;
      int i=0;
      int test = read(sock,&c,sizeof(char));
      while( c != '\0' && test > 0){
         printf("%c",c);
         i++;
         test = read(sock,&c,sizeof(char));
      }
      if (test <= 0){
         printf("Server has disconnected\n");
         exit(0);
      } 
      memset(buffer,'\0',strlen(buffer));
      c='A';
   }
   close(sock);
   return 0;
}