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
    Launches the server
*/
void server(database_t* db, sigset_t mask);
