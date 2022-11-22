#include <stdio.h>
#include <unistd.h>
#define N 10

int main()
{
    int c1, c2, t, i;
    c1 = 0;
    c2 = 1;
    for (i = 1; i < N; i++){
        switch(fork()){
            case -1:
                printf("Error creating process");
                return -1;
            case 0:
                // sons code
                t = c1;
                c1 = c2;
                c2 += t;
                break;
            default:
                waitpid(0, NULL, 0);
                i = N; // out from loop
                break;
        }
    }
    printf("%d ", c2);
}



