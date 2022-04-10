#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

volatile sig_atomic_t USR1_COUNTER = 0;

void handle_USR1(int signo, siginfo_t *sinfo, void *context) {
    char msg[70];

    USR1_COUNTER++;
    sprintf(msg, "SIGUSR1 signal captured, USR1_COUNTER = %d\n", USR1_COUNTER); 

    write(STDOUT_FILENO, msg, strlen(msg));

    sleep(1);
}

int main() {
    struct sigaction act;

    memset(&act, 0, sizeof(struct sigaction));
    sigemptyset(&act.sa_mask);
    act.sa_sigaction = handle_USR1;
    act.sa_flags = SA_SIGINFO;
    act.sa_flags = SA_NODEFER;

    sigaction(SIGUSR1, &act, NULL);

    pid_t pid = fork();

    if (pid == 0) {
        int i;
        pid_t ppid = getppid();
        struct timespec time1, time2;

        for (i = 0; i < 12; i++) {
            time1.tv_sec = 0;
            time1.tv_nsec = 10000000L;

            nanosleep(&time1, &time2);

            kill(ppid, 10);
        }
    } else {
        for(;;) {
            printf("I'm working!\n");
            sleep(1);
        }
    }
    
    return 0;
}