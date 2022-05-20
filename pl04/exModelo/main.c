#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <time.h>
#include <stdlib.h>

#define VOTADORES 505

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

void wait_for_children(int n){
	int i;
	for(i = 0; i < n; i++){
		wait(NULL);
	}
}


int main(){
	
	int id = spawn_children(VOTADORES);
	if(id > 0){
		execlp("./votaSocio","./votaSocio",(char*)NULL);
		exit(EXIT_SUCCESS);
	}
	
	execlp("./contaVotos","./contaVotos",(char*)NULL);
	
    return 0;
}
