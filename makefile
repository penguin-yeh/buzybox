SHELL = /bin/bash
CC = gcc --static
CFLAGS = -g -pthread 
SRC = $(wildcard *.c)
EXE = $(patsubst %.c, %, $(SRC))

all: ${EXE}

%:	%.c
	${CC} ${CFLAGS} $@.c -o $@
	sudo chown root $@
	sudo chmod +s $@
	ln -s $@ ln
	ln -s $@ chown
	ln -s $@ stat
	ln -s $@ lazyshell
clean:
	rm ${EXE}
