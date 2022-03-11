#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


int main(void) {
	
	int status,i;
	
	for(i = 0; i < 2; i++){
		
		if (fork() == 0){
			
			switch(i){
			
			case 0:
				sleep(1);
				exit(1);
			break;
			
			case 1:
				sleep(2);
				exit(2);
			break;
			
			}
		}			
	}
	
	for(i = 0; i < 2; i++){
		wait(&status);
		printf("A child has died with the exit status: %d\n",WEXITSTATUS(status));	
	
	}
	return 0;
}
