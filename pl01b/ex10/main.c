#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define CHAR_LIM 50

void handle_ALARM(int signo) {
    write(STDOUT_FILENO, "\nToo slow, that is why the program will end!\n", 45);
    exit(0);
}

int main() {
    char str[CHAR_LIM];
    struct sigaction act;
    sigset_t block_alarm_mask;
    int num_char;

    memset(&act, 0, sizeof(struct sigaction));
    sigemptyset(&act.sa_mask);
    act.sa_handler = handle_ALARM;
    sigaction(SIGALRM, &act, NULL);

    sigemptyset(&block_alarm_mask);
    sigaddset(&block_alarm_mask, SIGALRM); //Setting up a mask to block the alarm signal
    
    printf("Input a message: ");
    alarm(10);
    scanf("%[^\n]%*c", str);

    sigprocmask(SIG_BLOCK, &block_alarm_mask, 0); //Blocks the alarm signal

    num_char = strlen(str);
    printf("Number of characters in that message: %d\n", num_char);
    printf("Working in important stuff now...\n");
    sleep(20);
    
    return 0;
}