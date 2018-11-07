SHELL = /bin/sh
top_srcdir = .
srcdir = .

INSTALL = /usr/bin/install -c

prefix = /usr/local
exec_prefix = ${prefix}
bindir = $(exec_prefix)/bin
libdir = $(prefix)/lib
infodir = $(prefix)/info
jardir = $(prefix)/share/java

SUBDIRS = org native

JAVAC = javac
JAVA = java #
CLASSPATH = -classpath  `pwd`:`pwd`/junit.jar
LIBPATH = -Djava.library.path=./native

JUNITTESTOPT = -Xcheck:jni 
JARFILE = jMidWay.jar

all: 
	for dir in $(SUBDIRS); do\
        (cd $$dir; $(MAKE) $@);\
        if [ $$? -ne 0 ]; then exit 3; fi\
        done;

test.class: test.java
	$(JAVAC) $(CLASSPATH) $(JAVACFLAGS) $<

jar: $(JARFILE)

$(JARFILE): all
	jar cf $(JARFILE) org/midway/*.class org/midway/*.java

install: all $(JARFILE)
	(cd native; make $@ )
	/usr/bin/install -c -D -m 644 $(JARFILE) $(jardir)/$(JARFILE)

uninstall:
	(cd native; make $@ )
	rm -f $(jardir)/$(JARFILE)

runtest: test.class
	$(JAVA) $(LIBPATH) $(JUNITTESTOPT) $(CLASSPATH)  junit.textui.TestRunner test

runtestswing: test.class
	$(JAVA) $(LIBPATH) $(JUNITTESTOPT) $(CLASSPATH)  junit.swingui.TestRunner test


TestServer.class: TestServer.java
	$(JAVAC) $(CLASSPATH) $(JAVACFLAGS) $<

clean:
	for dir in $(SUBDIRS); do\
        (cd $$dir; $(MAKE) $@);\
        if [ $$? -ne 0 ]; then exit 3; fi\
        done;
	rm -f *.class *.so *.o $(JARFILE)

