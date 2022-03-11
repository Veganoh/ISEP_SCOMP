#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>


int main(void) {
	
	
	pid_t p;
	
	printf("Dad talking for the first time\n");
	p = fork();
	
	if (p == 0){
		printf("I'm his first child\n");
		exit(0);
	}
	
	printf("Dad talking for the second time\n");
	p = fork();
	
	if (p == 0){
		printf("I'm his second child\n");
		exit(0);
	}
	
	printf("Dad talking for the third time\n");
	p = fork();
	
	if (p ==0){
		printf("I'm his third child\n");
		exit(0);
	}
	
	return 0;
}


