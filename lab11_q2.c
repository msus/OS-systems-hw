#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
	char string[1] = "c";
	int fd[2];
	if (pipe(fd); == -1) {
        perror("error while creating pipe\n");
        exit(1);
    }
	
    switch (fork()){
    case -1:
        perror("fork error has occured\n");
        exit(1);
    case 0:
        // child
        dup2(fd[1], 1);
        close(fd[0]);
        close(fd[1]);
        execlp("date", "date", NULL);
        return 0;    
    default:
        // daddy
        wait();
        dup2(fd[0], 0);
        close(fd[0]);
        close(fd[1]);
        printf("In date there are:\n    Line: Words: Chars:\n");
        execlp("wc", "wc", NULL);
        break;
    }
    return 0;
}