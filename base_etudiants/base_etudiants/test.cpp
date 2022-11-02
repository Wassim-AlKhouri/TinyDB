#include <stdio.h>
#include <string.h>
#include <string> 
#include <time.h>
int main(){
	char champ[64]="10";
	char valeur[30]="sto";
	int i= 10;
	struct tm start = {.tm_year=2022-1900, .tm_mday=1};
    mktime(&start);
    printf("%s", asctime(&start));
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
