#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <semaphore.h>

sem_t sem1;

void* A (void* str)
{
    while(1){
        sleep(1);
        sem_post(&sem1);
        printf("Produced A\n");
    }
}

void* AA (void* str)
{
    int i = 0;
    while(1){
        sem_wait(&sem1);
        sem_wait(&sem1);
        printf ("Collected AA\n");
    }
}
 
int main()
{
    pthread_t threadA, threadAA;
    sem_init(&sem1, 0, 0);
    if(pthread_create(&threadA, NULL, A, (void*)"Thread A") != 0 ||
        pthread_create(&threadAA, NULL, AA, (void*)"Thread AA") != 0){
            printf("Error creating thread/s\n");
            exit(1);
        }
    sleep(20);
    //pthread_cancel(threadA);
    //pthread_cancel(threadAA);
    return 0;
}

