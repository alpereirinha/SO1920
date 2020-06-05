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
	char buf[MAX];
	buf[0]='\0';
	char output[MAX_OUT];

	//se nao ha argumentos o servidor nao tem nada para fazer
	if (argc==1) {write(1,"Sem argumentos\n",15); return 0;}
	fd_in=open("fifo_in",O_WRONLY,O_TRUNC);
	

	for (int i=1;i<3;i++){//so interessam os primeiros 2 argumentos
		strcat(buf,argv[i]);
		strcat(buf,"\n"); //os argumentos ficam separados por \n
	}

	write(fd_in,buf,strlen(buf));
	close(fd_in);

	fd_out=open("fifo_out",O_RDONLY); 
	n=read(fd_out,output,MAX);
	close (fd_out);
	write(1,&output,n);
	
	return 0;
}
