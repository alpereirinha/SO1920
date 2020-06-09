#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "tarefas.h"

int main(int argc, char *argv[]){
	
	int i,n,fd_in,fd_out,c,pid;
	int t_exec,t_ina;t_exec=t_ina=0;
	int q = 1; //terminar servidor
	char buf[MAX];
	char output[MAX_OUT];
	char* comandos[MAX_COMMANDS];
	//informacao sobre as tarefas efetuadas
	int ntarefas=0;//numero de tarefas
	tarefa historico[MAX];

	mudarOUTandERR();

	mkfifo("fifo_out",0666);//depois de analisar o input escreve no fifo output para o cliente escrever
	mkfifo("fifo_in",0666);//cria fifo input e espera o cliente
	
	do{

		fd_in=open("fifo_in",O_RDONLY); 
		n = read(fd_in,buf,MAX);
		close(fd_in);
		
		c=parse(buf,comandos);

		if (strcmp(comandos[0],CMD_TEMP_INATIV_LONG)==0 || strcmp(comandos[0],CMD_TEMP_INATIV)==0){//tempo de inatividade

			if (c<2) {strcpy(output,"Argumento dado invalido.\n");}
			else{
				t_ina=atoi(comandos[1]);
				if (t_ina<0){
					strcpy(output,"Argumento dado invalido.\nTempo maximo de inatividade alterado para 0.\n");
					t_ina=0;
				}else{strcpy(output,"Tempo maximo de inatividade alterado.\n");}
			}
		}else if(strcmp(comandos[0],CMD_TEMP_EXEC_LONG)==0 || strcmp(comandos[0],CMD_TEMP_EXEC)==0){//tempo de execucao
			if (c<2) {strcpy(output,"Argumento dado invalido.\n");}
			else{
				t_exec=atoi(comandos[1]);
				if (t_exec<0){
					strcpy(output,"Argumento dado invalido.\nTempo maximo de execucao alterado para 0.\n");
					t_exec=0;
				}
				else{strcpy(output,"Tempo maximo de execucao alterado.\n");}
			}
		}else if(strcmp(comandos[0],CMD_EXECUTAR_LONG)==0 || strcmp(comandos[0],CMD_EXECUTAR)==0){//executar uma tarefa
			if (c<2) {strcpy(output,"Argumento dado invalido.\n");}
			else{
				historico[ntarefas%MAX].terminada=0;
				historico[ntarefas%MAX].argumentos=strdup(comandos[1]);
				switch (pid=fork()){
					case -1:
						perror("fork");
						break;
					case 0:
						n=executarTarefa(comandos[1],t_exec,t_ina);					   
						exit(n);
				}
				historico[ntarefas%MAX].pid=pid;
				novaTarefa(++ntarefas%MAX,output);
			}
	 
		}else if(strcmp(comandos[0],CMD_LIST_LONG)==0 || strcmp(comandos[0],CMD_LIST)==0){//listar tarefas em execucao
			listarTarefas(historico,ntarefas,0,output);

		}else if(strcmp(comandos[0],CMD_TERMINAR_LONG)==0 || strcmp(comandos[0],CMD_TERMINAR)==0){//terminar uma tarefa em execucao
			atualizarHistorico(historico,ntarefas);
			n=atoi(comandos[1]);//da 0 no caso da string ser invalida
			n-=1;//valor real
			if (c<=1 || n<0 || n>=ntarefas)
				strcpy(output,"Argumento dado invalido.\n");			
			else if (historico[n].terminada==0){
				if (kill(historico[n].pid,SIGUSR1)==-1){
					perror("erro: terminar\n");
				}
				else strcpy(output,"Tarefa foi terminda.\n");
			}else{
				strcpy(output,"Tarefa ja terminou.\n");
			}

		}else if(strcmp(comandos[0],CMD_HISTORY_LONG)==0 || strcmp(comandos[0],CMD_HISTORY)==0){//historico de tarefas terminadas
			listarTarefas(historico,ntarefas,1,output);

		}else if(strcmp(comandos[0],CMD_AJUDA_LONG)==0 || strcmp(comandos[0],CMD_AJUDA)==0){//ajuda
			strcpy(output,"tempo-inatividade segs\ntempo-execucao segs\nexecutar p1 | p2 ... | pn\nlistar\nterminar n\nhistorico\najuda\nquit");
		
		}else if(strcmp(comandos[0],CMD_QUIT)==0){
			strcpy(output,"O servidor terminou.\n");
			q=0;
		
		}else{
			strcpy(output,"Opcao invalida.\n");
		}

		fd_out=open("fifo_out",O_WRONLY,O_TRUNC); 		 
		write(fd_out,&output,strlen(output));	
		close(fd_out);	

	}while(q);

	return 0;
}
