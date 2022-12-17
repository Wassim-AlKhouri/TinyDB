#include "common.hpp"

int safe_socket(int domain, int type, int protocol){
    int test = socket(domain,type,protocol);
    if(test<0){perror("Socket error");exit(1);}
    else return test;
}

void safe_bind(int fd, const sockaddr *addr, socklen_t len){
    if(bind(fd,addr,len)<0){perror("Bind error");exit(1);}
    else return;
}

void safe_listen(int fd, int n){
    if(listen(fd,n)<0){perror("Listen error");exit(1);}
    else return;
}

int safe_accept(int fd, sockaddr *__restrict__ addr, socklen_t *__restrict__ addr_len){
    int test = accept(fd,addr,addr_len);
    if(test<0){perror("Accept error");exit(1);}
    else return test;
}

void safe_connect(int fd, const sockaddr *addr, socklen_t len){
    if(connect(fd,addr,len)<0){perror("Connect error");exit(1);}
    else return;
}

size_t safe_write(int fd, const void* buffer, size_t nbytes){
    ssize_t bytes_written = write(fd, buffer, nbytes);
    if (bytes_written < 0) {
      perror("Write error");
      exit(1);
    }
    return (size_t)bytes_written;
}