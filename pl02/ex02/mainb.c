#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define MAX_LIMIT 30

typedef struct PipeData{
    int number;
    char str[MAX_LIMIT];
} PipeData;

int main() {
    int fd[2];
    pid_t p;
    PipeData pd;

    if (pipe(fd) == -1) {
        perror("Pipe failed!");
        return 1;
    }

    p = fork();

    if (p != 0) {
        close(fd[0]);

        printf("String: ");
        scanf("%[^\n]%*c", &pd.str);

        printf("Int: ");
        scanf("%d", &pd.number);

        write(fd[1], &pd, sizeof(pd));

        close(fd[1]);
    } else {
        close(fd[1]);

        read(fd[0], &pd, sizeof(PipeData));
        printf("Child print string: %s\n", pd.str);
        printf("Child print number: %d\n", pd.number); 

        close(fd[0]);
    }

    return 0;
}

