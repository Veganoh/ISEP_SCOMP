#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>


void handle_USR1(int signo,siginfo_t *sinfo, void *context){
	
	char msg[80];
	sprintf(msg,"SIGUSR1 has been captured and was sent by the process with PID %d \n",sinfo->si_pid);
	write(STDOUT_FILENO,msg,strlen(msg));
}


int main(){
	struct sigaction act;
	
	/* Limpar a variável act */
	memset(&act,0,sizeof(struct sigaction));
	sigemptyset(&act.sa_mask); /* Remove sinais bloqueados */
	act.sa_sigaction = handle_USR1;
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1,&act,NULL);


	for(;;){
		printf("O Cristóvão é o rei\n");
		sleep(1);
	}

}

/*
Perguntas:
 a) O mais apropriado para lidar com sinais é o SIGACTION, porque permite adaptar
o funcionamento do programa ao sinal recebido.
 
 b) 
	SIG - O número do sinal que foi recebido;
	ACT - Define os detalhes de ação para o sinal recebido;
	OACT - Se não for null, é usada para guardar os detalhes da ação previamente definida.
 
 d) Bloqueie a entrega de sinal no programa principal ao chamar funções que não são seguros ou operam em dados globais que também são acessados ​​por o manipulador de sinais.
  
 e) * man signal-safety *
*/
