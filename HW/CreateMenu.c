#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#define BUFF_SIZE 1024
#define MAX_RESTURANT_STRINGS 25

int main(int argc, char *argv[]){
	int menu_count = atoi(argv[2]);
	int fd_wr, wrbytes, i, j;
	char wr_buff[BUFF_SIZE];
	char dish_letter = 'a';
	char resturant_name[MAX_RESTURANT_STRINGS];
	char filename[MAX_RESTURANT_STRINGS];
	char type_dish[MAX_RESTURANT_STRINGS];
	char folder_name[MAX_RESTURANT_STRINGS];
	char dish_name[MAX_RESTURANT_STRINGS] = {'\0'};
	char *digit_ptr;

	if(argc != 3){
		printf("USAGE: ./CreateMenu <RESTURANT_NAME> <MENU_COUNT>\n");
		exit(0);
	}

	// Initalizing variabels
	strcpy(resturant_name, argv[1]);
	sprintf(filename, "%s.txt", resturant_name);

	// Creating file descriptor
	if((fd_wr = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0664)) == -1){
		perror("ERROR (fd_wr-open)");
		exit(1);
	}

	sprintf(wr_buff, "%s Menu\n\n", resturant_name);
	if((wrbytes = write(fd_wr, wr_buff, strlen(wr_buff))) == -1){
			perror("ERROR (wbytes)");
			exit(1);
	}

	// main loop a,b,c...
	for(; menu_count != 0; menu_count--){
		strcpy(type_dish, "");
		fprintf(stdout, "Insert Type Dish %c: ", dish_letter);
		fgets(type_dish, MAX_RESTURANT_STRINGS, stdin);
		type_dish[strlen(type_dish)-1] = '\0'; 			// removing newline char
		if(!type_dish) continue;						// no type_dish entered
		sprintf(wr_buff, "%c. %s\n", dish_letter++, type_dish);
		if((wrbytes = write(fd_wr, wr_buff, strlen(wr_buff))) == -1){
			perror("ERROR (wbytes)");
			exit(1); 
		}

		// Inner loop for dish names
		for(i=1; ;i++){
			fprintf(stdout, "Insert dish name and name %d: ", i);
			fgets(dish_name, MAX_RESTURANT_STRINGS, stdin); 
			dish_name[strlen(dish_name)-1] = '\0'; 			// removing newline char
			if(!strcmp(dish_name, "stop"))
				break;
			
			// Building dish line
			strcpy(wr_buff, "\t");
			digit_ptr = dish_name;
			for(digit_ptr = strtok(dish_name, " ");
				digit_ptr != NULL && !isdigit(digit_ptr[0]);
				digit_ptr = strtok(NULL, " ")
				){
					strcat(wr_buff, digit_ptr);
					strcat(wr_buff, " ");
				}

			for(j=MAX_RESTURANT_STRINGS - strlen(wr_buff); j>0; j--, strcat(wr_buff, "."));
			strcat(wr_buff, " ");
			strcat(wr_buff, digit_ptr);
			strcat(wr_buff, "NIS\n");	

			if((wrbytes = write(fd_wr, wr_buff, strlen(wr_buff))) == -1){
				perror("ERROR (wbytes)");
				exit(1);
			}
		}
	}

	sprintf(wr_buff, "\n\n\t\tBon Appetit\n");
	if((wrbytes = write(fd_wr, wr_buff, strlen(wr_buff))) == -1){
		perror("ERROR (wbytes)");
		exit(1);
	}

	sprintf(folder_name, "%s_Order", resturant_name);
	if(mkdir(folder_name, 0755) != 0){
		perror("ERROR (mkdir)");
		exit(1);
	}

	close(fd_wr);
	return 0;
}