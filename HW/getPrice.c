#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#define MAX_RESTURANT_STRINGS 25
#define BUFF_SIZE 128

int main(int argc, char *argv[]){
    int i = 2, fd_rd, rbytes, price = -1;
    char *readc, *ptr, *tmp, tmp_buff[BUFF_SIZE];
    char resturant_file[MAX_RESTURANT_STRINGS];
    char dish_name[MAX_RESTURANT_STRINGS];

    if(argc < 3){
		printf("USAGE: ./getPrice <RESTURANT_NAME> <DISH_NAME>\n");
		exit(0);
	}

    // Collecting the 2nd arg onwards to form a dish name
    for(dish_name[0] = '\0';
        i < argc;
        strcat(dish_name, argv[i++]),
        strcat(dish_name, " "));

    // Memmory allocation and file descriptor creation
    if ((tmp = (char *) malloc(BUFF_SIZE)) == NULL){
        printf("Couldn't allocate memmory\n");
        exit(1);
    }
    readc = tmp;

    // Initiate 'readc' and create file descriptor
    readc[0] = '\0';
    sprintf(resturant_file, "%s.txt", argv[1]);
    if((fd_rd = open(resturant_file, O_RDONLY)) == -1){
        printf("Restaurant Not Found!\n");
		//perror("ERROR (fd_rd-open)");
		exit(1);
	}

    // Dinamicly reading the file into 'readc'
    do{
		if((rbytes = read(fd_rd, tmp_buff, BUFF_SIZE-1)) == -1){
			perror("ERROR (rbytes)");
			exit(1);
		}
        tmp_buff[rbytes] = '\0';
        if(rbytes > 0 && (tmp = (char *) realloc(readc, strlen(readc) + rbytes + 1)) == NULL){
            printf("Couldn't allocate memmory\n");
            exit(1);
        }
        readc = tmp;
        
        strcat(readc, tmp_buff);
	}while(rbytes > 0);

    // Finding the corrent line
    ptr = strstr(readc, dish_name);
    if(ptr == '\0'){
        printf("Dish not found\n");
        exit(0);
    }
    // Getting pointer to the price `XXXNIS`
    ptr = strstr(ptr, ". ") + 2;
    // Converting to ineger
    price = atoi(ptr);

    printf("%dNIS\n", price);
    free(readc);
    return price;
}