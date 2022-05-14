#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#include "spawnChilds.h"

#define NUM_CHILD 6

int main() {
    int i;
    int status;
    int idx = spawn_childs(NUM_CHILD);
    pid_t pid;

    if (idx != 0)
        exit(idx * 2);
    
    for (i = 0; i < NUM_CHILD; i++) {
        pid = wait(&status);

        if(WIFEXITED(status))
            printf("Process ended %d and returned %d!\n", pid, WEXITSTATUS(status));
    }

    return 0;
}