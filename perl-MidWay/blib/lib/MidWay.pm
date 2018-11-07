package MidWay;

use strict;
use Carp;
use vars qw($VERSION @ISA @EXPORT @EXPORT_OK $AUTOLOAD);

require Exporter;
require DynaLoader;
require AutoLoader;

@ISA = qw(Exporter DynaLoader);
# Items to export into callers namespace by default. Note: do not export
# names by default without a very good reason. Use EXPORT_OK instead.
# Do not simply export all your public functions/methods/constants.
@EXPORT = qw(
	     MWCLIENT
	     MWCLIENTMASK
	     MWCONV
	     MWEVSTRING
	     MWEVGLOB
	     MWEVREGEXP
	     MWEVEREGEXP
	     MWFAIL
	     MWFASTPATH
	     MWGATEWAY
	     MWGATEWAYMASK
	     MWINDEXMASK
	     MWLOG_ALERT
	     MWLOG_DEBUG
	     MWLOG_DEBUG1
	     MWLOG_DEBUG2
	     MWLOG_DEBUG3
	     MWLOG_DEBUG4
	     MWLOG_ERROR
	     MWLOG_FATAL
	     MWLOG_INFO
	     MWLOG_WARNING
	     MWMAXNAMELEN
	     MWMAXSVCNAME
	     MWMORE
	     MWMULTIPLE
	     MWNOBLOCK
	     MWNOREPLY
	     MWNOTCLIENT
	     MWNOTIME
	     MWNOTRAN
	     MWSAFEPATH
	     MWSERVER
	     MWSERVERMASK
	     MWSERVERONLY
	     MWSERVICEMASK
	     MWSIGRST
	     MWSTDIO
	     MWSUCCESS
	     MWUNIQUE
	     mwattach
	     mwdetach
	     mwlistsvc
	     mwcall
	     mwacall
	     mwfetch
	     mwopenlog
	     mwsetloglevel
	     mwlog
	     mwbegin
	     mwcommit
	     mwabort
	     mwprovide
	     mwunprovide
	     mwreply
	     mwforward
	     mwservicerequest
	     mwMainLoop
	     mwaddtask
	     mwwaketask
	     mwsettaskinterval
	     mwdotasks
	     mwevent
	     mweventbcast
	     mwsubscribe
	     mwunsubscribe
	     mwrecvevents
);
$VERSION = '0.1';

sub sigabort {
    mwdetach();
    exit(-1);
};
sub install_sigactions {
    $SIG{HUP} = \&sigabort;
    $SIG{INT} = \&sigabort;
    $SIG{TERM} = \&sigabort;
    $SIG{QUIT} = \&sigabort;
};

#
#
#

sub debug1 {
    my $fmt = shift;
    mw_log(6, sprintf($fmt, @_));
};

sub debug {
    my $fmt = shift;
    mw_log(5, sprintf($fmt, @_));
#    print sprintf($fmt, @_), "\n";
};

sub Error {
    my $fmt = shift;
    mw_log(1, sprintf($fmt, @_));
#    print sprintf($fmt, @_), "\n";
};


# wrapper funcs, needed to export mw* names. 
# Also needed to make sure funcs returning a list are treated
# correct in a scalar context, even if that don't make sence.

sub mwattach {
    my $rc;
    debug1 "mwattach: ", @_;
    $rc = attach(@_);
    install_sigactions();
    return $rc;
};

sub mwdetach {
    my $rc;
    $rc = detach(@_);
    return $rc;
};

sub mwlistsvc {
    my @rc;
    debug1 ("mwlistsvc start");
    @rc = listservices(@_);
    debug1 ("mwlistsvc end");
    return @rc;
}

sub mwcall {
    my @rc;
    @rc = call(@_);
    return @rc;
};

sub mwacall {
    my $rc;
    $rc = acall(@_);
    return $rc;
};

sub mwfetch {
    my @rc;
    @rc = fetch(@_);
    return @rc;
};

sub mwopenlog {
    openlog(@_);
    return;
};

sub mwsetloglevel {
    my $rc;
    $rc = setloglevel(@_);
    return $rc;
};

sub mwlog {
    my $llevel;
    my $msg;
    my $fmt;
    $llevel = shift @_;
    $fmt = shift @_;

    $msg = sprintf $fmt, @_; 
    mw_log($llevel, $msg);
};

sub mwbegin {
    my $rc;
    $rc = begin(@_);
    return $rc;
};

sub mwcommit {
    my $rc;
    $rc = commit();
    return $rc;
};

sub mwabort {
    my $rc;
    $rc = abort();
    return $rc;
};

######################################################################
my $provided = 0;
my %serviceid = ();
my %servicesubref = ();
my $requestpending = 0;

sub mwprovide  ( $&$ ) {
    my $svcname = $_[0];
    my $subref = $_[1];
    my $flags = $_[2];
    my $rc;
    my $svcid;

    $rc = isattached() ;
    if ($rc != 1) { 
	debug1("mwprovide aborts we're not attached to an instance");
	return -107; # ENOTCONN
    }; 
    $rc = systemstate() ;
    if ($rc != 0) { 
	debug1("mwprovide aborts because instance is in shutdown state");
	return $rc;
    }; 
    
    $svcid = ipc_provide($svcname);
    if ($svcid < 0) { 
	debug1("mwprovide aborts because ipc_provide failed");
	return $svcid;
    } 
    debug1("provide of $svcname succeded svcid=%#x", $svcid);
    $serviceid{$svcname} = $svcid;
    $servicesubref{$svcname} = $subref;
    $provided++;
    incprovided();
    install_sigactions();
    return $svcid;
};
    
sub mwunprovide ( $ ) {
    my $svcname = shift;
    my $rc;
    my $svcid;
    $rc = isattached() ;
    if ($rc != 1) { 
	debug1("mwprovide aborts we're not attached to an instance");
	return -107; # ENOTCONN
    }; 
    $rc = systemstate() ;
    if ($rc != 0) { 
	debug1("mwprovide aborts because instance is in shutdown state");
	return $rc;
    }; 
    $rc = ipc_unprovide($svcname, $serviceid{$svcname});
    delete $serviceid{$svcname};
    delete $servicesubref{$svcname};
    debug1("unprovide of $svcname succeded");
    $provided--;
    decprovided();
    return $rc;
};

sub mwreply {
    my $rc;
    if ($_[1] != MWMORE()) {
	$requestpending = 0;
    };
    $rc = reply (@_);
    return $rc;
};

sub mwforward {
    my $rc;
    $rc = forward (@_);
    if ($rc == 0) {
	$requestpending = 0;
    };
    return $rc;
};

#
# for a more indepth on these func see mwserverapi.c
# These funcs are imp'ed in C there.
#
sub mwservicerequest {
    my $flags = 0;
    my %svcinfo;
    my $subref;
    my $rc;

    if (defined ($_[0])) { $flags = shift; };
    $rc = 0;

    %svcinfo = GetServiceRequest($flags);
    debug1 ("Get Service request returned (", scalar(keys(%svcinfo)), ")");
#    "Get Service request returned (", join (", ", @svcinfo), ") (", scalar(@svcinfo), ")");
    
    if (scalar(keys(%svcinfo)) == 1) {
	debug1("Failed to get a service request, reason %d", $svcinfo{'errorcode'});
	return -$svcinfo{'errorcode'};
    };
    
    $subref = $servicesubref{$svcinfo{'service'}};
    if (! defined ($subref)) {
	Error "got an unepected service request for %s", 
	$svcinfo{'service'};
	return -117; # EUCLEAN
    };
    $requestpending = 1;
    $rc = &${subref} (%svcinfo);
    
    if ($requestpending != 0) {
	mwreply "", $rc, 0, 0;
    };
    
    return $rc;
};

sub mwMainLoop {
    my $flags = 0;
    my $rc;

    if (defined ($_[0])) { $flags = shift; };
    
    while (1) {
	debug1 "MainLoop are calling mwservicerequest";
	$rc = mwservicerequest($flags);
	if ($rc < 0) {
	    if ( ($rc == -EINTR)  && ($flags & MWSIGRST())) { next;};
	    return $rc;
	};
    };
};

######################################################################

my %tasks = ();

sub task_dispatch {
    my $pt = shift;
    my $rc;
    my $ref;
    debug1 "task dispatch pt = $pt\n";
    $ref = $tasks{$pt};
    debug1 "task ref = $ref\n";
    $rc = &$ref;
    
    return $rc;
};


sub mwaddtask {
    my $funcref;
    my $interval = -1;
    my $delay = 0;
    my $pt; 

    if (!defined($_[0]) || ! ref($_[0])) {
	die("mwaddtask myst be passed an reference to a function as fist argument.");
    };
    $funcref = $_[0];

    if (defined $_[1]) {
	$interval  = int($_[1]);
    };

    if (defined $_[2]) {
	$delay  = int($_[2]);
    };

    $pt = addtaskdelayed($interval, $delay);
    debug1 "mwaddtask pt = $pt ref = $funcref\n";
    $tasks{$pt} = $funcref;

    return $pt
};

sub mwwaketask {
    my $task = shift;
    
    return waketask($task);
};

sub mwsettaskinterval {
    my $task = shift;
    my $interval = -1;
    
    if (defined($_[1])) {
	$interval = int($_[1]);
    };
	
    return settaskinterval($task, $interval);
};

sub  mwdotasks {
    dotasks();
};


######################################################################
my %eventsubs = ();
my $lastsubid = 0;

sub mwevent {
    my $event = shift;
    my $data = shift;
    my $user = shift;
    my $name = shift;
    my $rc;

    debug1 ("event: $event data=$data user=$user clientname=$name");

    $rc = event($event, $data, $user, $name);
    debug1 ("event rc = $rc");

    return $rc;
};

sub event_dispatch {
    my $subid = shift;
    my $event = shift; 
    my $data = shift;
    my $funcref;
    
    debug1("got $subid for event $event \"$data\"");

    $funcref = $eventsubs{$subid};

    if (!ref ($funcref)) { return; };
    
    &$funcref($event, $data);
    
    return;
};

sub mwsubscribe {
    my $pattern = shift;
    my $flags = shift;
    my $funcref = shift;
    my $subid; 
    my $rc;

    $subid = $lastsubid++;
    debug1 ("mwsubscribe: pattern $pattern subid $subid flags $flags"); 

    $subid = subscribe($pattern, $flags, $subid);
    if ($rc >= 0) {
	$eventsubs{$subid} = $funcref;
    };
    
    return $subid;
};

sub mwunsubscribe {
    my $subid = shift;
    
    delete $eventsubs{$subid};
    
    return unsubscribe ($subid);
};

    
sub mweventbcast {
    my $event = shift;
    my $data = shift;

    return mwevent $event, $data, undef, undef;
};

sub mwrecvevents {
    recvevents();
};


######################################################################



sub AUTOLOAD {
    # This AUTOLOAD is used to 'autoload' constants from the constant()
    # XS function.  If a constant is not found then control is passed
    # to the AUTOLOAD in AutoLoader.

    my $constname;
    ($constname = $AUTOLOAD) =~ s/.*:://;
    croak "& not defined" if $constname eq 'constant';
    my $val = constant($constname, @_ ? $_[0] : 0);
    debug1 "Constant $constname = $val\n";
    if ($! != 0) {
	if ($! =~ /Invalid/) {
	    $AutoLoader::AUTOLOAD = $AUTOLOAD;
	    goto &AutoLoader::AUTOLOAD;
	}
	else {
		croak "Your vendor has not defined MidWay macro $constname";
	}
    }
    no strict 'refs';
    *$AUTOLOAD = sub  { $val };
    goto &$AUTOLOAD;
}

bootstrap MidWay $VERSION;

# Preloaded methods go here.
set_task_dispatch(\&task_dispatch);
set_event_dispatch(\&event_dispatch);

# Autoload methods go after =cut, and are processed by the autosplit program.

1;
__END__
# Below is the stub of documentation for your module. You better edit it!

=head1 NAME

MidWay - Perl extension for MidWay, a service request broker.

=head1 SYNOPSIS

    use MidWay;

=head2 Client Service Call API

    $rc = mwattach ["url" [, "progname" [ flags]]];
    $rc = mwdetach ;
    ($rc, $replydata, $applicationreturncode) = 
            mwcall "service" [, "data" [, flags]];

    $rc = mwsetcred authentication_type, "username" [, ...];

    $handle = mwacall "service" [, "data" [, flags]];
    ($rc, $replydata, $applicationreturncode) = 
            mwfetch $handle [, flags];

    @svclist = mwlistsvc [ glob ];

=head2 Logging API

    mwlog loglevel, "printf formated message", ...;
    mwopenlog "progname" [, "filename" [, loglevelthreshold]];
    $oldlevel = mwsetloglevel [ newloglevelthreshold];

=head2 Extentions to API for Servers
    
    sub servicefunction {
	my %svcinfo = @_;
	my $handle =      $svcinfo {'handle'};
	my $data =        $svcinfo {'data'};
	my $flags =       $svcinfo {'flags'};
	my $servicename = $svcinfo {'service'};
	my $deadline =    $svcinfo {'deadline'};
	my $clientid =    $svcinfo {'clientid'};
	my $serverid =    $svcinfo {'serverid'};
	my $serviceid =   $svcinfo {'serviceid'};
	
	body of service function
	mwreply returndata, returncode, applicationreturncode, flags;
	mwforward "service", data, , flags;
    }

    $rc = mwprovide "servicename", \&servicefunction, flags;
    $rc = mwunprovide "servicename";

    $rc = mwservicerequest [flags];
    $rc = mwMainLoop [flags];

    sub tasklet {
        my $taskref = shift;
	return 0;
    }                
  
=head2 Tasklet API

    $taskref = mwaddtask \&tasklet, interval [, initialdelay]; 
    $rc = mwwaketask $taskref;
    $rc = mwsettaskinterval $taskref, $interval;

    mwdotasks ;

=head2 Event API
    $rc = mweventbcast $eventname [, $data];

    $rc = mwevent $eventname, $data, $username, $clientname;

    sub eventhandler {
	my $event = shift;
	my $data =) shift;

	return;
    };

    $subid = mwsubscribe $pattern, $flags, \&eventhandler;

    $rc = mwunsubscribe $subid;

    mwrecvevents();

=head1 DESCRIPTION

This module provides a Perl interface to the MidWay C api.  See the
man pages for the C api for the gory details.  Note that the C library
must be installed on you host. If you don't have it, use the Netclient
module instead. It is implemented completely in Perl, and don't
require any compilation.  Of course it only support the SRB protocol,
and call/forward/reply only.  This native module does it all.

The differences in Perl can be summed up as follows:

All char * data, int datalen pairs are replace by a Perl scalar,
pretty straight forward since Perl scalars may contain NUL chars.  If
you want to do the equivalent to NULL for data, pass undef as data
argument.

Function pointers are replaced by references to Perl subroutines.

mwcall and mwfetch return a list instead of returning thru their
parameters.  This make more sense in Perl. Just note that in case of
failure they return a list with one element only, the return code.
This might change in order to be more in compliance to Perl error
handling.

I might change the parameter order in some Perl call, in order to move
the more used parameters forward. All parameters defaults to either 0
or "", and this may be used as fillers if needed.

Note the the service handler functions are passed a hash. This
reflects the reason we use a struct i the native C API. Members will
be added without notice.

=head1 Exported constants

  MWCLIENT
  MWCLIENTMASK
  MWCONV
  MWEVSTRING
  MWEVGLOB
  MWEVREGEXP
  MWEVEREGEXP
  MWFAIL
  MWGATEWAY
  MWGATEWAYMASK
  MWINDEXMASK
  MWLOG_ALERT
  MWLOG_DEBUG
  MWLOG_DEBUG1
  MWLOG_DEBUG2
  MWLOG_DEBUG3
  MWLOG_DEBUG4
  MWLOG_ERROR
  MWLOG_FATAL
  MWLOG_INFO
  MWLOG_WARNING
  MWMAXNAMELEN
  MWMAXSVCNAME
  MWMORE
  MWMULTIPLE
  MWNOBLOCK
  MWNOREPLY
  MWNOTCLIENT
  MWNOTIME
  MWNOTRAN
  MWSAFEPATH
  MWSERVER
  MWSERVERMASK
  MWSERVERONLY
  MWSERVICEMASK
  MWSIGRST
  MWSTDIO
  MWSUCCESS
  MWUNIQUE

=head1 AUTHOR

Terje Eggestad, terje.eggestad@iname.com

=head1 SEE ALSO

MidWay::NetClient(3). MidWay(7), 
mwacall(3C), mwcall(3C), mwfetch(3C), mwattach(3C), mwdetach(3C), 
mwprovide(3C), mwunprovide(3C), mwlog(3C), mwloglevel(3C), 
mwbegin(3C), mwcommit(3C), mwabort(3C), mwreply(3C), mwforward(3C), 
mwMainLoop(3C), mwservice(3C), mwservicerequest(3C)

=cut
