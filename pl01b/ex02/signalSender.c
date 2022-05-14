#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int main() {
    int pid, sinal;

    printf("Qual o pid do processo?\n");
    scanf("%d", &pid);

    // Alinea e)

    printf("Qual o sinal que quer mandar?\n");
    scanf("%d", &sinal);

    kill(pid, sinal);
    
    // Alinea f)
    /*
    kill(pid, 19);
    sleep(2);
    kill(pid, 18);
    sleep(2);
    kill(pid, 9);
    */
    
    return 0;
}