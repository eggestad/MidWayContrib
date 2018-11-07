#!/usr/bin/perl
# Before `make install' is performed this script should be runnable with
# `make test'. After `make install' it should work as `perl test.pl'

######################### We start with some black magic to print on failure.

# Change 1..1 below to 1..last_test_to_print .
# (It may become useful if the test is moved to ./t subdirectory.)

use MidWay;
$loaded = 1;

BEGIN { 
    my $rc; 
    $| = 1; 

    mwopenlog("event", "event", MWLOG_DEBUG1);

    $rc = mwattach();
    if ($rc != 0) {
	die("attach failed: $rc");
    }
}

END {
    mwdetach();
    print "not ok 1\n" unless $loaded;
}

print "ok 1\n";

######################### End of black magic.

# Insert your test code below (better if it prints "ok 13"
# (correspondingly "not ok 13") depending on the success of chunk 13
# of the test code):


sub eventhdl {
    my $event = shift;
    my $data = shift;
    print ("event: $event  data: $data\n");

    return;
};


print "MWEVGLOB = ", MWEVGLOB, "\n";
print "MWEVGLOB = ", MWEVGLOB(), "\n";
$rc = mwsubscribe("*", MWEVGLOB, \&eventhdl);
print "rc =$rc\n";
mwevent("perlev", "from PERL!");


while(1) {
    mwrecvevents();

    sleep(1);
};
