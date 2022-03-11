#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

#define ARRAY_SIZE 1000

int main (){
	int numbers[ARRAY_SIZE]; /* array to lookup */
	int n; /* the number to find */
	time_t t; /* needed to init. the random number generator (RNG)*/
	int i;

	/* intializes RNG (srand():stdlib.h; time(): time.h) */
	srand ((unsigned) time (&t));

	/* initialize array with random numbers (rand(): stdlib.h) */
	for (i = 0; i < ARRAY_SIZE; i++)
	numbers[i] = rand () % 10000;

	/* initialize n */
	n = rand () % 10000;
	
	int result;
	result = 0;
	int status;
	
	if(fork() > 0){
		
		for( i = 0; i < ARRAY_SIZE/2;i++){
			if(numbers[i] == n) result++;
		}
		wait(&status);
		printf("The number %d, was found %d times\n",n,WEXITSTATUS(status)+result);
	}
	else {
		for(i = ARRAY_SIZE/2 ; i < ARRAY_SIZE;i++){
			if(numbers[i] == n) result++;
		}
		exit(result);
	}
	
	
	
	
	
 } 
