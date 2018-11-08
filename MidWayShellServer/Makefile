
CC = gcc

INCLUDES =  

CFLAGS =  -g -O2  $(INCLUDES)

all: mw_shell_server

clean: 
	rm -f  mw_shell_server *.o *.so

distclean: 
	rm -f Makefile *~

mw_shell_server:
	$(CC) -lMidWay -o mw_shell_server mw_shell_server.c 

install: all


uninstall:



