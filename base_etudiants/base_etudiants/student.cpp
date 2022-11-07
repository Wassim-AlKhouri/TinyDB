#include "student.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sstream>

void student_to_str(char* buffer, student_t* s) {
	std::ostringstream tmp;
	tmp << s->fname<< " " << s->lname << " " << s->id << " " << s->section << " " << s->birthdate.tm_mday << "/" << s->birthdate.tm_mon + 1 << "/"  << s->birthdate.tm_year + 1900;
	strcpy(buffer,tmp.str().c_str());
}

int student_equals(student_t* s1, student_t* s2) {
	if(s1->id==s2->id){return true;}
	return false;
}
