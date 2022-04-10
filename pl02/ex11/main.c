#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

#define READ 0
#define WRITE 1

int main(void){
	
	int i,generatedValue,buffer,pipes[6][2];
		
	for(i = 0; i < 6; i++){
		if(pipe(pipes[i]) == -1){
			perror("Pipe failed");
			return 1;
		}
	}
	
	for(i = 0; i < 5; i++){	
		if ( fork() == 0){
			close(pipes[i][WRITE]);
			read(pipes[i][READ],&buffer,sizeof(int));
			srand ((getpid()));
			generatedValue = (rand() % 501) + 1;
			printf("Me with the PID %d, generated the value: %d \n",getpid(),generatedValue);
			
			if(generatedValue > buffer){
				write(pipes[i+1][WRITE],&generatedValue,sizeof(int));
			} else {
				write(pipes[i+1][WRITE],&buffer,sizeof(int));
			}
			close(pipes[i+1][WRITE]);
			exit(0);		
		}
	}
	
	srand((unsigned)time(NULL) * getpid());
    generatedValue = rand() % (1 + 500) + 1;
    printf("Me with the PID %d, generated the value: %d \n", getpid(), generatedValue);
    close(pipes[0][READ]);
    close(pipes[5][WRITE]);
	write(pipes[0][WRITE],&generatedValue,sizeof(int));
	close(pipes[0][WRITE]);
	read(pipes[5][READ],&generatedValue,sizeof(int));
	close(pipes[5][READ]);
	printf("The biggest number is: %d\n",generatedValue);
    return 0;
}
