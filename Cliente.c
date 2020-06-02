#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "tarefas.h"

int main (int argc, char* argv[]){
	
	int n,fd_in,fd_out;
	char buf[MAX]="";
	char output[MAX_OUT];
	
	//se nao ha argumentos o servidor nao tem nada para fazer
	if (argc==1) {write(1,"Sem argumentos\n",15); return 0;}
	mkfifo("fifo_in",0666);//cria fifo input e espera o cliente
	fd_in=open("fifo_in",O_WRONLY);
	
	for (int i=1;i<argc;i++){
		strcat(buf,argv[i]);
		strcat(buf,"\n"); //os argumentos ficam separados por \n
	}
	write(fd_in,buf,strlen(buf));
	close(fd_in);

	mkfifo("fifo_out",0666);//depois de analisar o input escreve no fifo output para o cliente escrever
	fd_out=open("fifo_out",O_RDONLY); 
	n=read(fd_out,output,MAX);
	close (fd_out);
	write(1,&output,n);
	
	return 0;
}
