#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]){
	int i, strl;
	char c;
	
	// Check an argument is provided
	if (argc != 2){
		printf("Usage: catByAt <SPACELESS_STRING>");
		exit(1);
	}

	// Getting the length of the string
	strl = strlen(argv[1]);

	// Iterating over the string
	for (i=0; i<=strl; i++){
		c = argv[1][i];	
		if (argv[1][i] == '@')
			printf("\n");
		else{
			// Checking if curr char is lower case
			if (c >= 'a' && c <= 'z')
				printf("%c", (c-32));
			else
				printf("%c", c);
		}	
	}
	printf("\n");
	return 0;
}
