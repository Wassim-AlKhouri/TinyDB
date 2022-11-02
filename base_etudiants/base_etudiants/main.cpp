#include <stdio.h>
#include <unistd.h>
#include <string>
#include "db.h"
#include "query.h"
#include "utils.h"
#include "student.h"
#include "parsing.h"
#define WRITE 1
#define READ 0


void select(int* fd_s,database_t* db){
	safe_close(fd_s[WRITE]);
	query_result_t result; 
	int success = 0;
	safe_read(fd_s[READ],&result,sizeof(query_result_t));
	char field[64];
	char value[64];
	if(parse_selectors(result->query,field,value)){
		printf("wrong type of query")
		success = 1;
	}else{
		for(int i=0;i>=db->lsize;i++){
			if(!strcmp(field,"fname") && value==db->data[i].fname){
				query_result_add(result,db->data[i]);
			}else if(!strcmp(field,"lname") && value==db->data[i].lname){
				query_result_add(result,db->data[i]);
			}else if(!strcmp(field,"id") && value==db->data[i].id){
				query_result_add(result,db->data[i]);
			}else if(!strcmp(field,"birthdate") && value==db->data[i].birthdate){
				query_result_add(result,db->data[i]);
			}else if(!strcmp(field,"section") && value==db->data[i].section){
				query_result_add(result,db->data[i]);
			}else {
				printf("wrong type of query");
				success = 1;
			}
		}
		if(success){
			result.status = QUERY_SUCCESS;
			log_query(&result);
		}
	}
}


void insert(int* fd_i,database_t* db){
	safe_close(fd_i[WRITE]);
	query_result_t result; 
	int success = 0;
	safe_read(fd_i[READ],&result,sizeof(query_result_t));
	student_t new_student;
	if(parse_insert(result->query,new_student.fname,new_student.lname,new_student.id,new_student.section,new_student.birthdate)){
		printf("wrong type of query")
	}else{
		int exist = 1;
		for(int i=0;i>=db->lsize;i++){
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
	}
}


void del(int* fd_d,database_t* db){
	safe_close(fd_d[WRITE]);	
	query_result_t result; 
	safe_read(fd_d[READ],&result,sizeof(query_result_t));
	int success = 0;
	char field[64];
	char value[64];
	if(parse_selectors(quary,field,value)){
		printf("wrong type of query")
		success = 1;
	}else{
		for(int i=0;i>=db->lsize;i++){
			if(!strcmp(field,"fname") && value==db->data[i].fname){
				db->data[i];
			}else if(!strcmp(field,"lname") && value==db->data[i].lname){
				db->data[i];
			}else if(!strcmp(field,"id") && value==db->data[i].id){
				db->data[i];
			}else if(!strcmp(field,"birthdate") && value==db->data[i].birthdate){
				db->data[i];
			}else if(!strcmp(field,"section") && value==db->data[i].section){
				db->data[i];
			}else {
				printf("wrong type of query");
				success = 1;
			}
		}
		if(success){
			result.status = QUERY_SUCCESS;
			log_query(&result);
		}
	}
}


void update(int* fd_u,database_t* db){
	safe_close(fd_u[WRITE]);
	query_result_t result; 
	int success = 0;
	safe_read(fd_u[READ],&result,sizeof(query_result_t));
	char* field_filter;
	char* value_filter;
	char* field_to_update;
	char* update_value;
	if(parse_update(result.query,field_filter,value_filter,field_to_update,update_value)){
			printf("wrong type of query");
	}else{
		for(int i=0;i>=db->lsize;i++){
			if(!strcmp(field_filter,"fname") && update_value==db->data[i].fname){
				if(!strcmp(field_to_update,"fname")){
					db->data[i].fname = update_value;
				}else if(!strcmp(field_to_update,"lname")){
					db->data[i].lname = update_value;
				}else if(!strcmp(field_to_update,"id")){
					db->data[i].id = update_value;
				}else if(!strcmp(field_to_update,"birthdate")){
					db->data[i].birthdate = update_value;
				}else if(!strcmp(field_to_update,"section")){
					db->data[i].section = update_value;
				}else {
					printf("wrong type of query");
					success = 1;
				}
			}else if(!strcmp(field_filter,"lname") && value_filter==db->data[i].lname){
				if(!strcmp(field_to_update,"fname")){
					db->data[i].fname = update_value;
				}else if(!strcmp(field_to_update,"lname")){
					db->data[i].lname = update_value;
				}else if(!strcmp(field_to_update,"id")){
					db->data[i].id = update_value;
				}else if(!strcmp(field_to_update,"birthdate")){
					db->data[i].birthdate = update_value;
				}else if(!strcmp(field_to_update,"section")){
					db->data[i].section = update_value;
				}else {
					printf("wrong type of query");
					success = 1;
				}
			}else if(!strcmp(field_filter,"id") && value_filter==db->data[i].id){
				if(!strcmp(field_to_update,"fname")){
					db->data[i].fname = update_value;
				}else if(!strcmp(field_to_update,"lname")){
					db->data[i].lname = update_value;
				}else if(!strcmp(field_to_update,"id")){
					db->data[i].id = update_value;
				}else if(!strcmp(field_to_update,"birthdate")){
					db->data[i].birthdate = update_value;
				}else if(!strcmp(field_to_update,"section")){
					db->data[i].section = update_value;
				}else {
					printf("wrong type of query");
					success = 1;
				}
			}else if(!strcmp(field_filter,"birthdate") && value_filter==db->data[i].birthdate){
				if(!strcmp(field_to_update,"fname")){
					db->data[i].fname = update_value;
				}else if(!strcmp(field_to_update,"lname")){
					db->data[i].lname = update_value;
				}else if(!strcmp(field_to_update,"id")){
					db->data[i].id = update_value;
				}else if(!strcmp(field_to_update,"birthdate")){
					db->data[i].birthdate = update_value;
				}else if(!strcmp(field_to_update,"section")){
					db->data[i].section = update_value;
				}else {
					printf("wrong type of query");
					success = 1;
				}
			}else if(!strcmp(field_filter,"section") && value_filter==db->data[i].section){ // example a suivre
				if(!strcmp(field_to_update,"fname")){
					strcpy(db->data[i].fname , update_value);
				}else if(!strcmp(field_to_update,"lname")){
					strcpy(db->data[i].lname , update_value);
				}else if(!strcmp(field_to_update,"id")){
					db->data[i].id = std::stoul(update_value);
				}else if(!strcmp(field_to_update,"birthdate")){
					char buff[sizeof(struct tm)];
					strftime(buff,sizeof(struct tm);
					strcpy(db->data[i].birthdate , update_value);
				}else if(!strcmp(field_to_update,"section")){
					strcpy(db->data[i].section ,update_value);
				}else {
					printf("wrong type of query");
					success = 1;
				}
			}else {
				printf("wrong type of query");
				success = 1;
			}
		}
		if(success){
			result.status = QUERY_SUCCESS;
			log_query(&result);
		}
	}
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
	safe_close(fd_s[READ]);
	int pipe (int fd_i[2]);
	safe_close(fd_i[READ]);
	int pipe (int fd_d[2]);
	safe_close(fd_d[READ]);
	int pipe (int fd_u[2]);
	safe_close(fd_u[READ]);
	if (fork()==0){
		//fils 1; select 
		pids[0] = getpid();
		select(fd_s,&db);
	}
	else {
		if(fork()==0){
			//fils 2; insert
			
			pids[1] = getpid();
			insert(fd_i,&db);
		}
		else{
			if(fork()==0){
				//fils 3; delete
				
				pids[2] = getpid();
				del(fd_d,&db);
			}
			else{
				if(fork()==0){
					//fils 4; update
					
					pids[3] = getpid();
					update(fd_u,&db);
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
		while(query[i] != ' '){i++;}
		strncpy(query_type, query, i);
		query_result_init(&result,query);
		if(!strcmp(query_type,"select")){
            safe_write(fd_s[WRITE],&result,sizeof(query_result_t));
        }else if(!strcmp(query_type,"insert")){
            safe_write(fd_i[WRITE],&result,sizeof(query_result_t));
        }else if(!strcmp(query_type,"delete")){
            safe_write(fd_d[WRITE],&result,sizeof(query_result_t));
        }else if(!strcmp(query_type,"update")){
            safe_write(fd_u[WRITE],&result,sizeof(query_result_t));
        }else{
            printf("demande mal formée");
        }
	}
	db_save(&db, db_path);
	printf("Bye bye!\n");
	return 0;
}
