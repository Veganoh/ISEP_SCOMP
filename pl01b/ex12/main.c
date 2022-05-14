#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

#include "spawnChilds.h"

#define NUM_CHILDS 50

volatile sig_atomic_t finishedChilds = 0;
volatile sig_atomic_t successfulChilds = 0;

int simulate1() {    
    srand(time(NULL));
    int n = rand() % 100;

    if (n > 80) {
        return 1;
    }
    return 0;
}

void simulate2() {
    printf("Simulating 2...\n");
    exit(0);
}

void handle_sig(int signo) {
    switch(signo) {
        case SIGUSR1: 
            finishedChilds++;
            successfulChilds++;

            break;
        case SIGUSR2:
            finishedChilds++;
        
            break;
        case SIGCONT:
            simulate2();
            break;
    }
}

int main() {
    pid_t pidArr[NUM_CHILDS];
    struct sigaction act;

    memset(&act, 0, sizeof(struct sigaction));
    sigfillset(&act.sa_mask);
    act.sa_handler = handle_sig;
    
    int idx = spawn_childs(NUM_CHILDS, pidArr);
    
    if (idx > 0) {
        sigaction(SIGCONT, &act, NULL);

        sleep(idx);
        printf("Simulating...\n");

        if (simulate1()) {
            kill(getppid(), SIGUSR1);
        } else {
            kill(getppid(), SIGUSR2);
        } 

        pause();
    } else {
        int i;
        
        sigaction(SIGUSR1, &act, NULL);
        sigaction(SIGUSR2, &act, NULL);

        while(finishedChilds < 25) {
            pause();
        }

        if (successfulChilds > 0) {
            for (i = 0; i < NUM_CHILDS; i++) {
                kill(pidArr[i], SIGCONT);
            }
        } else {
            printf("Inefficient algorithm!\n");

            for (i = 0; i < NUM_CHILDS; i++) {
                kill(pidArr[i], SIGKILL);
            }
        }
    }

    return 0;
}