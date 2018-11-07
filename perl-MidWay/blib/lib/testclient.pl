#!/usr/bin/perl
# Before `make install' is performed this script should be runnable with
# `make test'. After `make install' it should work as `perl test.pl'

######################### We start with some black magic to print on failure.

# Change 1..1 below to 1..last_test_to_print .
# (It may become useful if the test is moved to ./t subdirectory.)

BEGIN { $| = 1; print "1..1\n"; }
END {print "not ok 1\n" unless $loaded;}
use MidWay;
$loaded = 1;
print "ok 1\n";

######################### End of black magic.

# Insert your test code below (better if it prints "ok 13"
# (correspondingly "not ok 13") depending on the success of chunk 13
# of the test code):

mwopenlog("testclient.pl", "logg", MWLOG_DEBUG);
$l = mwsetloglevel();
print "loglevel is $l\n";

$rc = mwattach();
print "mwattach returned $rc\n\n";

@l = mwlistsvc("s*");
print @l, "\n";
print "[", join(", ", @l), "]\n";

@rc = mwcall("test");
print "mwcall returned ", join(", ", @rc), " (", scalar(@rc), ")\n";

#goto end;
$rc = mwacall("hello", "date", "Perl test");
print "mwacall returned $rc\n";

if ($rc > 0) {
    @rc = mwfetch($rc);
    print "mwfetch returned ", join(", ", @rc), " (", scalar(@rc), ")\n";
};
mwlog(MWLOG_INFO, "infoing %s", "hei hei");
mwlog(MWLOG_DEBUG(), "debugging %s", "hei hei");

end:
mwdetach();
