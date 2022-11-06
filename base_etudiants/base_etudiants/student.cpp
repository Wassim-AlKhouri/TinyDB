#include "student.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sstream>

void student_to_str(char* buffer, student_t* s) {
	std::ostringstream tmp;
	tmp << s->fname<< " " << s->lname << " " << s->id << " " << s->section << " " << s->birthdate.tm_mday << "/" << s->birthdate.tm_mon<< "/"  << s->birthdate.tm_year;
	//sscanf(tmp,"%s %s %u %s %d/%d/%d",s->fname,s->lname,&s->id,s->section,&s->birthdate.tm_mday,&s->birthdate.tm_mon,&s->birthdate.tm_year);
	//sscanf(tmp," %u %d/%d/%d",&s->id,&s->birthdate.tm_mday,&s->birthdate.tm_mon,&s->birthdate.tm_year);
	//strcpy(buffer,tmp.str());
	strcpy(buffer,tmp.str().c_str()),
	printf("%s BUFFF\n", buffer);
}

int student_equals(student_t* s1, student_t* s2) {
	if(s1->id==s2->id){return 0;}
	return 1;
}
