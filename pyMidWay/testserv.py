#!/usr/bin/python

import midway as mw

mw.openlog("testserv", "./testserv", 6)

rc = mw.attach("", "Mr Anderson", mw.MWSERVER)
print rc
rc = mw.geturl()
print rc

def svc1(si):
    print "handler s1: "
    print si
    mw.reply (mw.MWSUCCESS, "hello from python\n", 666);
    return

def svc2(si):
    print "handler s2: " 
    print  si
    return mw.MWSUCCESS

def svc3(si):
    print "handler s1: "
    print si
    mw.reply (mw.MWSUCCESS);
    return

def svc4(si):
    print "handler s1: "
    print si
    mw.reply (mw.MWFAIL);
    return


mw.provide("s1", svc1)
mw.provide("s2", svc2)
mw.provide("s3", svc3)
mw.provide("s4", svc4)

def task1():
    print "task1"
    return 0

mw.addtask (task1, 3000, 100)

try:
    print "\nSTARTING\n\n"
    mw.mainloop()
    print "\nMAINLOOP EXITING \n\n"
except:
    print "exception"
    pass


mw.unprovide("s1")
mw.unprovide("s2")
mw.unprovide("s3")
mw.unprovide("s4")

mw.detach()


