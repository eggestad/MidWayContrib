#!/usr/bin/python

from midway import * 

import time

openlog("testevents", "./testevents", 6)

rc = attach("", "Neo", MWSERVER)
print rc
rc = geturl()
print rc

    
def evehdl(eve, data):
    print 'starting\n'
    print '-------- Event: %s (%s)\n' % (eve, data)
    print 'ending\n'
    return 0

    

rc = subscribe ("*", evehdl, MWEVGLOB)
print rc


event ('myself', 'calling')

i = 0

while 1:
    recvevents()
    i = i + 1
    print i
    time.sleep(0.3)
    
