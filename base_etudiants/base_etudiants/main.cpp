#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sstream>
//#include <stdatomic.h>
#include <signal.h>
#include <sys/mman.h>

#include "db.h"
#include "query.h"
#include "utils.h"
#include "student.h"
#include "parsing.h"
#include "child_functions.h"
#define WRITE 1
#define READ 0

int SIGRECIEVED = 0;
void USR1_handler(int signal){
	//handler pour les signaux de type SIGUSR1
	if(signal == SIGUSR1){
		
	}else{
		printf("Unknown signal");
	}
}

void INT_handler(int signal){
	//handler pour les signaux de type SIGINT
	if(signal == SIGINT){
		printf("Waiting for requests to terminate...\n");
		printf("Commiting database changes to the disk...\n");
		SIGRECIEVED = 1;
		exit(0);
	}else{
		printf("Unknown signal");
	}
}

int main(int argc, char const *argv[]) {
	printf("Welcome to the Tiny Database!\n");
	printf("Loading the database...\n");
	const char *db_path = argv[1];
	database_t* db = (database_t*)create_shared_memory(sizeof(database_t));
	db_init(db);
	db_load(db, db_path);
	printf("Done!\n");
	volatile sig_atomic_t queries = 0;
	pid_t pids[5]= {0,0,0,0,0};
	int fd_s[2];
	int fd_i[2];
	int fd_d[2];
	int fd_u[2];
	if(pipe(fd_s)<0 || pipe(fd_i)<0 || pipe(fd_d)<0 || pipe(fd_u)<0){
		perror("Pipe failed");
		exit(0);
	}
	
	if (fork()==0){
		//fils 1
		pids[1] = getpid();
	}
	else {
		pids[0] = getpid();
		if(fork()==0){
			//fils 2
			pids[2] = getpid();
		}
		else{
			if(fork()==0){
				//fils 3
				pids[3] = getpid();
			}
			else{
				if(fork()==0){
					//fils 4
					pids[4] = getpid();
				}
			}
		}
	}
	
	pid_t pid = getpid();
	if(pid == pids[0]){
		// processus père
		signal(SIGUSR1,USR1_handler);
		signal(SIGINT,&INT_handler);
		if(close(fd_s[READ])<0 || close(fd_i[READ])<0 || close(fd_d[READ])<0 || close(fd_u[READ])<0){
			perror("Close pipe failed");
			exit(0);
		}
		while(!SIGRECIEVED){
			char query[256];
			char query_type[20];
			int i = 0;
			scanf(" %[^\t\n]",query);
			printf("Running query '%s'\n",query);
			while(query[i] != ' '){i++;}
			strncpy(query_type, query, i);
			if(!strcmp(query_type,"select")){
				safe_write(fd_s[WRITE],&query,256*sizeof(char));
				queries +=1;
			}else if(!strcmp(query_type,"insert")){
				safe_write(fd_i[WRITE],&query,256*sizeof(char));
				queries +=1;
			}else if(!strcmp(query_type,"delete")){
				safe_write(fd_d[WRITE],&query,256*sizeof(char));
				queries +=1;
			}else if(!strcmp(query_type,"update")){
				safe_write(fd_u[WRITE],&query,256*sizeof(char));
				queries +=1;
			}else{
				printf("demande mal formée\n");
			}
			//printf("%i\n",queries);
		}
		db_save(db, db_path);
		printf("Bye bye!\n");
		munmap(db->data,db->psize);
		munmap(db,sizeof(database_t));
		return 0;
	}else if(pid == pids[1]){
		// processus fils: select
		if(close(fd_s[WRITE])){
			perror("Close pipe_s failed");
		}
		while(1){
			if(select(fd_s, db)){
				printf("select query done\n");
				queries -=1;
			}
		}
		
	}else if(pid == pids[2]){
		// processus fils: insert	
		if(close(fd_i[WRITE])){
			perror("Close pipe_i failed");
		}
		while(1){
			if(insert(fd_i, db)){
				printf("insert query done\n");
				queries -=1;
			}
		}
	}else if(pid == pids[3]){
		// processus fils: delete
		if(close(fd_d[WRITE])){
			perror("Close pipe_d failed");
		}
		while(1){
			if(del(fd_d, db)){
				printf("delete query done\n");
				queries -=1;
			}
		}
		
	}else if(pid == pids[4]){
		// processus fils: update
		if(close(fd_u[WRITE])){
			perror("Close pipe_u failed");
		}
		while(1){
			if(update(fd_u, db)){
				printf("update query done\n");
				queries -=1;
			}
		}
	}
}
