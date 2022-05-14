#include <stdio.h>
#include <unistd.h>

int main() {
    for(;;) {
        printf("I love SCOMP!\n");
        sleep(1);
    }
}

/*
a) adicionar as diferentes bibliotecas necessárias

b) SIGSTOP

    1. $ ps a (ter a lista dos PID dos diferentes processos)
    2. $ kill -SIGSTOP [pid] (Suspender o processo)

c) SIGCONT
    1. $ kill -SIGCONT [pid] (Retomar o processo)

d) SIGKILL
    1. $ kill -SIGKILL [pid] (Terminar o processo)

*/
