#include <stdio.h>
#include <string.h>
#include <string> 
#include <sstream>
#include <time.h>
#include <iostream>
#include <signal.h>
bool parse_birthdate(char* date, struct tm* birthdate){
	/*
	int itoken;
	char* datacopy = strdup(date);
	printf("dnas la fonction\n");
	char* token = strtok_r(datacopy,"/",&datacopy);
	printf("%s\n",token);
	if (token == NULL) {
        return false;
    }
	sscanf(token, "%d", &itoken);
    birthdate->tm_mday = itoken;
    token = strtok_r(datacopy,"/",&datacopy);
    printf("%s\n",token);
    if (token == NULL) {
        return false;
    }
	sscanf(token, "%d", &itoken);
	birthdate->tm_mon = itoken;
    token = strtok_r(datacopy,"/",&datacopy);
    printf("%s\n",token);
	if (token == NULL) {
        return false;
    }
	sscanf(token, "%d", &itoken);
    birthdate->tm_year = itoken;
    free(datacopy);
    */
    if (strptime(date, "%d/%m/%Y", birthdate) == NULL) {
        return false;
    } 
    return true;
}

int separator(char* string) {
    int i = 0;
    
    char day[3]; char month[3]; char year[64];
    
    while(string[i] != '/'){
        day[i] = string[i];
        i++;
    }
    day[i] = '\0';
    printf("%s\n", day);
    
    int j = i + 1;
    while(string[j] != '/'){
        month[j-i-1] = string[j];
        j++;
    }
    month[j] = '\0';
    printf("%s\n", month);
    
    int k = j+1;
    while(string[k] != '\0'){
        year[k-j-1] = string[k];
        k++;
    }
    year[k] = '\0';
    printf("%s\n", year);
    
    int int_day; int int_month; int int_year;
    sscanf(day, "%d", &int_day);
    sscanf(month, "%d", &int_month);
    sscanf(year, "%d", &int_year);
    
    struct tm birthdate =  {.tm_mday = 12, .tm_mon = 02, .tm_year = 2000};
    
    if(int_day == birthdate.tm_mday){
        if(int_month == birthdate.tm_mon){
            if(int_year == birthdate.tm_year){
                printf("GOOD\n");
                }
            }
        }
    
    return 0;
}
int SIG = 1;
void INT_handler(int signal){
	//handler pour les signaux de type SIGINT
	if(signal == SIGINT){
		printf("Waiting for requests to terminate...\n");
		SIG=0;
	}else{
		printf("Unknown signal");
	}
}

int main()
{
    char date[64]="31/02/2000";
    char query[64];
    int i = 0;
    struct tm birthdate =  {.tm_mday = 12, .tm_mon = 02, .tm_year = 2000};
    struct tm c_birthdate;
    strptime(date,"%d/%m/%Y",&c_birthdate);
    printf("%d %d %d \n",c_birthdate.tm_mday,c_birthdate.tm_mon,c_birthdate.tm_year);
    printf("%d %d %d \n",birthdate.tm_mday,birthdate.tm_mon,birthdate.tm_year);
    double diff = difftime( mktime(&birthdate) , mktime(&c_birthdate) );
    //signal(SIGINT,&INT_handler);
    //separator(date);
    //fgets(query,64,stdin);
	//while(query[i] != ' '){i++;}
	//strncpy(date, query, i*sizeof(char));
	if(diff == 0){
		printf("C BON\n");
	}
    return 0;
}
/*
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
	scanf("%s", champ);
	struct tm birthdate;
	parse_birthdate(champ,&birthdate);
	struct tm b = {.tm_mday = 1, .tm_mon = 5, .tm_year = 2000};
	
	if(b.tm_mday == birthdate.tm_mday && b.tm_mon == birthdate.tm_mon && b.tm_year == birthdate.tm_year){
		printf("c bon\n");
	}
	
	time_t tb = mktime(&b);
	time_t tbirthdate = mktime(&birthdate);
	if( difftime(tbirthdate,tb) == 0){
		printf("c bon\n");
	} 
}
*/
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
