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
#define TRUE 1
#define FALSE 0

bool select(int* fd_s,database_t* db){
	query_result_t result;
	char query[256];
	safe_read(fd_s[READ],&query,256*sizeof(char));
	query_result_init(&result,query);
	bool success = false;
	char field[64];
	char value[64];
	if(!parse_selectors(result.query,field,value)){
		printf("wrong type of query\n");
		success = FALSE;
	}else{
		printf("%s\n", value);
		printf("%s\n", field);
		for(int i=0;i<=(int)db->count;i++){
			if(!strcmp(field,"select fname")){
				if(!strcmp(value,db->data[i].fname)){
					printf("c bon\n");
					query_result_add(&result,db->data[i]);
					success = true;
				}
			}else if(!strcmp(field,"lname") ){
				if(!strcmp(value,db->data[i].lname)){
					query_result_add(&result,db->data[i]);
					success = true;
				}
			}else if(!strcmp(field,"id")){
				unsigned int UIvalue;
				sscanf(value, "%u", &UIvalue);
				if(UIvalue == db->data[i].id){
					query_result_add(&result,db->data[i]);
					success = true;
				}
			}else if(!strcmp(field,"birthdate")){// à faire
				struct tm birthdate;
				parse_birthdate(value,&birthdate);
				double diff = difftime( mktime(&birthdate) , mktime(&db->data[i].birthdate) );
				if(diff == 0){
					query_result_add(&result,db->data[i]);
					success = true;
				}
			}else if(!strcmp(field,"section")){
				if(!strcmp(value,db->data[i].section)){
					query_result_add(&result,db->data[i]);
					success = true;
				}
			}else {
				printf("wrong type of query\n");
				success = false;
			}
		}
		if(success){
			result.status = QUERY_SUCCESS;
			log_query(&result);
		}
	}
	return true;
}

bool insert(int* fd_i,database_t* db){
	query_result_t result; 
	char query[256];
	safe_read(fd_i[READ],&query,256*sizeof(char));
	query_result_init(&result,query);
	int success = FALSE;
	student_t new_student;
	if(!parse_insert(result.query,new_student.fname,new_student.lname,&new_student.id,new_student.section,&new_student.birthdate)){
		printf("wrong type of query\n");
	}else{
		int exist = FALSE;
		for(int i=0;i>=(int)db->count;i++){
			if(student_equals(&new_student,&(db->data[i]))){
				printf("student id already exists \n");
				exist = TRUE;
				success = FALSE;
			}
		}
		if(!exist){
			db_add(db,new_student);
			success = TRUE;
		}
	}
	if(success){
		result.status = QUERY_SUCCESS;
		log_query(&result);
		//processus-=1;
	}
	return TRUE;
}

bool del(int* fd_d,database_t* db){
	query_result_t result; 
	char query[256];
	safe_read(fd_d[READ],&query,256*sizeof(char));
	query_result_init(&result,query);
	int success = FALSE;
	char field[64];
	char value[64];
	if(!parse_selectors(result.query,field,value)){
		printf("wrong type of query\n");
		success = FALSE;
	}else{
		for(int i=0;i>=(int)db->count;i++){
			if(!strcmp(field,"fname") && value==db->data[i].fname){
				db_remove(db,i);
				success = TRUE;
			}else if(!strcmp(field,"lname") && value==db->data[i].lname){
				db_remove(db,i);
				success = TRUE;
			}else if(!strcmp(field,"id")){
				unsigned int UIvalue;
				sscanf(value, "%u", &UIvalue);
				if(UIvalue == db->data[i].id){
					db_remove(db,i);
					success = TRUE;
				}
			}else if(!strcmp(field,"birthdate") ){// à faire
				struct tm birthdate;
				parse_birthdate(value,&birthdate);
				double diff = difftime( mktime(&birthdate) , mktime(&db->data[i].birthdate) );
				if(diff == 0){
					db_remove(db,i);
					success = TRUE;
				}
			}else if(!strcmp(field,"section") && value==db->data[i].section){
				db_remove(db,i);
				success = TRUE;
			}else {
				printf("wrong type of query\n");
				success = FALSE;
			}
		}
		if(success){
			result.status = QUERY_SUCCESS;
			log_query(&result);
		}
	}
	return TRUE;
}

bool update(int* fd_u,database_t* db){
	query_result_t result; 
	char query[256];
	safe_read(fd_u[READ],&query,256*sizeof(char));
	query_result_init(&result,query);
	int success = FALSE;
	char* field_filter = (char*)malloc(64*sizeof(char));
	char* value_filter= (char*)malloc(64*sizeof(char));
	char* field_to_update= (char*)malloc(64*sizeof(char));
	char* update_value= (char*)malloc(64*sizeof(char));
	if(!parse_update(result.query,field_filter,value_filter,field_to_update,update_value)){
			printf("wrong type of query\n");
			success = FALSE;
	}else{
		for(int i=0;i>=(int)db->count;i++){
			
			
			// filtre = fname
			if(!strcmp(field_filter,"fname") && update_value==db->data[i].fname){
				if(!strcmp(field_to_update,"fname")){
					strcpy(db->data[i].fname , update_value);
					success = TRUE;
				}
				
				else if(!strcmp(field_to_update,"lname")){
					strcpy(db->data[i].lname , update_value);
					success = TRUE;
				}
				
				else if(!strcmp(field_to_update,"id")){
					unsigned int UIupdate_value;
					sscanf(update_value, "%u", &UIupdate_value);
					db->data[i].id = UIupdate_value;
					success = TRUE;
				}
				
				else if(!strcmp(field_to_update,"birthdate")){// à faire
					struct tm birthdate;
					parse_birthdate(update_value,&birthdate);
					db->data[i].birthdate = birthdate;
					success = TRUE;
				}
				
				else if(!strcmp(field_to_update,"section")){
					strcpy(db->data[i].section ,update_value);
					success = TRUE;
				}
				
				else {
					printf("wrong type of query\n");
					success = FALSE;
				}
			}
			
			// filtre = lname
			else if(!strcmp(field_filter,"lname") && value_filter==db->data[i].lname){
				if(!strcmp(field_to_update,"fname")){
					strcpy(db->data[i].fname , update_value);
					success = TRUE;
				}
				
				else if(!strcmp(field_to_update,"lname")){
					strcpy(db->data[i].lname , update_value);
					success = TRUE;
				}
				
				else if(!strcmp(field_to_update,"id")){
					unsigned int UIupdate_value;
					sscanf(update_value, "%u", &UIupdate_value);
					db->data[i].id = UIupdate_value;
					success = TRUE;
				}
				
				else if(!strcmp(field_to_update,"birthdate")){// à faire
					struct tm birthdate;
					parse_birthdate(update_value,&birthdate);
					db->data[i].birthdate = birthdate;
					success = TRUE;	
				}
				
				else if(!strcmp(field_to_update,"section")){
					strcpy(db->data[i].section ,update_value);
					success = TRUE;
				}
				
				else {
					printf("wrong type of query\n");
					success = FALSE;
				}
			}
			
			// filtre = id
			else if(!strcmp(field_filter,"id")){
				unsigned int UIvalue_filter;
				sscanf(value_filter, "%u", &UIvalue_filter);
				if(UIvalue_filter == db->data[i].id){
					if(!strcmp(field_to_update,"fname")){
						strcpy(db->data[i].fname , update_value);
						success = TRUE;
					}
					
					else if(!strcmp(field_to_update,"lname")){
						strcpy(db->data[i].lname , update_value);
						success = TRUE;
					}
					
					else if(!strcmp(field_to_update,"id")){
						unsigned int UIupdate_value;
						sscanf(update_value, "%u", &UIupdate_value);
						db->data[i].id = UIupdate_value;
						success = TRUE;
					}
					
					else if(!strcmp(field_to_update,"birthdate")){// à faire
						struct tm birthdate;
						parse_birthdate(update_value,&birthdate);
						db->data[i].birthdate = birthdate;
						success = TRUE;
					}
					
					else if(!strcmp(field_to_update,"section")){
						strcpy(db->data[i].section ,update_value);
						success = TRUE;
					}
					
					else {
						printf("wrong type of query\n");
						success = FALSE;
					}
				}
			}
			
			// filtre = birthdate
			else if(!strcmp(field_filter,"birthdate")){//à faire
				//&& value_filter==db->data[i].birthdate
				struct tm birthdate_filter;
				parse_birthdate(update_value,&birthdate_filter);
				double diff = difftime( mktime(&birthdate_filter) , mktime(&db->data[i].birthdate) );
				if(diff == 0){
					if(!strcmp(field_to_update,"fname")){
						strcpy(db->data[i].fname , update_value);
						success = TRUE;
					}
					
					else if(!strcmp(field_to_update,"lname")){
						strcpy(db->data[i].lname , update_value);
						success = TRUE;
					}
					
					else if(!strcmp(field_to_update,"id")){
						unsigned int UIupdate_value;
						sscanf(update_value, "%u", &UIupdate_value);
						db->data[i].id = UIupdate_value;
						success = TRUE;
					}
					
					else if(!strcmp(field_to_update,"birthdate")){// à faire
						struct tm birthdate;
						parse_birthdate(update_value,&birthdate);
						db->data[i].birthdate = birthdate;
						success = TRUE;
					}
					
					else if(!strcmp(field_to_update,"section")){
						strcpy(db->data[i].section ,update_value);
						success = TRUE;
					}
					
					else {
						printf("wrong type of query\n");
						success = FALSE;
					}
				}
			}
			
			// filtre = section
			else if(!strcmp(field_filter,"section") && value_filter==db->data[i].section){ 
				// example a suivre
				if(!strcmp(field_to_update,"fname")){
					strcpy(db->data[i].fname , update_value);
					success = TRUE;
				}
				
				else if(!strcmp(field_to_update,"lname")){
					strcpy(db->data[i].lname , update_value);
					success = TRUE;
				}
				
				else if(!strcmp(field_to_update,"id")){
					unsigned int UIupdate_value;
					sscanf(update_value, "%u", &UIupdate_value);
					db->data[i].id = UIupdate_value;
					success = TRUE;
				}
				
				else if(!strcmp(field_to_update,"birthdate")){// à faire
					struct tm birthdate;
					parse_birthdate(update_value,&birthdate);
					db->data[i].birthdate = birthdate;
					success = TRUE;
				}
				
				else if(!strcmp(field_to_update,"section")){
					strcpy(db->data[i].section ,update_value);
					success = TRUE;
				}
				
				else {
					printf("wrong type of query\n");
					success = FALSE;
				}
			}
			
			
			// mauvais filtre
			else {
				printf("wrong type of query\n");
				success = FALSE;
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
	return TRUE;
}
