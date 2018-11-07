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

mwopenlog("test.pl", "logg", MWLOG_DEBUG3());
$l = mwsetloglevel();
print "loglevel is $l\n";

$rc = mwattach("", "Perl test server", MWSERVER());
print "mwattach returned $rc\n\n";

sub svcfunc {
    my %svcinfo = @_;
    my $reply;
    
    print "SERVICE HANDLER BEGINS !!!!!!!!!!!!!!!!!!\n";
    foreach $si (keys(%svcinfo)) {
	print "\$svcinfo{$si} = \"", $svcinfo{$si}, "\"\n";
    };
    $reply = " scalar(svcinfo)=".scalar (@_)." svcinfo=(".join (", ", @_).")";
    mwreply "  ****** svcfunc $reply ", MWSUCCESS(), 989, 0;
    print "SERVICE HANDLER ENDS !!!!!!!!!!!!!!!!!!!!\n";
};

$rc = mwprovide ("perlsvc", \&svcfunc, 0);
print "mwprovide returned $rc\n\n";

goto MAINLOOP;

TESTCLIENT:
@rc = mwcall("test", "date", "data");
print "mwcall returned ", join(", ", @rc), " (", scalar(@rc), ")\n";

$rc = mwacall("test", "date", "Perl test");
print "mwacall returned $rc\n";

@rc = mwfetch($rc);
print "mwfetch returned ", join(", ", @rc), " (", scalar(@rc), ")\n";

mwlog(MWLOG_INFO(), "infoing %s", "hei hei");
mwlog(MWLOG_DEBUG(), "debugging %s", "hei hei");

MAINLOOP:
    mwMainLoop();
DETACH:

$rc = mwunprovide ("perlsvc");
print "mwunprovide returned $rc\n\n";

mwdetach();
