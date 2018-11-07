#!/usr/bin/python

import midway

midway.openlog("test.py", "./xx", 4)

rc = midway.attach("", "Mr Anderson")
print rc
rc = midway.geturl()
print rc

rc = midway.listsvc("*")
print rc

rc = midway.call("hello", "from python")
print rc
rc = midway.call("hello")
print rc


hdl = midway.acall("ping", "xx")
print hdl
hdl = midway.acall("shell", "/sbin/route")
print hdl

rc = midway.MWMORE
while rc == midway.MWMORE:
    rcl = midway.fetch(0, midway.MWMULTIPLE)
    print rcl
    print len(rcl)
    rc = rcl[0]

rc = midway.MWMORE
while rc == midway.MWMORE:
    rcl = midway.fetch(0, midway.MWMULTIPLE)
    print rcl
    print len(rcl)
    rc = rcl[0]


midway.detach()


