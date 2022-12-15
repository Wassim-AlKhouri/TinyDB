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

static volatile std::atomic<int> run(1);
static volatile std::atomic<int> end(0);
static volatile std::atomic<int> queries(0);
static volatile std::atomic<int> threads(0);
static database_t* db = new database_t;


void handler(int sig){
  if(sig == SIGINT){
    // The server stop accepting queries, saves the changes and shuts down
    run.fetch_sub(1);
    end.fetch_add(1);
    while(queries.load() > 0){sleep(1);}
    db_save(db);
  }else if(sig == SIGUSR1){
    // The server saves the changes done to the database
    run.fetch_sub(0);
    printf("Waiting for all queries to end ...\n");
    while(queries.load() > 0){sleep(1);}
    printf("Saving changes ...\n");
    db_save(db);
    printf("DONE!\n");
    run.fetch_add(1);
  }
}


void* f(void * ptr){
  thread_args_t* args = (thread_args_t*) ptr;
  threads.fetch_add(1);
  char query[1024];
  char result[1024];
  size_t test = 1;
  char c;
  int i=0;
  while(test > 0 && end.load() == 0){
    if(run.load() == 1){
      test = read(args->socket,&c,sizeof(char));
      query[i]=c; // a augmanter la taille
      i++;
      if(c == '\0'){
        queries.fetch_add(1);
        i=0;
        query[strlen(query)] = '\0';
        int type = parse(query);
        switch (type){
        case 0: //writer
          pthread_mutex_lock(args->new_access_mutex);
          pthread_mutex_lock(args->write_access_mutex);
          pthread_mutex_unlock(args->new_access_mutex);
          parse_and_execute(args->socket, args->db, query);
          pthread_mutex_unlock(args->write_access_mutex);
          break;

        case 1: //reader
          //printf("ICI1\n");
          pthread_mutex_lock(args->new_access_mutex);
          //printf("ICI2\n");
          pthread_mutex_lock(args->reader_registration_mutex);
          //printf("ICI3\n");
          if (args->socket,args->readers->load() == 0){pthread_mutex_lock(args->write_access_mutex);}
          args->socket,args->readers->fetch_add(1);
          //printf("ICI4\n");
          pthread_mutex_unlock(args->new_access_mutex);
          //printf("ICI5\n");
          pthread_mutex_unlock(args->reader_registration_mutex);
          //printf("ICI6\n");
          parse_and_execute(args->socket, args->db, query);
          //printf("ICI7\n");
          pthread_mutex_lock(args->reader_registration_mutex);
          //printf("ICI8\n");
          args->socket,args->readers->fetch_sub(1);
          if (args->socket,args->readers->load() == 0){pthread_mutex_unlock(args->write_access_mutex);}
          //printf("ICI9\n");
          pthread_mutex_unlock(args->reader_registration_mutex);
          //printf("ICI10\n");
          break;
        
        case -1:
          query_fail_bad_query_type(args->socket);
          break;
        }
        memset(query,'\0',strlen(query));
        c='A';
        queries.fetch_sub(1);
      }
    }else{sleep(1);}
  };
  if(end.load() == 1){
    printf("Closing connection to client %i\n",args->socket);
    close(args->socket);
    delete(args);
    threads.fetch_sub(1);
    return NULL;
  }else if (test == 0){
      printf("Client %i disconnected (normal). Closing connection and thread\n",args->socket);
      close(args->socket);
      delete(args);
      threads.fetch_sub(1);
      return NULL;
  }else{
    printf("Lost connection to client %i\n",args->socket);
    printf("Closing connection\n");
    close(args->socket);
    delete(args);
    printf("Closing thread for connection %i\n",args->socket);
    threads.fetch_sub(1);
    return NULL;
  }
}


int main(int argc, char const* argv[]) {
  // Checks usage
  if(argc != 2){printf("Wrong usage.\n try smalldb [<path_to_the_database>]\n");exit(1);}
  const char* path = argv[1];

  // Loads database
  //database_t* db = new database_t;
  db_load(db,path);

  // Set up signal handler 
  struct sigaction action;
  action.sa_handler = handler;
  sigemptyset(&action.sa_mask);
  action.sa_flags =0;
  sigaction(SIGUSR1,&action,NULL);
  sigaction(SIGINT,&action,NULL);

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
  while(end.load() == 0){
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
  printf("Waiting for all queries to end ...\n");
  while(queries.load() > 0){sleep(1);}
  printf("Saving changes ...\n");
  db_save(db);
  printf("Disconnecting with the clients... \n");
  while(threads.load() > 0){sleep(1);}
  close(server_fd);
  delete(db);
  printf("BYE!\n");
  return 0;
}
