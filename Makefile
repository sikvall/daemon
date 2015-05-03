
all:	daemon

daemon: main.o
	gcc main.o -o daemon

main.o: main.c
	gcc -c main.c
