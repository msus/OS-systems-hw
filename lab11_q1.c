#include <stdio.h>
#include <stdlib.h>
#include<unistd.h>

int main() {
    while (1)
        switch (fork()){
        case -1:
            printf("ERROR forking.");
            exit(1);
        case 0:
            // child
            printf("a\n");
            return 0;            
        default:
            // daddy
            wait();
            printf("b\n");
            break;
        }
    return 0;
}