SHELL = /bin/sh
top_srcdir = ../..
srcdir = .

INSTALL = /usr/bin/install -c

prefix = /usr/local
exec_prefix = ${prefix}
bindir = $(exec_prefix)/bin
libdir = $(prefix)/lib
infodir = $(prefix)/info

all: MidWay.class CallReply.class MidWayException.class ServiceProvider.class ICallListener.class

CLASSPATH=../..

JAVACFLAGS=-classpath $(CLASSPATH)
JAVAC = javac

clean:
	rm -f *.class


MidWay.class: MidWay.java CallReply.class MidWayException.class

CallReply.class: CallReply.java

MidWayException.class: MidWayException.java



%.class: %.java
	$(JAVAC) $(JAVACFLAGS) $<
