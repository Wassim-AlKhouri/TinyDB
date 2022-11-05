#include <stdio.h>
#include <unistd.h>
#include <string>
#include <time.h>
#include <sstream>
//#include <stdatomic.h>
#include <signal.h>

#include "db.h"
#include "query.h"
#include "utils.h"
#include "student.h"
#include "parsing.h"
#define WRITE 1
#define READ 0

//atomic_int processus = 0;
void select(int* fd_s,database_t* db){
	safe_close(fd_s[WRITE]);
	query_result_t result; 
	int success = 0;
	safe_read(fd_s[READ],&result,sizeof(query_result_t));
	char field[64];
	char value[64];
	if(parse_selectors(result.query,field,value)){
		printf("wrong type of query");
		success = 1;
	}else{
		for(int i=0;i>=(int)db->lsize;i++){
			if(!strcmp(field,"fname") && value==db->data[i].fname){
				query_result_add(&result,db->data[i]);
			}else if(!strcmp(field,"lname") && value==db->data[i].lname){
				query_result_add(&result,db->data[i]);
			}else if(!strcmp(field,"id")){
				unsigned int UIvalue;
				sscanf(value, "%u", &UIvalue);
				if(UIvalue == db->data[i].id){
					query_result_add(&result,db->data[i]);
				}
			}else if(!strcmp(field,"birthdate")){// à faire
				query_result_add(&result,db->data[i]);
			}else if(!strcmp(field,"section") && value==db->data[i].section){
				query_result_add(&result,db->data[i]);
			}else {
				printf("wrong type of query");
				success = 1;
			}
		}
		if(success){
			result.status = QUERY_SUCCESS;
			log_query(&result);
			//processus-=1;
		}
	}
}


void insert(int* fd_i,database_t* db){
	safe_close(fd_i[WRITE]);
	query_result_t result; 
	int success = 0;
	safe_read(fd_i[READ],&result,sizeof(query_result_t));
	student_t new_student;
	if(parse_insert(result.query,new_student.fname,new_student.lname,&new_student.id,new_student.section,&new_student.birthdate)){
		printf("wrong type of query");
	}else{
		int exist = 1;
		for(int i=0;i>=(int)db->lsize;i++){
			if(student_equals(&new_student,&(db->data[i]))){
				printf("student id already exists \n");
				exist = 0;
				success = 1;
			}
		}
		if(!exist){
			db_add(db,new_student);
		}
	}
	if(success){
		result.status = QUERY_SUCCESS;
		log_query(&result);
		//processus-=1;
	}
}


void del(int* fd_d,database_t* db){
	safe_close(fd_d[WRITE]);	
	query_result_t result; 
	safe_read(fd_d[READ],&result,sizeof(query_result_t));
	int success = 0;
	char field[64];
	char value[64];
	if(parse_selectors(result.query,field,value)){
		printf("wrong type of query");
		success = 1;
	}else{
		for(int i=0;i>=(int)db->lsize;i++){
			if(!strcmp(field,"fname") && value==db->data[i].fname){
				db_remove(db,i);
			}else if(!strcmp(field,"lname") && value==db->data[i].lname){
				db_remove(db,i);
			}else if(!strcmp(field,"id")){
				unsigned int UIvalue;
				sscanf(value, "%u", &UIvalue);
				if(UIvalue == db->data[i].id){
					db_remove(db,i);
				}
			}else if(!strcmp(field,"birthdate") ){// à faire
				db_remove(db,i);
			}else if(!strcmp(field,"section") && value==db->data[i].section){
				db_remove(db,i);
			}else {
				printf("wrong type of query");
				success = 1;
			}
		}
		if(success){
			result.status = QUERY_SUCCESS;
			log_query(&result);
			//processus-=1;
		}
	}
}


void update(int* fd_u,database_t* db){
	safe_close(fd_u[WRITE]);
	query_result_t result; 
	int success = 0;
	safe_read(fd_u[READ],&result,sizeof(query_result_t));
	char* field_filter = (char*)malloc(64*sizeof(char));
	char* value_filter= (char*)malloc(64*sizeof(char));
	char* field_to_update= (char*)malloc(64*sizeof(char));
	char* update_value= (char*)malloc(64*sizeof(char));
	if(parse_update(result.query,field_filter,value_filter,field_to_update,update_value)){
			printf("wrong type of query");
	}else{
		for(int i=0;i>=(int)db->lsize;i++){
			
			
			// filtre = fname
			if(!strcmp(field_filter,"fname") && update_value==db->data[i].fname){
				if(!strcmp(field_to_update,"fname")){
					strcpy(db->data[i].fname , update_value);
				}
				
				else if(!strcmp(field_to_update,"lname")){
					strcpy(db->data[i].lname , update_value);
				}
				
				else if(!strcmp(field_to_update,"id")){
					unsigned int UIupdate_value;
					sscanf(update_value, "%u", &UIupdate_value);
					db->data[i].id = UIupdate_value;
				}
				
				else if(!strcmp(field_to_update,"birthdate")){// à faire
					
				}
				
				else if(!strcmp(field_to_update,"section")){
					strcpy(db->data[i].section ,update_value);
				}
				
				else {
					printf("wrong type of query");
					success = 1;
				}
			}
			
			// filtre = lname
			else if(!strcmp(field_filter,"lname") && value_filter==db->data[i].lname){
				if(!strcmp(field_to_update,"fname")){
					strcpy(db->data[i].fname , update_value);
				}
				
				else if(!strcmp(field_to_update,"lname")){
					strcpy(db->data[i].lname , update_value);
				}
				
				else if(!strcmp(field_to_update,"id")){
					unsigned int UIupdate_value;
					sscanf(update_value, "%u", &UIupdate_value);
					db->data[i].id = UIupdate_value;
				}
				
				else if(!strcmp(field_to_update,"birthdate")){// à faire
					
				}
				
				else if(!strcmp(field_to_update,"section")){
					strcpy(db->data[i].section ,update_value);
				}
				
				else {
					printf("wrong type of query");
					success = 1;
				}
			}
			
			// filtre = id
			else if(!strcmp(field_filter,"id")){
				unsigned int UIvalue_filter;
				sscanf(value_filter, "%u", &UIvalue_filter);
				if(UIvalue_filter == db->data[i].id){
					if(!strcmp(field_to_update,"fname")){
						strcpy(db->data[i].fname , update_value);
					}
					
					else if(!strcmp(field_to_update,"lname")){
						strcpy(db->data[i].lname , update_value);
					}
					
					else if(!strcmp(field_to_update,"id")){
						unsigned int UIupdate_value;
						sscanf(update_value, "%u", &UIupdate_value);
						db->data[i].id = UIupdate_value;
						
					}
					
					else if(!strcmp(field_to_update,"birthdate")){// à faire
						
					}
					
					else if(!strcmp(field_to_update,"section")){
						strcpy(db->data[i].section ,update_value);
					}
					
					else {
						printf("wrong type of query");
						success = 1;
					}
				}
			}
			
			// filtre = birthdate
			else if(!strcmp(field_filter,"birthdate")){//à faire
				//&& value_filter==db->data[i].birthdate
				if(!strcmp(field_to_update,"fname")){
					strcpy(db->data[i].fname , update_value);
				}
				
				else if(!strcmp(field_to_update,"lname")){
					strcpy(db->data[i].lname , update_value);
				}
				
				else if(!strcmp(field_to_update,"id")){
					unsigned int UIupdate_value;
					sscanf(update_value, "%u", &UIupdate_value);
					db->data[i].id = UIupdate_value;
				}
				
				else if(!strcmp(field_to_update,"birthdate")){// à faire
					
				}
				
				else if(!strcmp(field_to_update,"section")){
					strcpy(db->data[i].section ,update_value);
				}
				
				else {
					printf("wrong type of query");
					success = 1;
				}
			}
			
			// filtre = section
			else if(!strcmp(field_filter,"section") && value_filter==db->data[i].section){ 
				// example a suivre
				if(!strcmp(field_to_update,"fname")){
					strcpy(db->data[i].fname , update_value);
				}
				
				else if(!strcmp(field_to_update,"lname")){
					strcpy(db->data[i].lname , update_value);
				}
				
				else if(!strcmp(field_to_update,"id")){
					unsigned int UIupdate_value;
					sscanf(update_value, "%u", &UIupdate_value);
					db->data[i].id = UIupdate_value;
				}
				
				else if(!strcmp(field_to_update,"birthdate")){// à faire
					
				}
				
				else if(!strcmp(field_to_update,"section")){
					strcpy(db->data[i].section ,update_value);
				}
				
				else {
					printf("wrong type of query");
					success = 1;
				}
			}
			
			
			// mauvais filtre
			else {
				printf("wrong type of query");
				success = 1;
			}
		}
		
		if(success){
			result.status = QUERY_SUCCESS;
			log_query(&result);
			//processus-=1;
		}
	}
	free(field_filter);
	free(value_filter);
	free(field_to_update);
	free(update_value);
}

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
	pid_t pids[5]= {0,0,0,0,0};
	int fd_s[2];
	int fd_i[2];
	int fd_d[2];
	int fd_u[2];
	int pipe (int fd_s[2]);
	safe_close(fd_s[READ]);
	int pipe (int fd_i[2]);
	safe_close(fd_i[READ]);
	int pipe (int fd_d[2]);
	safe_close(fd_d[READ]);
	int pipe (int fd_u[2]);
	safe_close(fd_u[READ]);
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
				//processus+=1;
			}else if(!strcmp(query_type,"insert")){
				safe_write(fd_i[WRITE],&result,sizeof(query_result_t));
				//processus+=1;
			}else if(!strcmp(query_type,"delete")){
				safe_write(fd_d[WRITE],&result,sizeof(query_result_t));
				//processus+=1;
			}else if(!strcmp(query_type,"update")){
				safe_write(fd_u[WRITE],&result,sizeof(query_result_t));
				//processus+=1;
			}else{
				printf("demande mal formée");
			}
		}
		db_save(db, db_path);
		printf("Bye bye!\n");
		return 0;
	}else if(pid == pids[1]){
		// fprocessus fils: select
		while(1){
			select(fd_s, db);
		}
		
	}else if(pid == pids[2]){
		// fprocessus fils: insert
		while(1){
			insert(fd_i, db);
		}
	}else if(pid == pids[3]){
		// fprocessus fils: delete
		while(1){
			del(fd_d, db);
		}
		
	}else if(pid == pids[4]){
		// fprocessus fils: update
		while(1){
			update(fd_u, db);
		}
	}
}
