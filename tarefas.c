#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "tarefas.h"

int parse (char* t,char* comandos[]){

	int i=0;
	int r;
	char* comando;
	char* texto=strdup(t);
	
	comando=strtok(texto,"\n");

	r=atoi(comando);//numero de argumentos
	r--;

	comando=strtok(NULL,"\n");

	while (comando!=NULL) {
		comandos[i]=comando;
		comando=strtok(NULL,"\n");
		i++;
	} 
	comandos[i]=NULL;

	return r;
}

void atualizarHistorico(tarefa t[],int n){
	int i,status;

	for (i=0;i<n && i<MAX;i++){
		if (t[i].terminada!=0) continue;
		
		if ((waitpid(t[i].pid,&status,WNOHANG))==-1) perror("erro: wait\n");

		if (WIFEXITED(status))
			t[i].terminada=WEXITSTATUS(status);

		//if (WIFSIGNALED(status)){ //se foi terminado por um sinal
		//	t[i].terminada=WEXITSTATUS(status);
		//	printf("2: %i\n", t[i].terminada);}
	}
}


void listarTarefas(tarefa t[],int n,int x,char res[]){//if x=0 da tarefas em execucao else da tarefas terminadas 
	char str[3];
	res[0]='\0';
	int i;

	atualizarHistorico(t,n);

	if (x==0){
		strcat(res,"Tarefas em execucao:\n");
		for (i=0;i<n && i<MAX;i++){
			if (t[i].terminada==0){
				strcat(res,"#");
				intToStr(i+1,str);
				strcat(res,str);
				strcat(res,": ");
				strcat(res,t[i].argumentos);
				strcat(res,"\n");
			}
		}
	}else{
		strcat(res,"Tarefas terminadas:\n");
		for (i=0;i<n && i<MAX;i++){
			if (t[i].terminada==1){
				strcat(res,"#");
				intToStr(i+1,str);
				strcat(res,str);
				strcat(res,", concluida: ");
				strcat(res,t[i].argumentos);
				strcat(res,"\n");

			}else if (t[i].terminada==2){
				strcat(res,"#");
				intToStr(i+1,str);
				strcat(res,str);
				strcat(res,", max inatividade: ");
				strcat(res,t[i].argumentos);
				strcat(res,"\n");

			}else if (t[i].terminada==3){
				strcat(res,"#");
				intToStr(i+1,str);
				strcat(res,str);
				strcat(res,", max execucao: ");
				strcat(res,t[i].argumentos);
				strcat(res,"\n");

			}else if (t[i].terminada==4){
				strcat(res,"#");
				intToStr(i+1,str);
				strcat(res,str);
				strcat(res,", terminada por cliente: ");
				strcat(res,t[i].argumentos);
				strcat(res,"\n");

			}else if (t[i].terminada==5){
				strcat(res,"#");
				intToStr(i+1,str);
				strcat(res,str);
				strcat(res,", erro: ");
				strcat(res,t[i].argumentos);
				strcat(res,"\n");

			}
		}
	}		
}


void novaTarefa(int i,char out[]){
	out[0]='\0';
	char str[3];
	strcat(out,"Nova tarefa criada #");
	intToStr(i,str);
	strcat(out,str);
	strcat(out,"\n");
}

void intToStr(int i,char str[]){
	str[0]='\0';
	if (i<10){
		gcvt(i,1,str);
	}else if (i<100){
		gcvt(i,2,str);
	}else{gcvt(i,3,str);}
}

void parseExec (char* t,char* programa[][MAX_ARGS]){
	char* texto=strdup(t);
	char* token;
	int j=0;
	int i=0;
	
	token=strtok(texto," ");

	while (token!=NULL && i<MAX_COMMANDS && j<MAX_ARGS) {
		if (strcmp(token,"|")==0){
			programa[i][j]=NULL;
			j=0; i++; 
			token=strtok(NULL," ");
			continue;
		}
		programa[i][j++]=token;
		token=strtok(NULL," ");
	}

	programa[i][j]=NULL;
	programa[i+1][0]=NULL;

}


void mudarOUTandERR () {
	int fd = open("LOGS.txt",O_TRUNC | O_WRONLY | O_CREAT,0644);
	dup2(fd,1);
	close(fd);

	int fd_err = open("erros.txt",O_TRUNC | O_WRONLY | O_CREAT,0644);
	dup2(fd_err,2);
	close(fd_err);
}


int executarTarefa(char* arg,int t_exec,int t_ina){//executa uma tarefa
	char* programa[MAX_COMMANDS][MAX_ARGS];
	int ncomandos,i,w;//numero de comandos
	char buf[MAX];
	int pipes[MAX_COMMANDS-1][2];
	int status[MAX_COMMANDS];

	alarm(t_exec);
	signal(SIGALRM,handlerMaxExec);

	//sleep(10);

	parseExec(arg,programa);

	for (ncomandos=0;programa[ncomandos][0]!=NULL;ncomandos++);

	if (ncomandos==1){//so ha um comando -> nao precisa de pipes
		switch (fork()){
			case -1:
				perror("fork");
				_exit(5);
				return -1;
			case 0:
				execvp(programa[0][0],programa[0]);
				_exit(0);
		}
	}else{
		for (i=0;i<ncomandos;i++){
			if (i==0){
				if (pipe(pipes[i])!=0){
					perror("pipe");
					_exit(5);
				}
				switch (fork()){
					case -1:
						perror("fork");
						_exit(5);
						break;
					case 0:
						close(pipes[i][0]);

						dup2(pipes[i][1],1);
						close(pipes[i][1]);

						execvp(programa[i][0],programa[i]);
						_exit(0);
					default:
						close(pipes[i][1]);
						break;
				}
			}else if(i==ncomandos-1){
				switch (fork()){
					case -1:
						perror("fork");
						_exit(5);
						break;
					case 0:
						dup2(pipes[i-1][0],0);
						close(pipes[i-1][0]);

						execvp(programa[i][0],programa[i]);
						_exit(0);
						break;
					default:
						close(pipes[i-1][0]);
				}
			}else{
				if (pipe(pipes[i])!=0){
					perror("pipe");
					_exit(5);
					return -1;
				}
				switch (fork()){
					case -1:
						perror("fork");
						_exit(5);
						break;
					case 0:
						close(pipes[i][0]);

						dup2(pipes[i][1],1);
						close(pipes[i][1]);

						dup2(pipes[i-1][0],0);
						close(pipes[i-1][0]);

						execvp(programa[i][0],programa[i]);
						_exit(0);
						break;
					default:
						close(pipes[i][1]);
						close(pipes[i-1][0]);
				}
			}
		}
	}

	for (w=0;w<ncomandos;w++){
		wait(&status[w]);
		WEXITSTATUS(status[w]);
		if (status[w]==5) _exit(5);
	}

	return 0;
}


/*Tratamento de sinais*/

void handlerMaxExec (int sig){
	_exit(3);	
}

void handlerMaxInat (int sig){
	_exit(2);	
}



/*----cliente------*/

void paraIN(int argc,char* argv[],char* buf){
	char narg[3];
	narg[0]='\0';
	buf[0]='\0';

	intToStr(argc,narg);
	strcat(buf,narg);//a primeira linha corresponde ao numero de argumentos
	strcat(buf,"\n");

	for (int i=1;i<argc;i++){
		strcat(buf,argv[i]);
		strcat(buf,"\n"); //os argumentos ficam separados por \n
	}
}


void escreverLerFIFO(char buf[]){

	int fd_in,fd_out,n;
	char output[MAX_OUT]="";

	fd_in=open("fifo_in",O_WRONLY,O_TRUNC);
	write(fd_in,buf,strlen(buf));
	close(fd_in);

	fd_out=open("fifo_out",O_RDONLY); 
	n=read(fd_out,output,MAX_OUT);
	close (fd_out);
	write(1,&output,n);
}

void parseInput(char dest[], char src[]){

	char texto[MAX];
	char nargs[3];
	int i,j;
	int c=0;

	texto[0]='\n';
	j=1;
	for (i=0;src[i]!='\0';i++) {
		if (src[i]==' ') {
			texto[j++]='\n';
		}else if (src[i]=='\''){
			for (i+=1;src[i]!='\'' && src[i]!='\0';i++){
				texto[j++]=src[i];
			}
			break;
		}
		else texto[j++]=src[i];
	}
	texto[j]='\n';
	texto[j+1]='\0';
	i=0;
	while (i<j){//calcula o numero de argumentos
		if (texto[i]=='\n') {while(texto[i]=='\n') i++;}
		else {c++; while(texto[i]!='\n') i++;}
	}
	dest[0]='\0';
	intToStr(c+1,nargs);
	strcat(dest,nargs);
	strcat(dest,texto);

}