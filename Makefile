all: argus argusd
        gcc argus.o argusd.o
argus.o: argus.c argus.h
        gcc -c argus.c
argusd.o: argusd.c argus.h
        gcc -c argusd.c
clean: rm *.p output