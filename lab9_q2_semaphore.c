#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdint.h>
#define M 3
#define N 2


sem_t block_everyone, block_youngsters;
int youngsters = 0;
void* oldManWantsToBuy(void* ind);
void* youngManWantsToBuy(void* ind);

int main(int argc, char * argv[]){
    intptr_t i=0, ans1[M], ans2[N];
    pthread_t tArr1[M], tArr2[N];

    sem_init(&block_everyone, 0, 1);
    sem_init(&block_youngsters, 0, 1);

    /*
    for(i=0; i<M; i++)
        ans1[i] = pthread_create(&tArr1[i], NULL, oldManWantsToBuy, (void*) i);
    for(i=0; i<N; i++)
        ans2[i] = pthread_create(&tArr2[i], NULL, youngManWantsToBuy, (void*) i);
    */
    i=0;
    while(1){
        if(i<M)
            pthread_create(&tArr1[i], NULL, oldManWantsToBuy, (void*) i);
        if(i<N)
            pthread_create(&tArr2[i], NULL, youngManWantsToBuy, (void*) i);
        i++;
    }

    for(i=0; i<M; i++)
        pthread_join(tArr1[i], NULL);
    for(i=0; i<N; i++)
        pthread_join(tArr1[i], NULL);
    return 0;
}

void* oldManWantsToBuy(void* ind){
    intptr_t num = (intptr_t) ind;
    printf("Old man #%d wants to buy\n", num);
    sem_wait(&block_everyone);
    printf("Old man #%d is buying\n", num);
    sleep(rand()%2);
    printf("Old man #%d is done\n", num);
    sem_post(&block_everyone);
}

void* youngManWantsToBuy(void* ind){
    intptr_t num = (intptr_t) ind;
    printf("Young man #%d wants to buy\n", num);
    sem_wait(&block_youngsters);
    youngsters++;
    if (youngsters == 1)
        sem_wait(&block_everyone);
    sem_post(&block_youngsters);
    printf("Young man #%d is buying\n", num);
    sleep(rand()%2);
    printf("Young man #%d is done\n", num);
    sem_wait(&block_youngsters);
    youngsters--;
    if (youngsters == 0)
        sem_post(&block_everyone);
    sem_post(&block_youngsters);
}
