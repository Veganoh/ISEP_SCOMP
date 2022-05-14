#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>

typedef struct{
	char string1[20];
	char string2[20];
} myStrings;

int main(void){
	myStrings writeStruct,readStruct;
	int fd[2];
	int status;
	
	if(pipe(fd) == -1){
		perror("Pipe failed");
		return 1;
	}
	
	pid_t pid = fork();
	
	if(pid > 0){
		close(fd[0]);
		strcpy(writeStruct.string1, "Hello World");
		strcpy(writeStruct.string2, "Goodbye!");

		write(fd[1],&writeStruct,sizeof(myStrings)+1);
		close(fd[1]);
		wait(&status);
		
		printf("My son pid is: %d and it's status is %d \n",pid,WEXITSTATUS(status));
	} else {
		close(fd[1]);
		read(fd[0],&readStruct,sizeof(myStrings));
		close(fd[0]);
		printf("%s \n",readStruct.string1);
		printf("%s \n",readStruct.string2);
		exit(10);
	}
}
