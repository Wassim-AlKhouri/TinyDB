#include "student.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

void student_to_str(char* buffer, student_t* s) {
  // Your code here
}

int student_equals(student_t* s1, student_t* s2) {
	if(s1->id==s2->id){return 0;}
	return 1;
}
