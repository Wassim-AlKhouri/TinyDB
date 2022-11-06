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
		printf("Commiting database changes to the disk...");
		
	}else{
		printf("Unknown signal");
	}
}

int main(int argc, char const *argv[]) {
	const char *db_path = *argv;
	database_t* db = (database_t*)create_shared_memory(sizeof(database_t));
	db_init(db);
	db_load(db, db_path);
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
	safe_close(fd_s[READ]);safe_close(fd_i[READ]);safe_close(fd_d[READ]);safe_close(fd_u[READ]);
	if (fork()==0){
		//fils 1
		pids[1] = getpid();
	}
	else {
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
		signal(SIGINT,INT_handler);
		while(1){
			char query[256];
			query_result_t result;
			char query_type[6];
			int i = 0;
			scanf("%[^\t\n]",query);
			while(query[i] != ' '){i++;}
			strncpy(query_type, query, i);
			query_result_init(&result,query);
			if(!strcmp(query_type,"select")){
				safe_write(fd_s[WRITE],&result,sizeof(query_result_t));
				queries +=1;
			}else if(!strcmp(query_type,"insert")){
				safe_write(fd_i[WRITE],&result,sizeof(query_result_t));
				queries +=1;
			}else if(!strcmp(query_type,"delete")){
				safe_write(fd_d[WRITE],&result,sizeof(query_result_t));
				queries +=1;
			}else if(!strcmp(query_type,"update")){
				safe_write(fd_u[WRITE],&result,sizeof(query_result_t));
				queries +=1;
			}else{
				printf("demande mal formée");
			}
		}
		db_save(db, db_path);
		printf("Bye bye!\n");
		munmap(db,db->psize)
		return 0;
	}else if(pid == pids[1]){
		// fprocessus fils: select
		while(1){
			select(fd_s, db);
			queries -=1;
		}
		
	}else if(pid == pids[2]){
		// fprocessus fils: insert
		while(1){
			insert(fd_i, db);
			queries -=1;
		}
	}else if(pid == pids[3]){
		// fprocessus fils: delete
		while(1){
			del(fd_d, db);
			queries -=1;
		}
		
	}else if(pid == pids[4]){
		// fprocessus fils: update
		while(1){
			update(fd_u, db);
			queries -=1;
		}
	}
}
