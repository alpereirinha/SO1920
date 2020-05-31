#include "rules.h"

typedef struct comandos{
	int isTempoDeInatividade;  	// FLAG
	int argTempoDeInatividade;
    int isTempoExecutar;        //FLAG    
    int argTempoDeExecutar;     
	int isExecutar;   			//FLAG
    int isListar;               //FLAG
	char argExecutar[MAX];
    int isTerminar;             //FLAG
    int argvTerminar;
    int isHistory;              //FLAG
    int isAjuda;                //FLAG
} st;