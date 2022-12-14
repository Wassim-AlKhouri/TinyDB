#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <csignal>
#include <iostream>
#include <atomic>


#include "db.hpp"
#include "queries.hpp"
#include "student.hpp"
#include "common.hpp"

void handler(int sig){
  if(sig == SIGINT){

  }else if(sig == SIGUSR1){

  }
}


void* f(void * ptr){
  thread_args_t* args = (thread_args_t*) ptr;
  char query[1024];
  char result[1024];
  printf("thread %i\n", args->socket);
  size_t test = 1;
  char c;
  int i=0;
  while(test > 0){
    test = recv(args->socket,&c,sizeof(char),0);
    query[i]=c;
    i++;
    if(c == '\0'){
      i=0;
      query[strlen(query)] = '\0';
      printf("query:%s\n",query);
      int type = parse(query);
      printf("type:%i\n",type);
      switch (type){
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
        if (args->socket,args->readers->load() == 0){pthread_mutex_lock(args->write_access_mutex);}
        args->socket,args->readers->fetch_add(1);
        pthread_mutex_unlock(args->new_access_mutex);
        pthread_mutex_unlock(args->reader_registration_mutex);
        parse_and_execute(args->socket, args->db, query);
        pthread_mutex_lock(args->reader_registration_mutex);
        args->socket,args->readers->fetch_sub(1);
        printf("readers:%i\n",args->readers->load());
        if (args->readers == 0){pthread_mutex_unlock(args->write_access_mutex);}
        pthread_mutex_unlock(args->reader_registration_mutex);
        break;
      
      case -1:
        query_fail_bad_query_type(args->socket);
        break;
      }
      memset(query,'\0',strlen(query));
      c='K';
    }
  };
  if (test == 0){
      printf("Client %i disconnected (normal). Closing connection and thread\n",args->socket);
      close(args->socket);
      delete(args);
      return NULL;
  }
  else{
    printf("Lost connection to client %i\n",args->socket);
    printf("Closing connection\n");
    close(args->socket);
    delete(args);
    printf("Closing thread for connection %i\n",args->socket);
    return NULL;
  }
}


int main(int argc, char const* argv[]) {
  static volatile int* queries = new int[1];
  // Checks usage
  if(argc != 2){printf("Wrong usage.\n try smalldb [<path_to_the_database>]\n");exit(1);}
  const char* path = argv[1];

  // Loads database
  database_t* db = new database_t;
  db_load(db,path);

  // Set up signal handler 
  struct sigaction action;
  action.sa_handler = handler;
  sigemptyset(&action.sa_mask);
  action.sa_flags =0;
  sigaction(SIGUSR1,&action,NULL);

  // Set up masks
  sigset_t mask;
  sigemptyset(&mask);
  sigaddset(&mask,SIGUSR1);
  sigaddset(&mask,SIGINT);

  // Creat the three mutex needed and readers
  pthread_mutex_t new_access_mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_t write_access_mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_t reader_registration_mutex = PTHREAD_MUTEX_INITIALIZER;
  std::atomic<int> readers(0);

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
    thread_args_t *args = new thread_args_t[1];
    args->socket = new_socket;
    args->db = db;
    args->new_access_mutex = &new_access_mutex;
    args->write_access_mutex = &write_access_mutex;
    args->reader_registration_mutex = &reader_registration_mutex;
    args->readers = &readers;
    // Block signal
    sigprocmask(SIG_BLOCK,&mask,NULL);

    // Create thread
    pthread_create(&new_thread, NULL, f, args);

    // Unblock signal
    sigprocmask(SIG_UNBLOCK,&mask,NULL);
  }
  close(server_fd);
  delete(db);
  return 0;
}
