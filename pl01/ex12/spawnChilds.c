#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


int spawn_childs(int n) {
    int i;

    for (i = 1; i <= n; i++) {
        if (fork() == 0) {
            return i;
        } 
    }

    return 0;
}