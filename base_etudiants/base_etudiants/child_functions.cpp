#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "child_functions.h"
#include "db.h"
#include "query.h"
#include "utils.h"
#include "student.h"
#include "parsing.h"


#define WRITE 1
#define READ 0


bool select(int* fd_s,database_t* db){
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
		}
	}
	return 0;
}

bool insert(int* fd_i,database_t* db){
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
	return 0;
}

bool del(int* fd_d,database_t* db){
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
	return 0;
}

bool update(int* fd_u,database_t* db){
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
	return 0;
}
