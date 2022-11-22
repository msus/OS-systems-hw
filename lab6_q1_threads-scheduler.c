#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

long cpu_holder = 0;
void* quanta_finder(void*);

int main(int argc, char* argv[])
{
    if (argc != 2){
        perror("USAGE: ./q1 <NUM_OF_THREADS>\n");
        exit(0);
    }

    int n = atoi(argv[1]);
    pthread_t *arr_threads;
    int *arr_rc;
    intptr_t i;         // int but with sozeof(void*) bit length

    arr_threads = (pthread_t*) malloc(n * sizeof(pthread_t));
    arr_rc = (int*) malloc(n * sizeof(int));

    // Creating threads
    for (i = 0; i < n; i++){
        arr_rc[i] = pthread_create(&arr_threads[i], NULL, quanta_finder, (void*) i);
        // Checking for errors in threads creation
        if (arr_rc[i] != 0){
            printf("\nFailed to create threads\n");
            exit(1);
        }
    }

    // Joining threads
    for (i = 0; i < n; i++)
        if(pthread_join(arr_threads[i], NULL) != 0){
            printf("Failure joining thread\n");
            exit(1);
        }
    return 0;
}

void* quanta_finder(void* my_num)
{
    intptr_t thread_id = (intptr_t) my_num;
    struct timeval t1, t2;
    double elapsedTime;
    while(1){
        cpu_holder = thread_id;
        gettimeofday(&t1, NULL);
        while(cpu_holder == thread_id){
            gettimeofday(&t2, NULL);
        }
        elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
        elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
        printf("THREAD %lu quanta: %.2lf\n", thread_id, elapsedTime);
    }
}