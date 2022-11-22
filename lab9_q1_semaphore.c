#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <semaphore.h>
#define K 5
#define N 10


//ניתן להוסיף קוד כרצונך
void stkPush(int num);
int stkPop();
void* tFunc(void* p);
int stk[N], idx = 0;
sem_t pop_sem, push_sem, mutex;

int main(int argc, char * argv[])
{
    pthread_t tArr[K];
    int i=0, ans[K];

    //ניתן להוסיף קוד כרצונך
    sem_init(&pop_sem, 0, 0);
    sem_init(&push_sem, 0, N);
    sem_init(&mutex, 0, 1);

    for(i=0; i<K; i++)
        ans[i] = pthread_create(&tArr[i], NULL,  tFunc, NULL);
    sleep(10);
    return 0;
}

void* tFunc(void* p)
{
    int num;
    while(1){
        num = rand()%100;
        if(rand()%2){
            stkPush(num);
            printf("Push %d\n", num);
        }else{
            printf("Pop %d\n", stkPop()); 
        }
        sleep(1);
    }
}


int stkPop()
{
    int ans;
    sem_wait(&pop_sem);
    sem_post(&push_sem);
    sem_wait(&mutex);
    ans = stk[--idx];
    sem_post(&mutex);
    return ans;
}

void stkPush(int num)
{
    sem_wait(&push_sem);
    sem_wait(&mutex);
    stk[idx++] = num;
    sem_post(&mutex);
    sem_post(&pop_sem);
}