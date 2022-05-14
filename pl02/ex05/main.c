#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#define MAX_SIZE 256

int main(void){
	
	char writeMsg[MAX_SIZE];
	char readMsg[MAX_SIZE];
	int up[2];
	int down[2];
	
	pid_t pid;
	
	if(pipe(up) == -1 || pipe(down) == -1){
		perror("Pipe failed");
		return 1;
	}
	
	pid = fork();
	
	if(pid > 0){
		close(up[1]);
		read(up[0],readMsg,MAX_SIZE);
		printf("O servidor leu: %s \n",readMsg);
		close(up[0]);
		
		for(int i = 0; i < strlen(readMsg);i++){
			if(!isupper(readMsg[i])){
				readMsg[i] = toupper(readMsg[i]);
			}
		}
		
		close(down[0]);
		write(down[1],readMsg,strlen(readMsg)+1);
		close(down[1]);	
		
	}
	else{
		close(up[0]);
		printf("Cliente, insere a tua mensagem:\n");
		scanf("%[^\n]s",writeMsg);
		write(up[1],writeMsg,strlen(writeMsg)+1);
		close(up[1]);	
		
		close(down[1]);
		read(down[0],readMsg,MAX_SIZE);
		close(down[0]);
		printf("A mensagem recebida pelo servidor foi: %s \n",readMsg);
	}
	
	
	
	
	
}
