#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>


int main(void) {
	

	fork();
	fork();
	fork();
	printf("SCOMP\n");	
}


/**
	a) 8 process will be created
	c) SCOMP is printed 8 times
 */ 

 
  
  
