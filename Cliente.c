#include "structs.h"

//Função que dá parse dos comandos para uma estrutura
// Retorna 0 quando dá erro, 1 quando corre bem
//Tipo=0 está a tratar a command line, senão é menu
int trataComandos(char* argv[],int argc,st* estrutura,int tipo){
    int i=0;

    if (tipo==0) i=1;

	for (;i<argc;i++){ 
		if (strcmp(argv[i], CMD_TEMP_INATIV_LONG)==0 || strcmp(argv[i], CMD_TEMP_INATIV)==0){
			estrutura->isTempoDeInatividade = true;
			estrutura->argTempoDeInatividade = atoi(argv[i++]);
        } else if(strcmp(argv[i],CMD_TEMP_INATIV_LONG)==0 || strcmp(argv[i], CMD_TEMP_INATIV)==0){
            estrutura->isTempoExecutar == true;
            estrutura->argTempoDeExecutar = atoi(argv[i++]);
		} else if (strcmp(argv[i], CMD_EXECUTAR_LONG)==0 || strcmp(argv[i], CMD_EXECUTAR)==0){
            char* aux = argv[i++];
            if (strlen(aux) > MAX-1){
                printf("Mensagem demasiado longa\n"); //Temos de tratar este erro
                return 0;
            } 
			estrutura->isExecutar = true;
			strcpy(estrutura->argExecutar, aux);
		} else if (strcmp(argv[i], CMD_LIST_LONG)==0 || strcmp(argv[i], CMD_LIST)==0){
            estrutura->isListar = true;
        } else if (strcmp(argv[i], CMD_TERMINAR_LONG)==0 || strcmp(argv[i], CMD_TERMINAR)==0){
            estrutura->isTerminar = true;
            estrutura->argvTerminar = atoi(argv[i++]);
        } else if (strcmp(argv[i], CMD_HISTORY_LONG)==0 || strcmp(argv[i], CMD_HISTORY)==0){
            estrutura->isHistory = true;
        } else if (strcmp(argv[i], CMD_AJUDA_LONG)==0 || strcmp(argv[i], CMD_AJUDA)==0){
            estrutura->isAjuda = true;
            //Colocar aqui os prints dos comandos
        }
	}
    return 1;
}

int main (int argc, char* argv[]){
	int n,fd_in,fd_out;
    char buff[2][MAX];
	st estrutura;

	//se nao ha argumentos o servidor nao tem nada para fazer
	if (argc==1) {write(1,"Sem argumentos\n",15); return 0;}

	fd_in=open("fifo_in", O_WRONLY);

    if (trataComandos(argv,argc,&estrutura,0))
        write(fd_in,estrutura,sizeof(st));
    else
        printf("Ocorreu um erro na leitura dos comandos.\n");

	while(1){
        printf("\nComando: ");
        scanf("%s",buff[0]);
        printf("\nArgumento: ");
        scanf("%s",buff[1]);  
        if (trataComandos(buff,2,&estrutura,1))
            write(fd_in,estrutura,sizeof(st));
        else
            printf("Ocorreu um erro na leitura dos comandos.\n");
        
    }

	//se e tempo de inatividade, tempo de execucao ou sair nao precissa de ler output
	if ((strncmp(argv[1],"-1",2))==0 || (strncmp(argv[1],"-m",2)==0)|| (strncmp(argv[1],"-t",2)==0)){	
		fd_out=open("fifo_out", O_RDONLY); 
		n=read(fd_out,buf,MAX);
		write(1,&buf,n);
	}
	return 0;
}