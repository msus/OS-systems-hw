#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>

sem_t sem1, sem2;

void* create_message(void* str )
{
    int i = 0;
    for (i = 0; i < 10; i++){
        sem_wait(&sem1);
        printf ("I've wrote a message #%d. %s\n", i+1, (char*)str);
        sem_post(&sem2);
    }
}
 
int main()
{
    pthread_t thread;
    int i = 0;
    sem_init(&sem1, 0, 1);
    sem_init(&sem2, 0, 0);
    pthread_create (&thread, NULL, create_message, (void*)"Thread A");
    for ( i = 0; i < 10; i++ ){
        sem_wait(&sem2);
        printf ("The message #%d was printed. Thread B \n\n", i+1);
        sem_post(&sem1);
    }
    pthread_join(thread, NULL);
    return 0;
}

