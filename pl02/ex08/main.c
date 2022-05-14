#include <stdio.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

#include "spawnChilds.h"

#define NUM_CHILDS 10

typedef struct {
    int round_number;
    char msg[4];
} data_p;

void catch_the_pipe(int fd) {
    data_p data;

    read(fd, &data, sizeof(data_p));

    printf("%s - Round %d - PID: %d\n", data.msg, data.round_number, getpid());

    exit(data.round_number);
    close(fd);
}

void send_the_pipe(int fd) {
    data_p  data;
    int i; 

    strcpy(data.msg, "Win");

    for (i = 1; i <= NUM_CHILDS; i++) {
        data.round_number = i;

        write(fd, &data, sizeof(data_p));
        sleep(2);
    }
}

int main() {
    int fd[2];
    int status;
    int i;
    pid_t pid;
    data_p  data;
    
    if(pipe(fd) == -1) {
        perror("Pipe failed!");
        exit(1);
    }

    int idx = spawn_childs(NUM_CHILDS);

    if (idx > 0) {
        close(fd[1]);

        catch_the_pipe(fd[0]);
    } else {
        close(fd[0]);
        
        send_the_pipe(fd[1]);

        close(fd[1]);
    }

    printf("\n-------------------------------\n\n");

    for (i = 0; i < NUM_CHILDS; i++) {
        pid = wait(&status);

        printf("Round %d winner was %d!!!\n", WEXITSTATUS(status), pid);
    }

    return 0;
}