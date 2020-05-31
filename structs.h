#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct rimefodime {
	int isTempoDeInatividade;  	// FLAG
	int argTempoDeInatividade;
	int isExecutar;   			// FLAG
	char* argExecutar;
} st;