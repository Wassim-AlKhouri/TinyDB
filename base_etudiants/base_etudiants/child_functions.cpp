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
	query_result_t result;
	char query[256];
	safe_read(fd_s[READ],&query,256*sizeof(char));
	query_result_init(&result,query);
	bool success = false;
	char field[64];
	char value[64];
	if(!parse_selectors(result.query + 7,field,value)){
		printf("wrong type of query\n");
		success = false;
	}else{
		printf("%s ICI\n", value);
		printf("%s TOT\n", field);
		for(int i=0;i<db->count;i++){
			if(!strcmp(field,"fname")){
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
			}else if(!strcmp(field,"birthdate")){
				printf("BIRTHDATE ICI\n");
				struct tm birthdate;
				strptime(value,"%d/%m/%Y",&birthdate);
				if(birthdate.tm_mday == db->data[i].birthdate.tm_mday && birthdate.tm_mon == db->data[i].birthdate.tm_mon && birthdate.tm_year == db->data[i].birthdate.tm_year){
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
	int success = false;
	student_t new_student;
	if(!parse_insert(result.query + 7,new_student.fname,new_student.lname,&new_student.id,new_student.section,&new_student.birthdate)){
		printf("wrong type of query\n");
	}else{
		int exist = false;
		for(int i=0;i<db->count;i++){
			if(student_equals(&new_student,&(db->data[i]))){
				printf("student id already exists \n");
				exist = true;
				success = false;
			}
		}
		if(!exist){
			db_add(db,new_student);
			success = true;
		}
	}
	if(success){
		query_result_add(&result,new_student);
		result.status = QUERY_SUCCESS;
		log_query(&result);
	}
	return true;
}

bool del(int* fd_d,database_t* db){
	query_result_t result; 
	char query[256];
	safe_read(fd_d[READ],&query,256*sizeof(char));
	query_result_init(&result,query);
	int success = false;
	char field[64];
	char value[64];
	if(!parse_selectors(result.query + 7,field,value)){
		printf("wrong type of query\n");
		success = false;
	}else{
		for(int i=0;i<db->count;i++){
			if(!strcmp(field,"fname")){
				if(!strcmp(value,db->data[i].fname)){
					query_result_add(&result,db->data[i]);
					//db_remove(db,i);
					success = true;
				}
			}else if(!strcmp(field,"lname")){
				if(!strcmp(value,db->data[i].lname)){
					query_result_add(&result,db->data[i]);
					//db_remove(db,i);
					success = true;
				}
			}else if(!strcmp(field,"id")){
				unsigned int UIvalue;
				sscanf(value, "%u", &UIvalue);
				if(UIvalue == db->data[i].id){
					query_result_add(&result,db->data[i]);
					//db_remove(db,i);
					success = true;
				}
			}else if(!strcmp(field,"birthdate") ){
				struct tm birthdate;
				strptime(value,"%d/%m/%Y",&birthdate);
				if(birthdate.tm_mday == db->data[i].birthdate.tm_mday && birthdate.tm_mon == db->data[i].birthdate.tm_mon && birthdate.tm_year == db->data[i].birthdate.tm_year){
					query_result_add(&result,db->data[i]);
					//db_remove(db,i);
					success = true;
				}
			}else if(!strcmp(field,"section")){
				if(!strcmp(value,db->data[i].section)){
					query_result_add(&result,db->data[i]);
					//db_remove(db,i);
					success = true;
				}
			}else {
				printf("wrong type of query\n");
				success = false;
			}
		}
		if(success){
			for(int i=0;i<result.count;i++){
				db_remove(db,result.students[i]);
			}
			result.status = QUERY_SUCCESS;
			log_query(&result);
			for(int i=0;i<db->count;i++){
				printf("%s \n",db->data[i].fname);
			}
		}
	}
	return true;
}

bool update(int* fd_u,database_t* db){
	query_result_t result; 
	char query[256];
	safe_read(fd_u[READ],&query,256*sizeof(char));
	query_result_init(&result,query);
	int success = false;
	char* field_filter = (char*)malloc(64*sizeof(char));
	char* value_filter= (char*)malloc(64*sizeof(char));
	char* field_to_update= (char*)malloc(64*sizeof(char));
	char* update_value= (char*)malloc(64*sizeof(char));
	if(!parse_update(result.query + 7,field_filter,value_filter,field_to_update,update_value)){
			printf("wrong type of query\n");
			printf("PARSE ER\n");
			success = false;
	}else{
		for(int i=0;i<db->count;i++){
			
		
			// filtre = fname
			if(!strcmp(field_filter,"fname")){
				if(!strcmp(value_filter,db->data[i].fname)){
					if(!strcmp(field_to_update,"fname")){
						strcpy(db->data[i].fname , update_value);
						success = true;
						query_result_add(&result,db->data[i]);
					}
					
					else if(!strcmp(field_to_update,"lname")){
						strcpy(db->data[i].lname , update_value);
						success = true;
						query_result_add(&result,db->data[i]);
					}
					
					else if(!strcmp(field_to_update,"id")){
						unsigned int UIupdate_value;
						sscanf(update_value, "%u", &UIupdate_value);
						db->data[i].id = UIupdate_value;
						success = true;
						query_result_add(&result,db->data[i]);
					}
					
					else if(!strcmp(field_to_update,"birthdate")){// à faire
						struct tm birthdate;
						strptime(update_value,"%d/%m/%Y",&birthdate);
						db->data[i].birthdate = birthdate;
						success = true;
						query_result_add(&result,db->data[i]);
					}
					
					else if(!strcmp(field_to_update,"section")){
						strcpy(db->data[i].section ,update_value);
						success = true;
						query_result_add(&result,db->data[i]);
					}
					
					else {
						printf("wrong type of query\n");
						success = false;
					}
				}
			}
			
			// filtre = lname
			else if(!strcmp(field_filter,"lname")){
				if(!strcmp(value_filter,db->data[i].lname)){
					if(!strcmp(field_to_update,"fname")){
						strcpy(db->data[i].fname , update_value);
						success = true;
						query_result_add(&result,db->data[i]);
					}
					
					else if(!strcmp(field_to_update,"lname")){
						strcpy(db->data[i].lname , update_value);
						success = true;
						query_result_add(&result,db->data[i]);
					}
					
					else if(!strcmp(field_to_update,"id")){
						unsigned int UIupdate_value;
						sscanf(update_value, "%u", &UIupdate_value);
						db->data[i].id = UIupdate_value;
						success = true;
						query_result_add(&result,db->data[i]);
					}
					
					else if(!strcmp(field_to_update,"birthdate")){// à faire
						struct tm birthdate;
						strptime(update_value,"%d/%m/%Y",&birthdate);
						db->data[i].birthdate = birthdate;
						success = true;
						query_result_add(&result,db->data[i]);	
					}
					
					else if(!strcmp(field_to_update,"section")){
						strcpy(db->data[i].section ,update_value);
						success = true;
						query_result_add(&result,db->data[i]);
					}
					
					else {
						printf("wrong type of query\n");
						success = false;
					}
				}
			}
			
			// filtre = id
			else if(!strcmp(field_filter,"id")){
				unsigned int UIvalue_filter;
				sscanf(value_filter, "%u", &UIvalue_filter);
				if(UIvalue_filter == db->data[i].id){
					if(!strcmp(field_to_update,"fname")){
						strcpy(db->data[i].fname , update_value);
						success = true;
						query_result_add(&result,db->data[i]);
					}
					
					else if(!strcmp(field_to_update,"lname")){
						strcpy(db->data[i].lname , update_value);
						success = true;
						query_result_add(&result,db->data[i]);
					}
					
					else if(!strcmp(field_to_update,"id")){
						unsigned int UIupdate_value;
						sscanf(update_value, "%u", &UIupdate_value);
						db->data[i].id = UIupdate_value;
						success = true;
						query_result_add(&result,db->data[i]);
					}
					
					else if(!strcmp(field_to_update,"birthdate")){// à faire
						struct tm birthdate;
						strptime(update_value,"%d/%m/%Y",&birthdate);
						db->data[i].birthdate = birthdate;
						success = true;
						query_result_add(&result,db->data[i]);
					}
					
					else if(!strcmp(field_to_update,"section")){
						strcpy(db->data[i].section ,update_value);
						success = true;
						query_result_add(&result,db->data[i]);
					}
					
					else {
						printf("wrong type of query\n");
						success = false;
					}
				}
			}
			
			// filtre = birthdate
			else if(!strcmp(field_filter,"birthdate")){//à faire
				//&& value_filter==db->data[i].birthdate
				struct tm birthdate_filter;
				strptime(value_filter,"%d/%m/%Y",&birthdate_filter);
				if(birthdate_filter.tm_mday == db->data[i].birthdate.tm_mday && birthdate_filter.tm_mon == db->data[i].birthdate.tm_mon && birthdate_filter.tm_year == db->data[i].birthdate.tm_year){
					if(!strcmp(field_to_update,"fname")){
						strcpy(db->data[i].fname , update_value);
						success = true;
						query_result_add(&result,db->data[i]);
					}
					
					else if(!strcmp(field_to_update,"lname")){
						strcpy(db->data[i].lname , update_value);
						success = true;
						query_result_add(&result,db->data[i]);
					}
					
					else if(!strcmp(field_to_update,"id")){
						unsigned int UIupdate_value;
						sscanf(update_value, "%u", &UIupdate_value);
						db->data[i].id = UIupdate_value;
						success = true;
						query_result_add(&result,db->data[i]);
					}
					
					else if(!strcmp(field_to_update,"birthdate")){// à faire
						struct tm birthdate;
						strptime(update_value,"%d/%m/%Y",&birthdate);
						db->data[i].birthdate = birthdate;
						success = true;
						query_result_add(&result,db->data[i]);
					}
					
					else if(!strcmp(field_to_update,"section")){
						strcpy(db->data[i].section ,update_value);
						success = true;
						query_result_add(&result,db->data[i]);
					}
					
					else {
						printf("wrong type of query\n");
						success = false;
					}
				}
			}
			
			// filtre = section
			else if(!strcmp(field_filter,"section")){ 
				if(!strcmp(value_filter,db->data[i].section)){
					if(!strcmp(field_to_update,"fname")){
						strcpy(db->data[i].fname , update_value);
						success = true;
						query_result_add(&result,db->data[i]);
					}
					
					else if(!strcmp(field_to_update,"lname")){
						strcpy(db->data[i].lname , update_value);
						success = true;
						query_result_add(&result,db->data[i]);
					}
					
					else if(!strcmp(field_to_update,"id")){
						unsigned int UIupdate_value;
						sscanf(update_value, "%u", &UIupdate_value);
						db->data[i].id = UIupdate_value;
						success = true;
						query_result_add(&result,db->data[i]);
					}
					
					else if(!strcmp(field_to_update,"birthdate")){// à faire
						struct tm birthdate;
						strptime(update_value,"%d/%m/%Y",&birthdate);
						db->data[i].birthdate = birthdate;
						success = true;
						query_result_add(&result,db->data[i]);
					}
					
					else if(!strcmp(field_to_update,"section")){
						strcpy(db->data[i].section ,update_value);
						success = true;
						query_result_add(&result,db->data[i]);
					}
					
					else {
						printf("wrong type of query\n");
						success = false;
					}
				}
			}
			
			
			// mauvais filtre
			else {
				printf("wrong type of query\n");
				success = false;
			}
		}
		
		if(success){
			for(int i=0;i<db->count;i++){
				printf("%s \n",db->data[i].fname);
			}
			result.status = QUERY_SUCCESS;
			log_query(&result);
			//processus-=1;
		}
	}
	free(field_filter);
	free(value_filter);
	free(field_to_update);
	free(update_value);
	return true;
}
