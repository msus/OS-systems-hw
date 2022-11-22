#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

void* thread(void*);
void* printer_thread(void*);
long *counters;
int n;

int main(int argc, char* argv[])
{
    if (argc != 2){
        perror("USAGE: ./q2 <NUM_OF_THREADS>\n");
        exit(0);
    }

    n = atoi(argv[1]);
    pthread_t *arr_threads, prnt_thread;
    int *arr_rc, j;
    intptr_t i;         // int but with sozeof(void*) bit length

    arr_threads = (pthread_t*) malloc(n * sizeof(pthread_t));
    arr_rc = (int*) malloc(n * sizeof(int));
    counters = (long*) malloc(n * sizeof(long));

    // Creating threads
    for (i = 0; i < n; i++){
        arr_rc[i] = pthread_create(&arr_threads[i], NULL, thread, (void*) i);
        // Checking for errors in threads creation
        if (arr_rc[i] != 0){
            printf("\nFailed to create threads\n");
            exit(1);
        }
    }

    if (pthread_create(&prnt_thread, NULL, printer_thread, NULL) != 0){
        printf("\nFailed to create thread\n");
        exit(1);
    }
    if (pthread_join(prnt_thread, NULL) != 0){
        printf("\nFailed to create thread\n");
        exit(1);
    }
    
    // canceling threads
    for (i = 0; i < n; i++)
        if(pthread_cancel(arr_threads[i]) != 0){
            printf("Failure canceling thread\n");
            exit(1);
    }
    free(counters);
    free(arr_rc);
    free(arr_threads);
    return 0;
}


void* printer_thread(void* a){
    int i, j;
    for (i = 0; i < 10; i++){
        for(j = 0; j < n; j++)
            printf("Tread %d counted %lu\n", j, counters[j]);
         sleep(2);
    }
}


void* thread(void* my_num)
{
    intptr_t thread_id = (intptr_t) my_num;
    while (1)
        ++counters[thread_id];
}