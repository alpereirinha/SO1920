

all: argus argusd

argus: argus.o argus.h
	gcc -o argus argus.o tarefas.o

argusd: argusd.o argus.h
	gcc -o argusd argusd.o tarefas.o

argus.o: argus.c tarefas.o argus.h
	gcc -o argus.o -c argus.c -W -Wall

argusd.o: argusd.c tarefas.o argus.h
	gcc -o argusd.o -c argusd.c -W -Wall

tarefas.o: tarefas.c argus.h
	gcc -o tarefas.o -c tarefas.c -W -Wall

clean:
	rm -rf fifo* *.o argus argusd LOGS.txt erros.txt