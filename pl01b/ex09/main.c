#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>
#include <string.h>


sig_atomic_t flag = 0;


void handle_SIG(int signo){
	char msg[80];
	flag = 1;
	sprintf(msg,"Sending signal! \n");
	write(STDOUT_FILENO,msg,strlen(msg));	
}

int main(void){
	
	int i;
	time_t t;
	srand((unsigned)time(&t));
	pid_t pid;
	struct sigaction act;
	sigemptyset(&act.sa_mask); /* No signals blocked */
	act.sa_handler = handle_SIG;
	sigaction(SIGUSR1, &act, NULL);	
	
	pid = fork();
	if(pid > 0){	
		for(i = 1; i <= 3; i++){
			sleep(1);
			printf("Task A is in progress, it has passed: %d seconds \n",i);
		}
		kill(pid,SIGUSR1);
	}
	else{
		int randomNumber;
		randomNumber = rand() % 5;
		
		for(i = 1; i <= randomNumber; i++){
			sleep(1);
			printf("Task B is in progress, it has passed: %d seconds \n",i);
		}
		
		while(flag != 1){
			pause();
		}
		printf("Performing task C");
	}
}
