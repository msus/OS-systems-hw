#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>

#define N 100

// Global variabels and struct
int array[N];
int found = 0;
int num;
struct searcher_kitbag
    {
        int start;
        int finish;
        int *array_ptr;
    }searcher_kitbag;

/* searcher_thread uses a 'kitbag' struct in order to
    recive search parameters and a pointer */
void* searcher_thread (void* kitbag);

int main ()
{
    pthread_t searcher1_t, searcher2_t;
    int searcher1_rc, searcher2_rc;
    int i;

    // Creating randomization seed and placment of random numbers in array[]
    srand(time(NULL));
    for (i = 0; i < N; i++)
        array[i] = rand() % 101;

    // Acquiring a number from end-user 
    while(1){
        printf("\nPlease enter a number (0-100): ");
        scanf("%d", &num);
        if(0 <= num && num <= 100)
            break;
    }
    printf("Searching for %d in the array.\n", num);

    // Creating 'kitbags' for managment of search parameters and int pointer
    struct searcher_kitbag kitbag1 = {0, N/2, array};
    struct searcher_kitbag kitbag2 = {N/2, N, &array[N/2]};

    // Threads creation
    searcher1_rc = pthread_create(&searcher1_t, NULL, searcher_thread, (void *) &kitbag1);
    searcher2_rc = pthread_create(&searcher2_t, NULL, searcher_thread, (void *) &kitbag2);

    // Detecting errors in thread creation
    if (searcher1_rc != 0 || searcher2_rc != 0){
        printf("Failed to create thread(s)\n");
        exit(1);
    }
    
    // joining threads
    if (pthread_join(searcher1_t, NULL) != 0){
        printf("Failure joining thread (printer_t)\n");
        exit(1);
    }
    if (pthread_join(searcher2_t, NULL) != 0){
        printf("Failure joining thread (printer_t)\n");
        exit(1);
    }

    printf("%sfound\n", found? "":"not ");
    return 0;
}

void* searcher_thread (void* kitbag)
{
    struct searcher_kitbag my_kitbag = *(struct searcher_kitbag*) kitbag;
    int i = my_kitbag.start, limit = my_kitbag.finish;
    int* ptr = (int *) my_kitbag.array_ptr;
    // Searching for num within search parameters
    for(; i < limit; i++){
         if(num == *ptr || found){
            found = 1;
            return NULL;
        }
        ptr++;
    }
    return NULL;
}