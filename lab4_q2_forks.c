#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#define MAX_COMMAND_LENGTH 300 		// getconf ARG_MAX -> 2097152

int main(){
	pid_t son_pid;
	char command[MAX_COMMAND_LENGTH];

	while(1){
		printf("$ ");
		fgets(command, MAX_COMMAND_LENGTH, stdin);
		command[strlen(command)-1] = '\0';		// removing newline char
		if(command[0] == '\0') continue; 		// No command entered
		if(!strcmp(command,"q")){				// exit command entered
			printf("END!\n");
			break;
		}

		// forking sub-proccess
		if(son_pid = fork()){
			// Parent's code / fork failure
			if(son_pid == -1){
                perror("ERROR (fork)");
				exit(1);
			}
            if(waitpid(son_pid) == -1){
                perror("ERROR (wait)");
				exit(1);
			}
		}else{
			// Son's code
			if(execlp(command, command, NULL) == -1){
				printf("ERROR ERROR ERROR\n");
				exit(1);
			}
			// killing child in case of bad command
			exit(0);
		}
	}
	return 0;
}