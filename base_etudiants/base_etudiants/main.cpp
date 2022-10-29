#include <stdio.h>
#include <unistd.h>
#include "db.h"
#define W 1
#define R 0


size_t safe_read(int fd, void* buffer, size_t nbytes) {
  ssize_t nbytes_read = read(fd, buffer, nbytes);
  if (nbytes_read < 0) {
    perror("read error: ");
    exit(1);
  }
  return (size_t)nbytes_read;
}

size_t safe_write(int fd, const void* buffer, size_t nbytes) {
  ssize_t bytes_written = write(fd, buffer, nbytes);
  if (bytes_written < 0) {
    perror("write: ");
    exit(1);
  }
  return (size_t)bytes_written;
}


void select(){
	close[fd_s[W]];
	query_result_t demande; 
	safe_read(de_s,&demande,sizeof(query_result_t));
	
}


void insert(){
	close[fd_i[W]];
	query_result_t demande; 
	safe_read(de_s,&demande,sizeof(query_result_t));
}


void del(){
	close[fd_d[W]];	
	query_result_t demande; 
	safe_read(de_s,&demande,sizeof(query_result_t));
}


void update(){
	close[fd_u[W]];
	query_result_t demande; 
	safe_read(de_s,&demande,sizeof(query_result_t));
}


int main(int argc, char const *argv[]) {
	//const char *db_path = "";
	//database_t db;
	//db_init(&db);
	//db_load(&db, db_path);
	pid_t pids[4];
	int fd_s[2];
	int fd_i[2];
	int fd_d[2];
	int fd_u[2];
	int pipe (int fd_s[2]);
	int pipe (int fd_i[2]);
	int pipe (int fd_d[2]);
	int pipe (int fd_u[2]);
	if (fork()==0){
		//fils 1;select 
		pids[0] = getpid();
		select();
	}
	else {
		if(fork()==0){
			//fils 2; insert
			
			pids[1] = getpid();
			insert();
		}
		else{
			if(fork()==0){
				//fils 3; delete
				
				pids[2] = getpid();
				del();
			}
			else{
				if(fork()==0){
					//fils 4; update
					
					pids[3] = getpid();
					update();
				}
			}
		}
	}
	while(1){
		char query[256];
		query_result_t result;
		query_result_init(result,query);
	}
	//db_save(&db, db_path);
	printf("Bye bye!\n");
	return 0;
}
