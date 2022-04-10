#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct {
    int credit;
    int bet;
} data_b;

int main() {
    int fd1[2], fd2[2];
    data_b bet_info;
    int number;
    int stat = 1;

    if(pipe(fd1) == -1 || pipe(fd2) == -1) {
        perror("Pipe failed!");
        exit(1);
    }

    if (fork() == 0) {
        int winnings;
        bet_info.credit = 20;

        close(fd1[1]);
        close(fd2[0]);
        srand(getpid());

        printf("My starts credit at: %d€\n--------------------------------\n", bet_info.credit);

        while (1) {
            if(!stat) {
                close(fd1[0]);
                close(fd2[1]);
                exit(0);
            } 

            bet_info.bet = rand() % 5 + 1;

            write(fd2[1], &bet_info, sizeof(data_b));
            read(fd1[0], &bet_info.credit, sizeof(int));

            printf("My credit is: %d€\n\n", bet_info.credit);

            read(fd1[0], &stat, sizeof(int));
        }
    } else {
        close(fd1[0]);
        close(fd2[1]);
        srand(getpid());

        while(read(fd2[0], &bet_info, sizeof(data_b))) {
            number = rand() % 5 + 1;

            if (bet_info.bet == number) {
                bet_info.credit += 10;
            } else {
                bet_info.credit -= 5;
            }

            printf("Bet: %d | Actual: %d\n", bet_info.bet, number);

            write(fd1[1], &bet_info.credit, sizeof(int));

            if (bet_info.credit <= 0) {
                stat = 0;
            } 

            write(fd1[1], &stat, sizeof(int));
        }

        close(fd1[1]);
        close(fd2[0]);
    }

    return 0;
}