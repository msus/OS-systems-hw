#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdint.h>
#include <time.h>

/*	number | City name		//
	   0   |  Karmiel		//	if Tzfat & Haifa on call:
	   1   |  Tiberia		//		Karmiel & Tiberia won't make new calls
	   2   |  Haifa			//
	   3   |  Tzfat		*///// 

sem_t mutex_caller[4], mutex;
int state[4];
int call_session[4] = {-1, -1, -1, -1};

void Calls(int City1, int City2);
void* thread_make_call(void* p);


int main(int argc, char * argv[]){
    pthread_t tArr[4];
	intptr_t i;
    int tAns[4];

	srand(time(0));

	for(i=0; i<4; i++)
    	sem_init(&mutex_caller[i], 0, 0);

	sem_init(&mutex, 0, 1);

    for (i=0; i<4; i++){
        tAns[i] = pthread_create(&tArr[i], NULL,  thread_make_call, (void*) i);
		if (tAns[i] != 0){
			printf("ERROR in threads creation.");
			exit(1);
		}
	}

    sleep(20);
    return 0;
}

void Calls(int City1, int City2)
{
	int i;
    sem_wait(&mutex);
    call_session[City1] = City2;
    while (call_session[City1] == City2) {
        for (i = 0; i < 4; i++) {
            if (call_session[i] == City1 &&
				state[i] == 0 &&
                state[City1] == 0) 
				{
                state[City1] = state[i] = 1;
                call_session[i] = -1;
                sem_post(&mutex_caller[i]);
                sem_post(&mutex);
				sem_wait(&mutex_caller[City1]);
				sem_wait(&mutex);
            }
        }
        sem_post(&mutex_caller[City2]);
		sem_post(&mutex_caller[i]);
		sem_post(&mutex);
		sem_wait(&mutex_caller[City1]);
		sem_wait(&mutex);
    }

    if (City1 + City2 == 5)
        sem_post(&mutex);
    
    printf("%d started to talk with %d\n", City1, City2); 
    sleep(1); 
    printf("%d finished to talk with %d\n", City1, City2);

    if (City1 + City2 == 5)
        sem_wait(&mutex);

    state[City1] = state[City2] = 0;
    sem_post(&mutex_caller[City2]);
    sem_post(&mutex);
}

void* thread_make_call(void* ind){
	intptr_t caller = (intptr_t) ind;
	int callto;
	while(1){
		callto = rand() % 4;
		if (callto != caller){
			Calls(caller, callto);
		}
	}
}
