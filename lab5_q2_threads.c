#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void* printer_thread (void*);
void* incrument_thread (void*);

unsigned long int counter = 0;

int main (){
    pthread_t printer_t, incrument_t;
    int printer_rc, incrument_rc;

    // Creating threads
    printer_rc = pthread_create(&printer_t, NULL, printer_thread, NULL);
    incrument_rc = pthread_create(&incrument_t, NULL, incrument_thread, NULL);

    // Checking for errors in threads creation
    if (printer_rc != 0 || incrument_rc != 0){
        printf("Failed to create thread(s)\n");
        exit(1);
    }

    // Joining threads
    if (pthread_join(printer_t, NULL) != 0){
        printf("Failure joining thread (printer_t)\n");
        exit(1);
    }
    if(pthread_cancel(incrument_t) != 0){
        printf("Failure joining thread (incrument_t)\n");
        exit(1);
    }
    
    return 0;
}

void* printer_thread (void*)
{
    int i;
    for (i = 0; i < 10; i++){
        printf("%lu\n", counter);
        usleep(100000); // tenth of a second
    }
    return NULL;
}

void* incrument_thread (void*)
{
    while(1) ++counter;
    return NULL;
}