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
/*
    Handler for SIGINT and SIGUSR1
*/
void handler(int sig);
/*
    Thread that takes care of a client's demands
*/
void* client_thread(void * ptr);
/*
    Executes the "delete", "insert" and "update" type of queries
*/
void writer(thread_args_t* args,char*query);
/*
    Executes the "select" type of queries
*/
void reader(thread_args_t* args,char*query);
/*
    Executes the given query
*/
void do_query(char* query, thread_args_t* args, char* c);
/*
    Does the final steps to end the thread
*/
void close_thread(thread_args_t* args, int test);
/*
    Sets up the signal handler
*/
void setup_signal_handler();
/*
    Creates a mask for the signals
*/
sigset_t create_mask();
/*
    Creates a new variable of type thread_args_t to pass tp the thread
    ns = socket
    nac = new_access_mutex
    wam = write_access_mutex
    rrm = reader_registration_mutex
    sm = sockets_mutex
*/
thread_args_t* new_args(int s, pthread_mutex_t* nac,pthread_mutex_t* wam,pthread_mutex_t* rrm,pthread_mutex_t* sm,std::atomic<int>*r);
/*
    Does the final steps to end the server
*/
void close_server(int server_fd, pthread_mutex_t sockets_mutex);
/*
    Launches the server
*/
void server(database_t* db);