#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    int i;
    int status;
    int counterPair = 0;
    pid_t sonPid;
    pid_t evenPid[4];

    for (i = 0; i < 4; i++) {
        sonPid = fork();

        if (sonPid == 0) {
            sleep(1); /*sleep(): unistd.h*/
            break; //or exit(0)
        } else if (sonPid % 2 == 0) {
            evenPid[counterPair] = sonPid;
            counterPair++;
        }
    }

    if (sonPid > 0) {
        for (i = 0; i < counterPair; i++){
            //printf("Waiting for %d...\n", evenPid[i]); //Debug line
            waitpid(evenPid[i], &status, 0);
            //printf("Done waiting for %d!\n", evenPid[i]); //Debug line
        }
        
        i = 0;
    } else 
        i++;
    
    printf("This is the end.\n");
    //printf("pid = %d | i = %d\n\n", getpid(), i); //Debug line
    exit(i);
} 

/*
    a) 16 Processes

    b) 
        Place 'break' in the child procress so that it doesn't repeat the loop. In case the code outside the loop doesn't matter when the Child is running then
        place exit(0) instead so that the process ends.
*/