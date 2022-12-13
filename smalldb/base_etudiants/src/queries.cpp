#include "queries.hpp"
using namespace std;
//#include "io.hpp"

// execute_* ///////////////////////////////////////////////////////////////////

void execute_select(int fout, database_t* const db, const char* const field,
                    const char* const value) {
  std::function<bool(const student_t&)> predicate = get_filter(field, value);
  if (!predicate) {
    query_fail_bad_filter(fout, field, value);
    return;
  }
  int i=0;
  for (const student_t& s : db->data) {
    if (predicate(s)) {
      char* buffer = new char[1024];
      i++;
      student_to_str(buffer,&s,1024),
      write(fout,buffer,strlen(buffer));
      write(fout,"\n",sizeof(char));
      delete(buffer);
    }
  }
  char end[50] = "(X) student(s) selected \0";
  end[1] = i +  '0';
  write(fout,&end,(strlen(end)+1)*sizeof(char));
}

void execute_update(int fout, database_t* const db, const char* const ffield, const char* const fvalue, const char* const efield, const char* const evalue) {
  std::function<bool(const student_t&)> predicate = get_filter(ffield, fvalue);
  printf("ok \n");
  if (!predicate) {
    query_fail_bad_filter(fout, ffield, fvalue);
    return;
  }
  std::function<void(student_t&)> updater = get_updater(efield, evalue);
  if (!updater) {
    query_fail_bad_update(fout, efield, evalue);
    return;
  }
  int i=0;
  for (student_t& s : db->data) {
    if (predicate(s)) {
      updater(s);
      i++;
    }
  }
  char end[50] = "(X) student(s) updated \0";
  end[1] = i +  '0';
  write(fout,&end,(strlen(end)+1)*sizeof(char));
}

void execute_insert(int fout, database_t* const db, const char* const fname,
                    const char* const lname, const unsigned id, const char* const section,
                    const tm birthdate) {
  db->data.emplace_back();
  student_t *s = &db->data.back();
  s->id = id;
  snprintf(s->fname, sizeof(s->fname), "%s", fname);
  snprintf(s->lname, sizeof(s->lname), "%s", lname);
  snprintf(s->section, sizeof(s->section), "%s", section);
  s->birthdate = birthdate;
  char buffer[1024];
  student_to_str(buffer,s,1024);
  buffer[strlen(buffer)]='\0';
  write(fout,buffer,(strlen(buffer)+1)*sizeof(char));
  //write(fout,"\n \0",(strlen(buffer)+1)*sizeof(char));
}

void execute_delete(int fout, database_t* const db, const char* const field,
                    const char* const value) {
  std::function<bool(const student_t&)> predicate = get_filter(field, value);
  if (!predicate) {
    query_fail_bad_filter(fout, field, value);
    return;
  }
  int length = db->data.size();
  auto new_end = remove_if(db->data.begin(), db->data.end(), predicate);
  db->data.erase(new_end, db->data.end());
  int new_length = db->data.size();
  int deleted_students = length - new_length;
  char end[50] = "(X) student(s) deleted \0";
  end[1] = deleted_students +  '0';
  write(fout,&end,(strlen(end)+1)*sizeof(char));
}

// parse_and_execute_* ////////////////////////////////////////////////////////

void parse_and_execute_select(int fout, database_t* db, const char* const query) {
  char ffield[32], fvalue[64];  // filter data
  int  counter;
  if (sscanf(query, "select %31[^=]=%63s%n", ffield, fvalue, &counter) != 2) {
    query_fail_bad_format(fout, "select");
  } else if (static_cast<unsigned>(counter) < strlen(query) && false) {
    printf("%li\n",strlen(query));
    query_fail_too_long(fout, "select");
  } else {
    execute_select(fout, db, ffield, fvalue);
  }
}

void parse_and_execute_update(int fout, database_t* db, const char* const query) {
  char ffield[32], fvalue[64];  // filter data
  char efield[32], evalue[64];  // edit data
  int counter;
  if (sscanf(query, "update %31[^=]=%63s set %31[^=]=%63s%n", ffield, fvalue, efield, evalue,
             &counter) != 4) {
    query_fail_bad_format(fout, "update");
  } else if (static_cast<unsigned>(counter) < strlen(query) && false) {
    query_fail_too_long(fout, "update");
  } else {
    execute_update(fout, db, ffield, fvalue, efield, evalue);
  }
}

void parse_and_execute_insert(int fout, database_t* db, const char* const query) {
  char      fname[64], lname[64], section[64], date[11];
  unsigned  id;
  tm        birthdate;
  int       counter;
  if (sscanf(query, "insert %63s %63s %u %63s %10s%n", fname, lname, &id, section, date, &counter) != 5 || strptime(date, "%d/%m/%Y", &birthdate) == NULL) {
    query_fail_bad_format(fout, "insert");
  } else if (static_cast<unsigned>(counter) < strlen(query) && false) {
    query_fail_too_long(fout, "insert");
  } else {
    execute_insert(fout, db, fname, lname, id, section, birthdate);
  }
}

void parse_and_execute_delete(int fout, database_t* db, const char* const query) {
  char ffield[32], fvalue[64]; // filter data
  int counter;
  if (sscanf(query, "delete %31[^=]=%63s%n", ffield, fvalue, &counter) != 2) {
    query_fail_bad_format(fout, "delete");
  } else if (static_cast<unsigned>(counter) < strlen(query) && false) {
    printf("%s\n", query);
    query_fail_too_long(fout, "delete");
  } else {
    execute_delete(fout, db, ffield, fvalue);
  }
}

void parse_and_execute(int fout, database_t* db, const char* const query) {
  if (strncmp("select", query, sizeof("select")-1) == 0) {
    parse_and_execute_select(fout, db, query);
  } else if (strncmp("update", query, sizeof("update")-1) == 0) {
    parse_and_execute_update(fout, db, query);
  } else if (strncmp("insert", query, sizeof("insert")-1) == 0) {
    parse_and_execute_insert(fout, db, query);
  } else if (strncmp("delete", query, sizeof("delete")-1) == 0) {
    parse_and_execute_delete(fout, db, query);
  } else {
    printf("%s\n", query);
    query_fail_bad_query_type(fout);
  }
}

int parse(const char* const query) {
  int i;
  if (strncmp("select", query, sizeof("select")-1) == 0) {
    i = 1;
  } else if (strncmp("update", query, sizeof("update")-1) == 0) {
    i = 0;
  } else if (strncmp("insert", query, sizeof("insert")-1) == 0) {
    i = 0;
  } else if (strncmp("delete", query, sizeof("delete")-1) == 0) {
    i = 0;
  } else {
    i = -1;
  }
  return i;
}

// query_fail_* ///////////////////////////////////////////////////////////////

void query_fail_bad_query_type(int fout) {
printf("Bad Query\n");
}

void query_fail_bad_format(int fout, const char * const query_type) {
printf("Bad Format\n");
}

void query_fail_too_long(int fout, const char * const query_type) {
printf("Too Long,%s\n",query_type);
}

void query_fail_bad_filter(int fout, const char* const field, const char* const filter) {
printf("Bad Filter\n");
}

void query_fail_bad_update(int fout, const char* const field, const char* const filter) {
printf("5\n");
}

