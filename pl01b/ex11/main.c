#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

volatile sig_atomic_t pid;

typedef struct {
	char cmd[32];
	int tempo;
} comando; 

void handle_ALRM(int signo){
	char msg[80];
	kill(pid, SIGKILL);
	sprintf(msg, "The proces %d didn’t end in its allowed time! \n", pid);
	write(STDOUT_FILENO,msg,strlen(msg));
}

int main() {
	int i;
	struct sigaction act;
	
	memset(&act, 0, sizeof(struct sigaction));
	sigemptyset(&act.sa_mask); /* No signals blocked */
	act.sa_handler = handle_ALRM;
	sigaction(SIGALRM, &act, NULL);
	
	comando c[3];
	strcpy(c[0].cmd, "./cmd1");
	strcpy(c[1].cmd, "./cmd2");
	strcpy(c[2].cmd, "./cmd3");

	c[0].tempo = 6;
	c[1].tempo = 0;
	c[2].tempo = 4;

	
	for (i = 0; i < 3; i++){
		pid = fork();
		if(pid == 0){
			execlp(c[i].cmd, c[i].cmd, NULL);
		}
		alarm(c[i].tempo);
		waitpid(pid,NULL,0);
	}
}
