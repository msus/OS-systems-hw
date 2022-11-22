#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#define MAX_PATH 4096 				// as specified at linux/limits.h

int main(int argc, char* argv[]) {
    int count = 0;
    DIR* dir;
    struct dirent* sd;
    char path[MAX_PATH], temp[MAX_PATH];

    if(argc != 2){
		printf("USAGE: ./getOrderNm <RESTURANT_NAME>\n");
		exit(0);
	}

    // Getting pull path to current directory
    getcwd(path, MAX_PATH);
    sprintf(temp, "/%s_Order", argv[1]);
    strcat(path, temp);

    dir = opendir(path);
    if (dir == NULL) {
        printf("Error! Unable to open directory\n");
        exit(1);
    }

    // Counting files
    while ((sd = readdir(dir)) != NULL)
        count++;

    printf("%d Orders\n", count - 2);
    closedir(dir);
    return 0;
}
