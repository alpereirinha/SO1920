CC = gcc
CFLAGS = -Wall

all: argus argusd

argus: argus.o
argus.o: argus.c argus.h

argusd:argus.o
argusd.o: argusd.c argus.h

clean:
	rm -rf *.o argus argusd