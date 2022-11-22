#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Error handling function
void Error(char *msg) {
	fprintf(stderr,"Error:%s\n", msg); 
	exit(1);
	return;
}

int main(int argc, char *argv[]) {
	// Meed to check input failure //
	int i;
	float res=0;
	char *ptr;

	// Make sure args are provided
	if (argc < 2)
		Error("Usage: avgNum <number\s>");

	// Make sure all args are numbers
	for (i=1; i<argc; i++) {
		res += strtol(argv[i], &ptr, 10);
		if (*ptr != '\0')
			Error("There is an invalid input");
	}

	res = res / (argc-1);
	printf("%g\n",res );
	return 0;
}