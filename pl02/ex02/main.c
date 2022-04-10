#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define MAX_LIMIT 30

int main() {
    int fd[2];
    pid_t p;
    char str[MAX_LIMIT]; 
    int number; 

    if (pipe(fd) == -1) {
        perror("Pipe failed!");
        return 1;
    }

    p = fork();

    if (p != 0) {
        close(fd[0]);

        printf("String: ");
        scanf("%[^\n]%*c", str);
        write(fd[1], str, strlen(str) + 1);

        sleep(0.5); //Sleep for the "Int: " message to only appear after the child usually prints the string received from the pipe

        printf("Int: ");
        scanf("%d", &number);
        write(fd[1], &number, sizeof(int));
        
        close(fd[1]);
    } else {
        close(fd[1]);

        read(fd[0], str, MAX_LIMIT);
        printf("Child print string: %s\n", str);

        read(fd[0], &number, sizeof(int));
        printf("Child print number: %d\n", number);        

        close(fd[0]);
    }

    return 0;
}