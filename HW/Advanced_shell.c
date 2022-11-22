#include <stdio.h>	// printf, sprintf, perror, fprintf, fgets
#include <stdlib.h>	// exit, atoi
#include <string.h>	// strcmp, strtok, strcat, strlen, strstr
#include <unistd.h> // fork, exec.., wait, open?
#define MAX_COMMAND_LENGTH 300 		// getconf ARG_MAX -> 2097152
#define MAX_PATH 4096 				// as specified at linux/limits.h
#define MAX_ARGS 5 					// executable-name + 3 * args + NULL byte

int main(){
	int i, background, son_pid, is_our_command = 0; // pid_t or int?????
	char *word, *args[MAX_ARGS];
	char path[MAX_PATH], command[MAX_COMMAND_LENGTH];
	char *my_commands[6] = {"getMenu", "MakeOrder", "getPrice", "getOrderNum", "CreateMenu", ""};

	while(1){
		// Resetting variables
		is_our_command = 0;
		strcpy(path, "./");
		for(i = 0; i < MAX_ARGS; args[i++] = NULL);

		// Prompt, input pre-processing, check 'exit' command
		fprintf(stdout, "AdvShell> ");
		fgets(command, MAX_COMMAND_LENGTH, stdin);
		command[strlen(command)-1] = '\0';		// removing newline char
		if(command[0] == '\0') continue; 		// No command entered
		if(!strcmp(command,"exit")){			// exit command entered
			fprintf(stdout, "GoodBye...\n");
			exit(0);
		}

		// Segmenting the command into args array
		for(word = strtok(command, " "), i=0;
			word != NULL && i < MAX_ARGS;
			word = strtok(NULL, " "))
				args[i++] = word;

		// In case last arg is '&', background execution is required
		if(!strcmp(args[i-1], "&")){
			background = 1;
			args[i-1] = NULL;
		}
		
		// Checking if the command is our spechial resturant command
		for(i=0; my_commands[i][0] != '\0'; i++)
			if(!strcmp(my_commands[i], args[0])){
				is_our_command = 1;
				strcat(path, args[0]);
				break;
			}
		
		// forking sub-proccess
		if(son_pid = fork()){
			// Parent's code
			if(son_pid < 0){
				perror("ERROR (fork)");
				exit(1);
			}
			if(!background){
				if(wait(NULL) == -1){
					perror("ERROR (wait)");
					exit(1);
				}
			}else fprintf(stdout, "%s %d\n", args[0], son_pid);
		}else{
			// child code
			if(is_our_command && execv(path, args) != 0){ // RUN IN LOCAL FOLDER
				perror("ERROR (execv)");
				exit(1);
			}
			if(execvp(args[0], args) != 0){ // TO RUN FROM $PATH
				printf("Not Supported\n");
				//perror("ERROR (execvp)");
				exit(1);
			}
		} // End child code
	} // End while
	return 0;
}
