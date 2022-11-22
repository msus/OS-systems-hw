#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MAX_RESTURANT_STRINGS 25

int main(int argc, char *argv[]){
    char resturant_file[MAX_RESTURANT_STRINGS];

    if(argc != 2){
		printf("USAGE: ./getMenu <RESTURANT_NAME>\n");
		exit(0);
	}

    // Presenting the file content using the 'cat' executable
    sprintf(resturant_file, "%s.txt", argv[1]);
    if(execlp("cat", "cat", resturant_file, NULL) == -1){
        perror("ERROR (execlp)");
        exit(1);
    }
    return 0;
}