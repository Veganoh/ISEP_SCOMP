#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


int main(){
    pid_t p;

    if (fork() == 0) {
        printf("PID = %d\n", getpid()); 
        exit(0);
    }

    if ((p=fork()) == 0) {
        printf("PID = %d\n", getpid()); 
        exit(0);
    }

    printf("Parent PID = %d\n", getpid());
    printf("Waiting... (for PID=%d)\n",p);
    waitpid(p, NULL, 0);

    printf("Enter Loop...\n");
    while (1); /* Infinite loop */
}

/*
    a) No they aren't, because the second child process was reaped by the parent process with the 'waitpid()' function so it isn't listed. 
    Meanwhile the parent process is stuck in an infinite loop and the first child process became a zombie and is still listed as a <defunct> process.

    b) The first child process became a zombie and is still listed as a <defunct> process since the parent process didn't acknoledge its termination with 'wait()'/'waitpid()'.
*/