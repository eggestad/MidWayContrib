SHELL = /bin/sh
top_srcdir = ..
srcdir = .


.SUFFIXES:
.SUFFIXES: .c .o


CC = gcc

CFLAGS= -I. -I/include -I/include/linux -I/usr/local/include -g -O2 -O0

LDFLAGS = 
LIBS = 
INSTALL = /usr/bin/install -c

prefix = /usr/local
exec_prefix = ${prefix}
bindir = $(exec_prefix)/bin
libdir = $(prefix)/lib
infodir = $(prefix)/info


JAVAH = /usr/bin/javah
CLASSPATH=.. # $(HOME)/projects/jMidWay

all: libjMidWay.so

../org/midway/MidWay.class:
	(cd ../org/midway; make all)

jMidWay.h: ../org/midway/MidWay.class
	rm $@
	$(JAVAH) -classpath $(CLASSPATH) -o jMidWay.h org.midway.MidWay

libjMidWay.so: jMidWay.o 
	$(CC) -g -o $@ -shared $+ -lMidWay

install: all
	/usr/bin/install -c -D -m 644 libjMidWay.so $(libdir)/libjMidWay.so

uninstall:
	rm -f $(libdir)/libjMidWay.so

jMidWay.o: jMidWay.c jMidWay.h
	$(CC) -c -fpic -g  $(CFLAGS) $<


clean:
	rm -f *.so *.o 
