#include <stdio.h>
int main(){
	char s[200];
	scanf("%[^\t\n]",s);
	char champ[64];
	char valeur[64];
	int i = 0;
	while(s[i] != ' '){i++;}
	int j = i;
	while(s[j] != '='){j++;}
	for(int x=i+1;x<j;x++){
		champ[x-i-1]=s[x];
	}
	int k = j;
	while(s[k] != '\0'){k++;}
	for(int x=j+1;x<k;x++){
		valeur[x-j-1]=s[x];
	}
	printf("%s %s \n",champ,valeur);
}
