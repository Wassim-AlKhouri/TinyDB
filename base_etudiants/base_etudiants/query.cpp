#include "query.h"

#include <time.h>

void query_result_init(query_result_t* result, const char* query) {
  struct timespec now;
  clock_gettime(CLOCK_REALTIME, &now);
  result->start_ns = now.tv_nsec + 1e9 * now.tv_sec;
  result->status = QUERY_SUCCESS;
  *result->lsize = 0;
  *result->psize = 2*sizeof(long) + 2*sizeof(size_t) + 256*sizeof(char) + sizeof(QUERY_STATUS);
}

void query_result_add(query_result_t* result, student_t s){
	*result->students[lsize] = student;
	*result->lsize = *result->lsize  + 1;
	*result->psize = *result->psize  + sizeof(student_t);
}
