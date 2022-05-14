#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>

#define NUM_CHILD 10

int main() {
    int randomArray[2000]; 
    srand(time(NULL));
    int n = rand() % 100;
    int i;
    int j;
    int status;
    pid_t pid;

    for (i = 0; i < 2000; i++)
        randomArray[i] = rand() % 100;

    printf("The random number selected was: %d\n", n);

    for (i = 0; i < NUM_CHILD; i++) {
        if (fork() == 0) {
            for (j = i * 100; j < (i + 2) * 100; j++) 
                if (randomArray[j] == n)
                    exit(j - (i * 100));

            exit(255);
        }
    }

    for (i = 0; i < NUM_CHILD; i++) {
        pid = wait(&status);

        if (WIFEXITED(status))
            printf("O processo %d terminou com o valor: %d\n", pid, WEXITSTATUS(status));
    }

    return 0;
}