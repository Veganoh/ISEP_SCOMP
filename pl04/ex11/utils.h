#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h>


/**
 * SEMAPHORES UTILS
 * */
 
 void close_semaphore(sem_t *sem){
	if(sem_close(sem) == -1){
		perror("Error at sem_close()!\n");
		exit(EXIT_FAILURE);
    }
 }
 
 void unlink_semaphore(char* sem_name){
	if(sem_unlink(sem_name) == -1){
		perror("Error at sem_unlink()!\n");
        exit(EXIT_FAILURE);
    }
 }

/**
 * PROCESSES UTILS
 * */
int spawn_children(int n){
	pid_t pid;
    int i;
    for (i = 0; i < n; i++){
		pid = fork();
        if (pid < 0)
            return -1;
        else if (pid == 0)
            return i + 1;
    }
    return 0;
}

void wait_for_children(int n){
	int i;
	for(i = 0; i < n; i++){
		wait(NULL);
	}
}

