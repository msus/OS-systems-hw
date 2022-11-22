#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    int loops, son_id;
    if(argc != 3){
        printf("USAGE: runInLoop <INTEGER>\n");
        exit(0);
    }
    loops = atoi(argv[2]);
    for(; loops != 0; loops--){
        if((son_id = fork()) == -1){
            // failure forking
            perror("ERROR (fork)\n");
            exit(1);
        }if(son_id == 0 && execv(argv[1], NULL) == -1){
            // son's exec-ing failure handling 
            perror("ERROR (execv)\n");
            exit(1);
        }else{
            // paretn's code
            if(waitpid(son_id) == -1){
                perror("ERROR (wait)");
                exit(1);
            }
        }
    }
    printf("END\n");
    return 0;
}