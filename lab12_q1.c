#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdint.h>
#include <time.h>
#define JURY_COUNT 15
#define JUDGE_COUNT 5

void* Jury(void* ind);
void enterJury();
void exitJury();
void* Judge(void* ind);
void enterJudge();
void exitJudge();
void check_start();

int trial_start = 0;
int juries_in_court_count = 0;
int judges_in_court_count = 0;
sem_t court_is_open, court_vars_mutex, judges_in_court, juries_in_court; // All init to 1
sem_t trial_end; // inited to 0

int main(int argc, char * argv[]){
    intptr_t i;
    pthread_t jury_thread_arr[JURY_COUNT], judge_thread_arr[JUDGE_COUNT];

    sem_init(&judges_in_court, 0, 1);
    sem_init(&juries_in_court, 0, 1);
	sem_init(&court_vars_mutex, 0, 1);
	sem_init(&court_is_open, 0, 1);
	sem_init(&trial_end, 0, 0);

	// Creating jurythreads
	for (i = 0; i<JURY_COUNT; i++){
		if (pthread_create(&jury_thread_arr[i], NULL, Jury, (void*) i) != 0){
			perror("Failure creating thread\n");
			exit(1);
		}
	}
	// Creating judge threads
	for (i = 0; i<JUDGE_COUNT; i++){
		if (pthread_create(&judge_thread_arr[i], NULL, Judge, (void*) i) != 0){
			perror("Failure creating thread\n");
			exit(1);
		}
	}
	
	// Go thru 10 trial
	sleep(20);

    return 0;
}

void enterJudge()
{
	sem_wait(&judges_in_court);
	sem_wait(&court_vars_mutex);	// Protect int
	judges_in_court_count = 1;
	sem_post(&court_vars_mutex);
	;
}

void exitJudge()
{
	int x;
	sem_wait(&court_vars_mutex);	// Protect int
	judges_in_court_count--;
	x = juries_in_court_count == 0 && judges_in_court_count == 0;
	if (x){
		trial_start = 0;
		sem_post(&judges_in_court);
		sem_post(&court_is_open);
	}
	sem_post(&court_vars_mutex);
}


void enterJury()
{
	sem_wait(&court_is_open);		// CLOSING COURT
	sem_wait(&court_vars_mutex);	// Protect int
	juries_in_court_count++;
	if (juries_in_court_count < 10)
		sem_post(&court_is_open);	// OPENING COURT
	sem_post(&court_vars_mutex);	
}

void exitJury()
{
	int x;
	sem_wait(&court_vars_mutex);	// Protect int
	juries_in_court_count--;
	x = juries_in_court_count == 0 && judges_in_court_count == 0;
	if (x){
		trial_start = 0;
		sem_post(&judges_in_court);
		sem_post(&court_is_open);
	}
	sem_post(&court_vars_mutex);
}


void check_start()
{
	int x, i;
	sem_wait(&court_vars_mutex);
	x = juries_in_court_count == 10 && judges_in_court_count == 1;
	if (x && !trial_start){
		trial_start = 1;
		printf("START\n");
		sleep(2);
		printf("END\n");
		for (i = 0; i < 11; i++)
			sem_post(&trial_end);
		//sem_post(&court_is_open);
	}
	sem_post(&court_vars_mutex);
}


void* Judge(void* ind)
{
    intptr_t num = (intptr_t) ind;
	while (1){
		enterJudge();
		printf("Judge #%lu in court.\n", num);
		check_start();
		sem_wait(&trial_end);
		//printf("Judge #%lu exit court.\n", num);
		exitJudge();
	}
	
}

void* Jury(void* ind)
{
    intptr_t num = (intptr_t) ind;
	while (1){
		enterJury();
		printf("Jury #%lu in court.\n", num);
		check_start();
		sem_wait(&trial_end);
		//printf("Jury #%lu exit court.\n", num);
		exitJury();

	}
	

}
