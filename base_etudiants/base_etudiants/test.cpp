#include <stdio.h>
#include <string.h>
#include <string> 
#include <sstream>
#include <time.h>
#include <iostream>
bool parse_birthdate(char* date, struct tm* birthdate){
	printf("avant datecopy\n");
	char *datecopy;
	strcpy(datecopy,date);
	int itoken;
	printf("dnas la fonction\n");
	char* token = strtok_r(datecopy,"/",&datecopy);
	printf("%s\n",token);
	if (token == NULL) {
        return false;
    }
	sscanf(token, "%d", &itoken);
    birthdate->tm_mday = itoken;
    token = strtok_r(datecopy,"/",&datecopy);
    printf("%s\n",token);
    if (token == NULL) {
        return false;
    }
	sscanf(token, "%d", &itoken);
	birthdate->tm_mon = itoken;
    token = strtok_r(datecopy,"/",&datecopy);
    printf("%s\n",token);
	if (token == NULL) {
        return false;
    }
	sscanf(token, "%d", &itoken);
    birthdate->tm_year = itoken;
    return true;
}
int main(){
	char* champ;
	char valeur[30]="sto";
	//unsigned b = 10258;
	//stringstream strb;
	//strb << champ;
	//strtoul(vIn,NULL,10);
	//unsigned int strb = (unsigned int *)strtoull(champ, NULL, 0
	/*
	unsigned int strb;
	sscanf(champ, "%d", &strb);
	if(b == strb){
		printf("c bon\n");
	}
	*/
	scanf("%s", champ);
	struct tm birthdate;
	parse_birthdate(champ,&birthdate);
	struct tm b = {.tm_mday = 1, .tm_mon = 5, .tm_year = 2000};
	/*
	if(b.tm_mday == birthdate.tm_mday && b.tm_mon == birthdate.tm_mon && b.tm_year == birthdate.tm_year){
		printf("c bon\n");
	}
	*/
	time_t tb = mktime(&b);
	time_t tbirthdate = mktime(&birthdate);
	if( difftime(tbirthdate,tb) == 0){
		printf("c bon\n");
	} 
}
/**
 * 	char champ[64];
	char valeur[64];
	int i = 0;
	while(result.query[i] != ' '){i++;}
	int j = i;
	while(result.query[j] != '='){j++;}
	for(int x=i+1;x<j;x++){
		champ[x-i-1]=result.query[x];
	}
	int k = j;
	while(result.query[k] != '\0'){k++;}
	for(int x=j+1;x<k;x++){
		valeur[x-j-1]=result.query[x];
	}
	if(champ == "id"){
		for(int s =0;s<=result.lsize;s++){
			if(result.students.id == valeur){query_result_add(result,result.students[s]);}
		}
	}else if(champ == "fname"){
		for(int s =0;s<=result.lsize;s++){
			if(result.students.fname == valeur){query_result_add(result,result.students[s]);}
		}
	}else if(champ == "lname"){
		for(int s =0;s<=result.lsize;s++){
			if(result.students.lname == valeur){query_result_add(result,result.students[s]);}
		}
	}else if(champ == "section"){
		for(int s =0;s<=result.lsize;s++){
			if(result.students.update == valeur){query_result_add(result,result.students[s]);}
		}
	}else if(champ == "birthdate"){
		for(int s =0;s<=result.lsize;s++){
			if(result.students.birthdate == valeur){query_result_add(result,result.students[s]);}
		}
	}else{
		printf("demande mal formée");
	}	
	* 
	* */
