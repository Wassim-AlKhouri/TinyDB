#include "query.h"

#include <cstdlib>
#include <time.h>

void query_result_init(query_result_t* result, const char* query) {
  struct timespec now;
  clock_gettime(CLOCK_REALTIME, &now);
  result->start_ns = now.tv_nsec + 1e9 * now.tv_sec;
  result->status = QUERY_FAILURE;
  result->students = (student_t*) malloc(1000 * sizeof(student_t));
  result->lsize = 0;
  result->count = 0;
  result->psize = 1000 * sizeof(student_t);
  strcpy(result->query,query);
}

void query_result_add(query_result_t* result, student_t s){
	if( (result->lsize + sizeof(student_t)) >= result->psize ){
		query_result_resize(result);
	}
	result->students[result->count] = s;
	result->lsize = result->lsize  + sizeof(student_t);
	result->count = result->count + 1;
}

void query_result_resize(query_result_t* result){
	//on alloue une mémoire qui est plus grand et on copie tous les 
	//étudiants dedans. Puis on libère la mémoire du 1er malloc et on redirige le pointeur
	//vers le nouveau.
	student_t *tmp = (student_t*) malloc( result->psize + (1000 * sizeof(student_t)) );
	for(int i=0; i<=(int)result->count; i++){
		tmp[i] = result->students[i];
	}
	free(result->students);
	result->students = tmp;
	result->psize = result->psize + (1000 * sizeof(student_t));	
}
