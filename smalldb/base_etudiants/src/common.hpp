#ifndef _COMMON_HPP
#define _COMMON_HPP

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <iostream>
#include <pthread.h>
#include <atomic>
#include <unistd.h>
#include <sys/mman.h>

#include "db.hpp"
/*
    
*/
typedef struct{
    int socket;
    database_t* db;
    pthread_mutex_t* new_access_mutex;
    pthread_mutex_t* write_access_mutex;
    pthread_mutex_t* reader_registration_mutex;
    pthread_mutex_t* sockets_mutex;
    std::atomic<int>* readers;
} thread_args_t;
/*
    Checks the return value of socket
*/
int safe_socket(int domain, int type, int protocol);
/*
    Checks the return value of bind
*/
void safe_bind(int fd, const sockaddr *addr, socklen_t len);
/*
    Checks the return value of listen
*/
void safe_listen(int fd, int n);
/*
    Checks the return value of accept
*/
int safe_accept(int fd, sockaddr *__restrict__ addr, socklen_t *__restrict__ addr_len);
/*
    Checks the return value of connect
*/
void safe_connect(int fd, const sockaddr *addr, socklen_t len);
/*
    Checks the return value of write
*/
size_t safe_write(int fd, const void* buffer, size_t nbytes);

#endif  // _COMMON_HPP