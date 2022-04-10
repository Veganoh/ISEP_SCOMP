#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int spawn_childs(int n, pid_t *arr) {
    int i;

    for (i = 1; i <= n; i++) {
        if (fork() == 0) {
            *arr = getpid();
            arr++; 
            return i;
        } 
    }

    return 0;
}