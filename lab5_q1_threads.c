#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#define N 30
#define M 10

int matrix[N][M];

void* worker (void* row);

int main (){
    pthread_t arr_threads[N];
    int arr_rc[N], j;
    long i;
    // Creating randomization seed
    srand(time(NULL));

    // Creating threads
    for (i = 0; i < N; i++){
        arr_rc[i] = pthread_create(&arr_threads[i], NULL, worker, (void*)i);
        // Checking for errors in threads creation
        if (arr_rc[i] != 0){
            printf("\nFailed to create threads\n");
            exit(1);
        }
    }

    // Joining threads
    for (i = 0; i < N; i++)
        if(pthread_join(arr_threads[i], NULL) != 0){
            printf("Failure joining thread\n");
            exit(1);
        }

    // Printing the matrix
    for (i = 0; i < N; i++){
        printf("[");
        for (j = 0; j < M; j++){
            printf("%3d%s", matrix[i][j], j==(M-1)?"":", ");
        }
        printf("]\n");
    }
    return 0;
}

void* worker (void* row){
    int i;
    for (i = 0; i < M; i++)
        matrix[(long) row][i] = rand() % 101;
}