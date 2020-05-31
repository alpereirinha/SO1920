#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

//Servidor
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <sys/wait.h>
//#include <stdio.h>
//#include <fcntl.h>
//#include <unistd.h>
// #include <stdlib.h>
// #include <string.h>

 #define MAX 256

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