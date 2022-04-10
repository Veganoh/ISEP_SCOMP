#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>


int main(void) {
    pid_t p;

    printf("I'm..\n");
    p = fork();

    if (p == 0) {
        printf("I will never join you!\n");
        exit(0);
    }

    printf("the..\n");
    p = fork();

    if (p == 0) {
        printf("I will never join you!\n");
        exit(0);
    }

    printf("father!\n");
    p = fork();

    if (p == 0) {
        printf("I will never join you!\n");
        exit(0);
    }

    return 0;
}