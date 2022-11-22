#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <stdint.h>

void* thread(void*);
int n, found = 0;


int main(int argc, char* argv[])
{
	// Chcking for argument
    if (argc != 2){
        perror("USAGE: ./q2 <NUM_OF_THREADS>\n");
        exit(0);
    }

	// Casting char into int
    n = atoi(argv[1]);
    pthread_t thread1, thread2;

	// Creating threads
    if (pthread_create(&thread1, NULL, thread, (void*) 1) != 0 ||
        pthread_create(&thread2, NULL, thread, (void*) 2) != 0){
        printf("\nFailed to create thread\n");
        exit(1);
    }
	// Joining threads
    if (pthread_join(thread1, NULL) != 0 ||
        pthread_join(thread2, NULL) != 0){
        printf("\nFailed to create thread\n");
        exit(1);
    }

	// In case no divisor is found - number is prime
    if (!found)
        printf("%d is prime number.\n", n);
    
    return 0;
}


void* thread(void* arg)
{
    intptr_t offset = (intptr_t) arg;
    int j = n - offset;
    for (; j > 1; j -= 2){
		// Is j is a divisor of n?
        if (n % j == 0){
            if (!found) // first divisor found
                printf("Devisors of %d\n", n);
            printf("%d\n", j);
            found = 1; // Global flag
        }
    }
}




