#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "spawnChilds.h"

#define NUM_CHILD 5

volatile sig_atomic_t finishedChilds = NUM_CHILD;

void handle_SIGCHLD(int signo, siginfo_t *sinfo, void *context) {
    finishedChilds--;
}

int main() {
    pid_t pid;
    int i;
    int idx = spawn_childs(NUM_CHILD);
    int start = idx * 200;
    int end = (idx + 5) * 200;

    struct sigaction act;
    int status;

    memset(&act, 0, sizeof(struct sigaction));
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGCHLD);

    act.sa_sigaction = handle_SIGCHLD;
    act.sa_flags = SA_SIGINFO;
    act.sa_flags = SA_NOCLDWAIT;
    act.sa_flags = SA_NOCLDSTOP;
    sigaction(SIGCHLD, &act, NULL);

    if (idx == 0) {
        while (finishedChilds > 0) {
            pause(); 
        }

        for (i = 0; i < NUM_CHILD; i++) {
            wait(&status);
        }    
    } else {
        for (i = start; i < end; i++) {
            printf("%d\n", i);
        }
        
        exit(0);
    }

    return 0;
}