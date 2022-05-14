#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>


void handle_SIG(int signo){
	
	char msg[80];
	sigset_t mask;
	sigprocmask(SIG_BLOCK,NULL,&mask);
	
	 for (int i = 1; i <= 64; i++){
        if(!sigismember(&mask, i) && (i != 33 && i != 32)){
            sprintf(msg, "Unblocked signal %d\n", i);
            write(STDOUT_FILENO, msg, strlen(msg));
        }
    }
}

int main(){
	
	struct sigaction act;
	
	memset(&act, 0, sizeof(struct sigaction));
	sigfillset(&act.sa_mask); /* All signals blocked */
		
	act.sa_handler = handle_SIG;
	sigaction(SIGUSR1, &act, NULL);

	for(;;){
		printf("I Like Signal\n");
		sleep(1);
	}

	


}
