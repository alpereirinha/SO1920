all: server client clean
client: client.o
    gcc client.o -o client -g
    
client.o: client.c rules.h structs.h
    gcc client.c -c -g

server: server.o
    gcc server.o -o server -g

server.o: server.c rules.h structs.h
    gcc server.c -c -g