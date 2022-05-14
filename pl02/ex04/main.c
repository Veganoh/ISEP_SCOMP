#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#define FILE_NAME "text.txt"
#define STR_MAX_SIZE 100

int main() {
    int fd[2];
    pid_t pid;
    int status;
    char line[STR_MAX_SIZE];
    FILE *filePtr;

    if(pipe(fd) == -1) {
        perror("Pipe failed!");
        exit(1);
    }

    pid = fork();

    if (pid > 0) {
        close(fd[0]);

        if((filePtr = fopen(FILE_NAME, "r")) == NULL) {
            printf("Failed to open the file!");
            exit(1);
        }

        while (NULL != fgets(line, STR_MAX_SIZE, filePtr))
            write(fd[1], &line, sizeof(line) + 1);

        fclose(filePtr);
        close(fd[1]);
        wait(&status);
        
    } else {
        close(fd[1]);

        while (read(fd[0], &line, sizeof(line) + 1) != 0) 
            printf("%s", line);

        close(fd[0]);
        printf("\n----------------\nChild finished!");
        exit(0);
    }

    printf("\nParent finished!\n");
    return 0;
}