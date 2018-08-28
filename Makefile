.POSIX:
.SUFFIXES:

all: vm
vm: main.o
	cc -o vm main.o

main.o: main.c exa.c
	cc -c main.c
