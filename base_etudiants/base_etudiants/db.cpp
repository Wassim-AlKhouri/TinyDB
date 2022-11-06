#include "db.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/mman.h>
#include "student.h"

void db_save(database_t *db, const char *path) {
    FILE *f = fopen(path, "wb");
    if (!f) {
        perror("Could not open the DB file");
        exit(1);
    }
    if (fwrite(db->data, sizeof(student_t), db->lsize, f) < 0) {
        perror("Could not write in the DB file");
        exit(1);
    }
    fclose(f);
}

void db_load(database_t *db, const char *path) {
    FILE *file = fopen(path, "rb");
    if (!file) {
        perror("Could not open the DB file");
        exit(1);
    }
    student_t student;
    while (fread(&student, sizeof(student_t), 1, file)) {
        db_add(db, student);
    }
    fclose(file);
}

void db_init(database_t *db) {
	db->data = (student_t*)create_shared_memory(1000 * sizeof(student_t));
	db->lsize = 0;
	db->count = 0;
	db->psize = 1000*sizeof(student_t);
}

void db_add(database_t *db, student_t student) {
	if( (db->lsize + sizeof(student_t)) >= db->psize ){
		db_resize(db);
	}
	db->data[db->count] = student;
	printf("%s\n", student.fname);
	db->lsize = db->lsize + sizeof(student_t);
	db->count = db->count + 1;
}

void db_remove(database_t *db, int i){
	for(int j=i+1; j<=(int)db->count; j++){
		db->data[j-1] = db->data[j];
	}
	db->lsize = db->lsize - sizeof(student_t);
	db->count = db->count -1;
}

void db_resize(database_t *db){
	student_t *tmp = (student_t*)create_shared_memory( db->psize + (1000 * sizeof(student_t)) );
	for(int i=0; i<=(int)db->count; i++){
		tmp[i] = db->data[i];
	}
	munmap(db->data,db->psize);
	db->data = tmp;
	db->psize = db->psize + (1000 * sizeof(student_t));
}
