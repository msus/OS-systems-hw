#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#define BUFF 257
#define MAX_BUFF 1024

int main(int argc, char* argv[]) {           // params: restaurant name, customer name //
	int i = 0, amount, sum = 0, price, fd_r, fd_w, rbytes, wbytes;
	char dir[BUFF], temp[BUFF], amount_str[BUFF], price_str[BUFF], line[BUFF], original_line[BUFF], header[BUFF],
		* ptr, dish[BUFF], rest_name[BUFF], menu_buffer[MAX_BUFF];
	time_t T = time(NULL);
	struct tm tm = *localtime(&T);

	if(argc != 3){
		printf("USAGE: ./MakeOrder <RESTURANT_NAME> <CUSTOMER_NAME>\n");
		exit(0);
	}

	// set order txt file path //
	sprintf(rest_name, "%s.txt", argv[1]);
	getcwd(dir, BUFF);
	sprintf(temp, "%s/%s_Order", dir, argv[1]);
	mkdir(temp, 0774);
	sprintf(dir + strlen(dir), "/%s_Order/%s.txt", argv[1], argv[2]);

	// open files //
	if ((fd_r = open(rest_name, O_RDONLY)) == -1) { printf("Restaurant wasnt found\n"); exit(1); }
	if ((fd_w = open(dir, O_WRONLY | O_CREAT, 0664)) == -1) { perror("error opening file: "); exit(1); }

	// set order header //
	strcpy(header, argv[1]); strcat(header, " Order\n\n\0");
	if ((wbytes = write(fd_w, header, strlen(header))) == -1) { perror("error writing to file: "); exit(1); }
	if (wbytes != strlen(header)) { printf("writing has been corrupted\n"); exit(1); }


	printf("Insert your order (entering \"Finish\" to finish):\n");

	while (1) {
		// scan user input //
		fgets(line, BUFF, stdin); line[strlen(line) - 1] = '\0';

		// loop stop condition //
		if (strcmp(line, "Finish\0") == 0)
			break;

		strcpy(original_line, line);

		// set i so that (line+i) will point to the end of a dish // 
		i = 0;
		while (!isdigit(*(line + i)))
			i++;

		// set amount_str and dish strings //
		strcpy(amount_str, line + i);
		line[i] = '\0';
		strcpy(dish, line);


		// set amount as int //
		amount = atoi(amount_str);

		// search dish in menu // 
		// read menu from menu txt file //
		do {
			rbytes = read(fd_r, menu_buffer, MAX_BUFF);
			if (rbytes == -1) { perror("error reading file: "); exit(1); }
			ptr = strstr(menu_buffer, dish);

		} while (ptr == NULL && rbytes != 0);

		// set pointer of file descriptor to top of file //
		lseek(fd_r, 0, SEEK_SET);

		// if rbytes was 0 in last iteration then ptr isnt found --> print appropriate massage and continue iterating //
		if (rbytes == 0 && rbytes == 0) {
			printf("dish Not Found!\n");
			continue;
		}

		// extract price from menu //
		// set i so that (ptr+i) points to to beginning of dish's price //
		i = 0;
		while (*(ptr + i) != '\0' && !isdigit(*(ptr + i)))
			i++;

		strcpy(price_str, ptr + i);
		price = atoi(price_str);

		sum = sum + price * amount;    // calculate overall sum //


		// write ordered item to file & handle errors //
		strcat(original_line, "\n\0");
		wbytes = write(fd_w, original_line, strlen(original_line));
		if ((wbytes) == -1) { perror("error writing to file: "); exit(1); }
		if (wbytes != strlen(original_line)) { printf("writing has been corrupted\n"); exit(1); }

	}

	// write sum to txt file //
	sprintf(temp, "\nTotal Price: %dNIS", sum);
	
	printf("Total Price: %dNIS (Confirm to approve/else cancel)\n", sum);
	scanf("%s", temp);
	if(strcmp(temp, "Confirm")){
		// cancel order
		printf("Order canceled\n");
		close(fd_w);
		close(fd_r);
		if(remove(dir) == -1){
			perror("error deleting file");
			exit(1);
		}
		return 0;
	}

	if ((wbytes = write(fd_w, temp, strlen(temp))) == -1) { perror("error writing to file: "); exit(1); }
	if (wbytes != strlen(temp)) { printf("error: writing haen corrupted\n"); exit(1); }

	// write date to txt file //
	snprintf(temp, BUFF, "\n\n%02d/%02d/%04d\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
	if ((wbytes = write(fd_w, temp, strlen(temp))) == -1) { perror("error writing to file: "); exit(1); }
	if (wbytes != strlen(temp)) { printf("error: writing haen corrupted\n"); exit(1); }


	printf("%s.txt Created in %s_Order Dir \n", argv[2], argv[1]);        // print output to user //
	chmod(dir, 0444);                  // change permissions of order //
	close(fd_w); close(fd_r);
	return 0;
}