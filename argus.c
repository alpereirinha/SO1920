#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "argus.h"

int main (int argc, char* argv[]){
	
	int n,q=1;
	char buf[MAX];
	buf[0]='\0';
	char str[MAX];

	if (argc==1) {
		do{
			write(1,"argus$ ",7);
			n=read(0,&str,MAX);
			str[n-1]='\0';
			if (strcmp(str,CMD_QUIT)==0) {q=0; continue;}

			parseInput(buf,str);

			escreverLerFIFO(buf);

		}while (q);
		write(1,"Saiu do terminal da aplicacao com sucesso.\n"
				"Para terminar o servidor use a opcao quit.\n",86);
		return 0;
	}

	paraIN(argc,argv,buf);

	escreverLerFIFO(buf);

	
	return 0;
}
