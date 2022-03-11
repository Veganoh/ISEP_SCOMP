#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(void) {
    int a=0, b, c, d;
    b = (int) fork();
    c = (int) getpid(); /* getpid(), getppid(): unistd.h*/
    d = (int) getppid();
    a = a + 5;
    printf("\na=%d, b=%d, c=%d, d=%d\n",a,b,c,d);
}

/*
    a) It will be the variable 'a';
    
    b) 

    P1
    |
    |int a = 0
    |
    |
    |
    | b = fork()            
    +-------------------+ P2
    |                   |
    |c = getpid()       | c = gitpid()
    |d = getppid()      | d = getppid()
    |a = a + 5          | a = a + 5
    | printf("...")     | printf("...")
*/