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
	
	int i,n,fd_in,fd_out,c;
	int t_exec,t_ina;t_exec=t_ina=0;
	int q = 1; //terminar servidor
	char buf[MAX];
	char output[MAX_OUT];
	char* comandos[MAX_COMMANDS];
	//informacao sobre as tarefas efetuadas
	int ntarefas=0;//numero de tarefas
	tarefa historico[MAX];

	mkfifo("fifo_out",0666);//depois de analisar o input escreve no fifo output para o cliente escrever
	mkfifo("fifo_in",0666);//cria fifo input e espera o cliente
	do{
		
		fd_in=open("fifo_in",O_RDONLY); 
		n = read(fd_in,buf,MAX);
		close(fd_in);
		
		c=parse(buf,comandos);

		if (strcmp(comandos[0],"-i")==0){//tempo de inatividade
			if ((t_ina=atoi(comandos[1]))<=0){strcpy(output,"Argumento dado invalido.\n");}
			else{strcpy(output,"Tempo maximo de inatividade alterado.\n");}

		}else if(strcmp(comandos[0],"-m")==0){//tempo de execucao
			if ((t_exec=atoi(comandos[1]))<=0){strcpy(output,"Argumento dado invalido.\n");}
			else{strcpy(output,"Tempo maximo de execucao alterado.\n");}

		}else if(strcmp(comandos[0],"-e")==0){//executar uma tarefa
			executarTarefa(comandos[1],&historico[ntarefas%MAX],t_exec,t_ina);
			historico[ntarefas%MAX].argumentos=strdup(comandos[1]);//quando chega ao final do array
			historico[ntarefas%MAX].terminada=0;				   //volta ao inicio
			novaTarefa(++ntarefas%MAX,output);
	 
		}else if(strcmp(comandos[0],"-l")==0){//listar tarefas em execucao
			strcpy(output,listarTarefas(historico,ntarefas,0));

		}else if(strcmp(comandos[0],"-t")==0){//terminar uma tarefa em execucao
			n=atoi(comandos[1]);
			n-=1;//valor real
			if (c<=1 || n<0 || n>ntarefas%MAX)
				strcpy(output,"Argumento dado invalido.\n");			
			else if (historico[n].terminada==0){
				//kill(historico[n].pid,SIGKILL);
				strcpy(output,"Tarefa foi terminda com sucesso?\n");
			}else{
				strcpy(output,"Tarefa ja terminou.\n");
			}

		}else if(strcmp(comandos[0],"-r")==0){//historico de tarefas terminadas
			strcpy(output,listarTarefas(historico,ntarefas,1));

		}else if(strcmp(comandos[0],"-h")==0){//ajuda
			strcpy(output,"Ajuda.\n");
		
		}else if(strcmp(comandos[0],"quit")==0){
			strcpy(output,"O servidor terminou.\n");
			q=0;
		
		}else{
			strcpy(output,"Opcao invalida.\n");
		}

		
		fd_out=open("fifo_out",O_WRONLY); 		 
		write(fd_out,&output,strlen(output));	
		close(fd_out);	

	}while(q);

	return 0;
}
