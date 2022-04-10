#include <stdio.h>
#include <unistd.h>

int main(){
	int readMsg,writeMsg;
	int fd[2];

	
	if(pipe(fd) == -1){
		perror("Pipe failed");
		return 1;
	}
	
	writeMsg = fork();
	
	if(writeMsg > 0){
		close(fd[0]);
		write(fd[1],&writeMsg,sizeof(writeMsg)+1);
		close(fd[1]);
	}else{
		close(fd[1]);
		read(fd[0],&readMsg,sizeof(readMsg));
		printf("Filho leu: %d", readMsg);
		close(fd[0]);
	}
		
}

