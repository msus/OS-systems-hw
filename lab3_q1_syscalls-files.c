#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#define BUFF_SIZE 129 // 128 +1 for '\0'

int main(int argc, char* argv[]){
	int fd1, fd2, rbytes1, rbytes2, i;
	char read1c[BUFF_SIZE], read2c[BUFF_SIZE];
	
	// In case too manny\little argument was provided 
	if(argc != 3){
		printf("USAGE: ./compare <1ST_FILE> <2ND_FILE>\n");
		exit(0);
	}
	// Creating file descriptors for both files
	if((fd1 = open(argv[1], O_RDONLY)) == -1){
		perror("ERROR (fd1-open)");
		printf("Error opening %s\n", argv[1]);
		exit(1);
	}
	if((fd2 = open(argv[2], O_RDONLY)) == -1){
		perror("ERROR (fd2-open)");
		printf("Error opening %s\n", argv[2]);
		exit(1);
	}

	// Reading upto 128 bytes at a time and comparing them
	do{
		if((rbytes1 = read(fd1, read1c, BUFF_SIZE-1)) == -1){
			perror("ERROR (rbytes1)");
			exit(1);
		}
		if((rbytes2 = read(fd2, read2c, BUFF_SIZE-1)) == -1){
			perror("ERROR (rbytes2)");
			exit(1);
		}
		// In case one file is shorter
		if(rbytes1 != rbytes2){
			printf("Different files\n");
			exit(0);
		}
		
		// Insert '\0' so strcmp can compare the buffers
		read1c[rbytes1] = read2c[rbytes2] = '\0';
		if(strcmp(read1c, read2c)){
			printf("Different files\n");
			exit(0);
		}
	}
	while(rbytes1 > 0 && rbytes2 > 0);  

	if(rbytes1 == rbytes2){
		printf("Identical files\n");
	}else{
		printf("Different files\n");
	}
	close(fd1);
    close(fd2);
	return 0;
}