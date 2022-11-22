#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#define BUFF_SIZE 128

int main(int argc, char* argv[]){
	int fd_rd, fd_wr, rbytes, wbytes, i;
	char read_buff[BUFF_SIZE];
	
	// In case too manny\little argument was provided 
	if(argc != 3){
		printf("USAGE: ./createCopy <FOLDER> <FILE_NAME>\n");
		exit(0);
	}
	
	// Creating file descriptors for source file
	if((fd_rd = open(argv[2], O_RDONLY)) == -1){
		perror("ERROR (fd_rd-open)");
		printf("Error opening %s\n", argv[2]);
		exit(1);
	}
	
	// Creating folder named as the 1st argument
	if(mkdir(argv[1], 0755) == -1){
		perror("ERROR (mkdir)");
		printf("Error creating folder: %s\n", argv[1]);
		exit(1);
	}
	
	// Changing directory for the next file descriptor 
	if(chdir(argv[1]) == -1){
		perror("ERROR (chdir)");
		printf("Error cd %s\n", argv[1]);
		exit(1);
	}
	
	// Creating file descriptors for destination file
	if((fd_wr = open(argv[2], O_WRONLY | O_CREAT, 0664)) == -1){
		perror("ERROR (fd_wr-open)");
		exit(1);
	}

	// Reading upto 128 bytes at a time and copying them
	do{
		if((rbytes = read(fd_rd, read_buff, BUFF_SIZE)) == -1){
			perror("ERROR (rbytes)");
			exit(1);
		}
		if((wbytes = write(fd_wr, read_buff, rbytes)) == -1){
			perror("ERROR (wbytes)");
			exit(1);
		}
		// In case the read bytes mismatch written bytes
		if(rbytes != wbytes){
			perror("ERROR (rbytes OR wbytes)");
			exit(1);
		}
	}while(rbytes > 0);

	printf("Success\n");
	close(fd_rd);
    close(fd_wr);
	return 0;	
}