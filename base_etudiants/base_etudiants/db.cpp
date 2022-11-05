#include "db.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/mman.h>
#include "student.h"
#define DATASIZE 100000

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
	db->data = (student_t*)create_shared_memory(DATASIZE * sizeof(student_t));
	db->lsize = 0;
	db->psize = 0;
}

void db_add(database_t *db, student_t student) {
  db->data[db->lsize] = student;
  db->lsize = db->lsize + 1;
  db->psize = db->psize + sizeof(student_t);
}

void db_remove(database_t *db, int i){
	for(int j=i+1; j<=(int)db->lsize; j++){
		db->data[j-1] = db->data[j];
	}
	db->lsize = db->lsize -1;
}
