#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"

#define MAX_COMMANDS 100
#define MAX 1024
#define NR_MAX_COMANDOS 30



// enum resposta {OK, ERRO}
// typedef struct {
// 	enum resposta r;
// 	char* conteudo;
// } resp;

typedef struct {
	int tempo_inatividade;
} definicoes;

int main(int argc, char const *argv[]){
	
	int n,fd_in,fd_out;
	st fds;
	char buf[MAX];
	definicoes def;

	// mkfifos devem ser feitos aqui
	// o servidor tem que abrir o seu pipe de leitura para escrita tbm
	// pq quando um server read pipe fica sem cliente do outro lado deixa de ser bloqueante (retorna logo resposta)
	

	// os opens dos named pipes devem ser feitos antes de entrar num ciclo de read, parse, write
	do{
		mkfifo("fifo_in",0666);//cria fifo input e espera o cliente
		printf("flag");
		fd_in=open("fifo_in",O_RDONLY);
		printf("flag1");
		// le o primeiro argumento e continua
		// SOLUCAO:
		// criar uma estrutura que contenha a informação toda necessária para o server saber o que tem de fazer
		// criar uma mensagem fixa

		n = read(fd_in, fds, sizeof(st));
		
		if (fds.isTempoDeInatividade) {
			def.tempo_inatividade = fds.argTempoDeInatividade;
		} else if (fds.isExecutar) {
			fds.argExecutar;

		}
		// int pos = 0;
		// while ((n = read(fd_in,buf,MAX))) {
		// 	strcpy(commands[pos++], buf);
		// }

		/*faz algo com u input*/
		// parse switch case


		
		mkfifo("fifo_out",0666);//depois de analisar o input escreve no fifo output para o cliente escrever
		fd_out=open("fifo_out",O_WRONLY);  			 
		write(fd_out,&buf,n);					
	}while(fds.isTernminarExecucao);
	// fechar pipes
	return 0;
}