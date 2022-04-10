#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "spawnChilds.h"

#define NUM_CHILDS 5

int main() {
    int fd[2];
    int vec1[1000];
    int vec2[1000];
    int resultArr[5];
    int result = 0;
    int tmp = 0;
    int i;

    if(pipe(fd) == -1) {
        perror("Pipe failed!");
        exit(1);
    }

    for (i = 0; i < 1000; i++) {
        vec1[i] = 1;
        vec2[i] = 1;
    }

    int idx = spawn_childs(NUM_CHILDS);
    
    if (idx > 0) {
        close(fd[0]);

        for (i = (idx - 1) * 200; i < idx * 200; i++) {
           tmp += vec1[i] + vec2[i];
        }

        write(fd[1], &tmp, sizeof(int));

        close(fd[1]);
        exit(0);
    } else {
        close(fd[1]);

        for (i = 0; i < NUM_CHILDS; i++) {
            read(fd[0], &tmp, sizeof(int));
            resultArr[i] = tmp;
        }

        close(fd[0]);
    }

    for (i = 0; i < 5; i++) {
        result += resultArr[i];
    }

    printf("%d\n", result);

    return 0;
}

/*
    a) There is no need for any type of synchronization mechanism because pipes have a buffer system in place, where the written data is buffered by the kernel
    untill it is read.
*/