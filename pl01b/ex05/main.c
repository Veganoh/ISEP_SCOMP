#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>


void handle_SIG(int signo){
	
	char msg[80];
	
	switch(signo)
	{
		case SIGINT:
			sprintf(msg,"I won’t let the process end with CTRL-C! \n");
			break;
		
		case SIGQUIT:
			sprintf(msg,"I won't let the process end with CTRL-\\! \n");
			break;
	}
	write(STDOUT_FILENO,msg,strlen(msg));
}

int main(){
	
	struct sigaction act;
	memset(&act, 0, sizeof(struct sigaction));
	sigemptyset(&act.sa_mask); /* No signals blocked */
	act.sa_handler = handle_SIG;
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGQUIT,&act, NULL);

	for(;;){
		printf("I Like Signal\n");
		sleep(1);
	}

	


}

/**
Perguntas:
  a) CTRL-C é uma combinação de teclas que causa uma interrupção de hardware, esta combinação
 envia o sinal SIGINT que termina o processo causando a sua interrupção.
  
  c) (Não está a funcionar)
  
  e) Ao dar o SIGINT acontece o mesmo que se dessemos CTRL-C, o mesmo para SIGQUIT se dessemos CTRL-\
   
  g) Não se deve usar o print porque ele ua uma variavel global, e os handlers podem estar a dar update as variaveis globais ao mesmo tempo que estão a ser utilizadas
 
*/
