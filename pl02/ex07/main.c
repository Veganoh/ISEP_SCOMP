#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

#define ARRAY_SIZE 1000

int main (void){
	int arr1[ARRAY_SIZE],arr2[ARRAY_SIZE],result[ARRAY_SIZE],pipes[5][2]; 
	time_t t;
	int i,j,k=0,x;
	srand ((unsigned) time (&t));

	for (i = 0; i < ARRAY_SIZE; i++){
	arr1[i] = rand () % 10000;
	arr2[i] = rand () % 10000;
	}
	
	for(i = 0; i < 5; i++){
		if(pipe(pipes[i]) == -1){
			perror("Pipe failed");
			return 1;
		}
	}
	
	for(i = 0; i < 5;i++){
		
		if(fork() == 0){
			
			close(pipes[i][0]);
			
			for(j = i * 200; j < ((i+1) * 200);j++){
				x = arr1[j] + arr2[j];
				write(pipes[i][1],&x,sizeof(x));
			}
			close(pipes[i][1]);

			exit(0);
		}
	}
	
	for(i = 0; i < 5;i++){
		
		close(pipes[i][1]);
		
		for(j = 0; j < 200; j++){
				read(pipes[i][0],result+k,sizeof(int));
				k++;
		}
		close(pipes[i][0]);
	}
	
	for(i = 0; i < 1000; i++){
		printf(" %d + %d = %d \n",arr1[i],arr2[i],result[i]);	
	}
}
