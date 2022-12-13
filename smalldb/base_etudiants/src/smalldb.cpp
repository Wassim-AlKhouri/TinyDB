#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <csignal>

#include "db.hpp"
#include "queries.hpp"
#include "student.hpp"
#include "common.hpp"
void* f(void * ptr){
  thread_args_t* args = (thread_args_t*) ptr;
  char query[1024];
  char result[1024];
  printf("%i\n", args->socket);
  while(read(args->socket,query,1024)){

    int type = parse(query);

    switch (type)
    {
    case 0: //writer
    
      pthread_mutex_lock(args->new_access_mutex);
      pthread_mutex_lock(args->write_access_mutex);
      pthread_mutex_unlock(args->new_access_mutex);
      parse_and_execute(args->socket, args->db, query);
      pthread_mutex_unlock(args->write_access_mutex);
      break;

    case 1: //reader
      pthread_mutex_lock(args->new_access_mutex);
      pthread_mutex_lock(args->reader_registration_mutex);
      if (args->readers == 0){pthread_mutex_lock(args->write_access_mutex);}
      args->readers++;
      pthread_mutex_unlock(args->new_access_mutex);
      pthread_mutex_unlock(args->reader_registration_mutex);
      parse_and_execute(args->socket, args->db, query);
      pthread_mutex_lock(args->reader_registration_mutex);
      args->readers--;
      if (args->readers == 0){pthread_mutex_unlock(args->write_access_mutex);}
      pthread_mutex_unlock(args->reader_registration_mutex);
      break;
    
    default:
      query_fail_bad_query_type(args->socket);
      break;
    }

    //printf("demande recu :%s\n",query);
    //FILE* tmp = tmpfile();
    //FILE* stream = fdopen(args->socket,"w");
    //student_t s;
    //char buff[1024];
    //parse_and_execute(args->socket,args->db,query);
    /*
    int type;
    fread(&type,sizeof(int),1,tmp);
    write(args->socket,&type,sizeof(int));
    switch (type)
    {
    case 0:
      break;

    case 1:
      int len;
      fread(&len,sizeof(int),1,tmp);
      write(args->socket, &len, sizeof(int));
      for (int i = 0; i < len; i++)
      {
        student_t s;
        fread(&s,sizeof(student_t),1,tmp);
        write(args->socket, &s, sizeof(student_t));
      }
      break;
    
    default:
      break;
    }
    //fread(&s,sizeof(student_t),1,tmp);
    //printf("ICI3\n");
    //write(args->socket, &s, sizeof(student_t));
    */
  };
  close(args->socket);
  return NULL;
}
int main(int argc, char const* argv[]) {
  //signal(SIGPIPE, SIG_IGN);
  if(argc < 2){printf("A path to the database is needed\n");exit(1);}
  const char* path = argv[1];
  database_t* db = new database_t;
  db_load(db,path);

  pthread_mutex_t new_access_mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_t write_access_mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_t reader_registration_mutex = PTHREAD_MUTEX_INITIALIZER;



  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  int opt = 1;
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  //28772
  address.sin_port = htons(28772);
  bind(server_fd, (struct sockaddr *)&address, sizeof(address));
  listen(server_fd, 3);
  size_t addrlen = sizeof(address);
  while(true){
    int new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    printf("client %i connecté\n",new_socket);
    pthread_t new_thread;
    thread_args_t args = {
      .socket = new_socket,
      .db = db,
      .new_access_mutex = &new_access_mutex,
      .write_access_mutex = &write_access_mutex,
      .reader_registration_mutex = &reader_registration_mutex
    };
    pthread_create(&new_thread, NULL, f, &args);
  }
  
  close(server_fd);
  delete(db);
  return 0;
}
