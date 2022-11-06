#include "parsing.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <string.h>

#include "student.h"

bool parse_update(char* query, char* field_filter, char* value_filter, char* field_to_update, char* update_value) {
    char* key_val_filter = strtok_r(NULL, " ", &query);  // key=val filter
    if (key_val_filter == NULL) {
        return false;
    }
    if (strtok_r(NULL, " ", &query) == NULL) {  // discard the "set" keyword
        return false;
    }

    char* key_val_update = strtok_r(NULL, " ", &query);  // key=val updated value
    if (key_val_update == NULL) {
        return false;
    }

    if (parse_selectors(key_val_filter, field_filter, value_filter) == 0) {
        return false;
    }
    if (parse_selectors(key_val_update, field_to_update, update_value) == 0) {
        return false;
    }
    return true;
}

bool parse_insert(char* query, char* fname, char* lname, unsigned* id, char* section, struct tm* birthdate) {
    char* token = strtok_r(NULL, " ", &query);
    if (token == NULL) {
        return false;
    }
    strcpy(fname, token);
    token = strtok_r(NULL, " ", &query);
    if (token == NULL) {
        return false;
    }
    strcpy(lname, token);
    token = strtok_r(NULL, " ", &query);
    if (token == NULL) {
        return false;
    }
    *id = (unsigned)atol(token);
    token = strtok_r(NULL, " ", &query);
    if (token == NULL) {
        return false;
    }
    strcpy(section, token);
    token = strtok_r(NULL, " ", &query);
    if (token == NULL) {
        return false;
    }
    if (strptime(token, "%d/%m/%Y", birthdate) == NULL) {
        return false;
    }
    return true;
}

bool parse_selectors(char* query, char* field, char* value) {
    char* token = strtok_r(NULL, "=", &query);
    if (token == NULL) {
        return false;
    }
    strcpy(field, token);
    token = strtok_r(NULL, "=", &query);
    if (token == NULL) {
        return false;
    }
    strcpy(value, token);
    return true;
}

bool parse_birthdate(char* C_birthdate, struct tm* birthdate){
    int i = 0;
    char day[3]; char month[3]; char year[64];
    while(C_birthdate[i] != '/'){
        day[i] = C_birthdate[i];
        i++;
    }
    day[i] = '\0';
    int j = i + 1;
    while(C_birthdate[j] != '/'){
        month[j-i-1] = C_birthdate[j];
        j++;
    }
    month[j] = '\0';
    int k = j+1;
    while(C_birthdate[k] != '\0'){
        year[k-j-1] = C_birthdate[k];
        k++;
    }
    year[k] = '\0';
    int int_day; int int_month; int int_year;
    sscanf(day, "%d", &int_day);
    sscanf(month, "%d", &int_month);
    sscanf(year, "%d", &int_year);
	birthdate->tm_mday = int_day;
	birthdate->tm_mon = int_month;
	birthdate->tm_year = int_year;
    return true;
}
