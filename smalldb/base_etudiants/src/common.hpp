#ifndef _COMMON_HPP
#define _COMMON_HPP

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <iostream>
#include <pthread.h>
#include <atomic>

#include "db.hpp"
/*
    
*/
typedef struct{
    int socket;
    database_t* db;
    pthread_mutex_t* new_access_mutex;
    pthread_mutex_t* write_access_mutex;
    pthread_mutex_t* reader_registration_mutex;
    std::atomic<int>* readers;
} thread_args_t;
/*
    Creates a socket while checking the return value
*/
int safe_socket(int domain, int type, int protocol);
/*

*/
void safe_bind(int fd, const sockaddr *addr, socklen_t len);
/*
    checkes if 'accept' was safely 
*/
void safe_listen(int fd, int n);
/*
    safely accept
*/
void safe_accept(int fd, sockaddr *__restrict__ addr, socklen_t *__restrict__ addr_len);
/*
    safely accept
*/
size_t safe_read(int fd, void* buffer, size_t nbytes);
/*
    safely accept
*/
size_t safe_write(int fd, const void* buffer, size_t nbytes);
/*
int _checked(int ret, char* calling_function) {
  if (ret < 0) {
    perror(calling_function);
    exit(1);
  }
  return ret;
}
*/
void safe_size(size_t s);
void safe_int(int i);
void safe_char(char* s);
// The macro allows us to retrieve the name of the calling function
#define checked(call) _checked(call, #call)
#endif  // _COMMON_HPP