#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>

#define READ 0
#define WRITE 1
#define NUMBER_OF_CHILDREN 10

int spawn_children(int n);

int main(){
	
	int array[1000],fd[2],i,j,max;
	time_t t;
    srand((unsigned)time(&t));
    
    for(i = 0; i < 1000; i++){
		array[i] = (rand() % 1000);
	}
	
	if(pipe(fd) == -1){
		perror("Pipe failed");
		return 1;
	}
	
	int id = spawn_children(NUMBER_OF_CHILDREN);
	if(id > 0){
		close(fd[READ]);
		max = 0;
		for (j = (id-1) * 100; j < id * 100; j++){
			if(max < array[j]) max = array[j];
		}
		printf("[NUMBER %d] %d\n",(id-1), max);			
		write(fd[WRITE], &max, sizeof(max));
		close(fd[WRITE]);
		exit(EXIT_SUCCESS);
	}
	
	int shared[10];
	close(fd[WRITE]);
	for(i = 0; i < 10; i++){
		wait(NULL);
		read(fd[READ], &shared[i], sizeof(int));
	} 
	
	max = 0;
	for(i = 0; i < 10; i++){
		if(max < shared[i]) max = shared[i];
	} 
	
	printf("[BIGGEST NUMBER] %d\n", max);
	exit(EXIT_SUCCESS);	
}

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

