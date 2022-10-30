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
	query_result_t result; 
	safe_read(fd_s,&result,sizeof(query_result_t));
	char champ[64];
	char valeur[64];
	int i = 0;
	while(result.query[i] != ' '){i++;}
	int j = i;
	while(result.query[j] != '='){j++;}
	for(int x=i+1;x<j;x++){
		champ[x-i-1]=result.query[x];
	}
	int k = j;
	while(result.query[k] != '\0'){k++;}
	for(int x=j+1;x<k;x++){
		valeur[x-j-1]=result.query[x];
	}
	if(champ == "id"){
		for(int s =0;s<=result.lsize;s++){
			if(result.students.id == valeur){query_result_add(result,result.students[s]);}
		}
	}else if(champ == "fname"){
		for(int s =0;s<=result.lsize;s++){
			if(result.students.fname == valeur){query_result_add(result,result.students[s]);}
		}
	}else if(champ == "lname"){
		for(int s =0;s<=result.lsize;s++){
			if(result.students.lname == valeur){query_result_add(result,result.students[s]);}
		}
	}else if(champ == "section"){
		for(int s =0;s<=result.lsize;s++){
			if(result.students.update == valeur){query_result_add(result,result.students[s]);}
		}
	}else if(champ == "birthdate"){
		for(int s =0;s<=result.lsize;s++){
			if(result.students.birthdate == valeur){query_result_add(result,result.students[s]);}
		}
	}else{
		printf("demande mal formée");
	}	
}


void insert(){
	close[fd_i[W]];
	query_result_t result; 
	safe_read(fd_i,&result,sizeof(query_result_t));
	for()
}


void del(){
	close[fd_d[W]];	
	query_result_t result; 
	safe_read(fd_d,&result,sizeof(query_result_t));
}


void update(){
	close[fd_u[W]];
	query_result_t result; 
	safe_read(fd_u,&result,sizeof(query_result_t));
}


int main(int argc, char const *argv[]) {
	const char *db_path = "";
	database_t db;
	db_init(&db);
	db_load(&db, db_path);
	pid_t pids[4];
	int fd_s[2];
	int fd_i[2];
	int fd_d[2];
	int fd_u[2];
	int pipe (int fd_s[2]);
	close[fd_s[R]];
	int pipe (int fd_i[2]);
	close[fd_i[R]];
	int pipe (int fd_d[2]);
	close[fd_d[R]];
	int pipe (int fd_u[2]);
	close[fd_u[R]];
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
		char query_type[6];
		int i = 0;
		scanf("%[^\t\n]",query);
		while(query[i] =! ' '){i++;}
		char *strncpy(char *query_type, const char *query, i);
		query_result_init(result,query);
		if(query_type == "select"){
			safe_write(fd_s;&result;sizeof(query_result_t));
		}else if(query_type == "insert"){
			safe_write(fd_i;&result;sizeof(query_result_t));
		}else if(query_type == "delete"){
			safe_write(fd_d;&result;sizeof(query_result_t));
		}else if(query_type == "update"){
			safe_write(fd_u;&result;sizeof(query_result_t));
		}else{
			printf("demande mal formée");
		}
	}
	db_save(&db, db_path);
	printf("Bye bye!\n");
	return 0;
}
