#include <stdio.h>
#include <ctype.h>
#include <string.h>

int main () {
	char first[100];
	char second[100];
	char name[100];
	char str[100];
	scanf("%s", first);
	scanf("%s", second);
	strcpy(str, second);
	for (int i = 0; str[i] != '\0'; i++)
		str[i] -= 32;
	printf("%s\n", str);
	int strcompare = strcmp(second, str);
	printf("%d\n", strcompare);
	strcpy(name, first);
	strcat(name, second);
	printf("%s\n", name);
	int year;
	scanf("%d", &year);
	
}
