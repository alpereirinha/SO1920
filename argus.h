#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COMMANDS 100
#define MAX_ARGS 30
#define MAX 256
#define MAX_OUT 2048

//Comandos
#define CMD_TEMP_INATIV_LONG "tempo-inatividade"
#define CMD_TEMP_INATIV "-i"
#define CMD_TEMP_EXEC_LONG "tempo-execucao"
#define CMD_TEMP_EXEC "-m"
#define CMD_EXECUTAR_LONG "executar"
#define CMD_EXECUTAR "-e"
#define CMD_LIST_LONG "listar"
#define CMD_LIST "-l"
#define CMD_TERMINAR_LONG "terminar"
#define CMD_TERMINAR "-t"
#define CMD_HISTORY_LONG "historico"
#define CMD_HISTORY "-r"
#define CMD_AJUDA_LONG "ajuda"
#define CMD_AJUDA "-h"
#define CMD_QUIT "quit"


typedef struct tarefas
{
	char* argumentos;
	int terminada;//0 -> ainda nao terminou || 1 -> ja terminou com sucesso || 
				  //2 -> terminou por max inativdade || 3 -> terminou por max execucao
				  //4 -> terminada por cliente || 5 -> terminou com erro
	int pid;
}tarefa;		  
				  

int parse (char* texto,char* comandos[]);
void listarTarefas(tarefa t[],int n,int x,char res[]);
void novaTarefa(int i,char res[]);
void intToStr(int i,char str[]);
int executarTarefa(char* arg,int t_exec,int t_ina);
void parseExec (char* t,char* programa[][MAX_ARGS]);
void mudarOUTandERR ();
void atualizarHistorico(tarefa t[],int n);
void handlerMaxExec(int sig);
void handlerKill (int sig);
void handlerMaxInat (int sig);


/*funcoes para cliente*/

void paraIN(int argc,char* argv[],char* buf);
void escreverLerFIFO(char buf[]);
void parseInput(char* dest, char* src);

