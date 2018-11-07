// -*- c -*- 
#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include <MidWay.h>

static char * CVSId = "$Id: MidWay.xs,v 1.6 2004/06/17 09:51:41 eggestad Exp $";


static int
not_here(char *s)
{
    croak("%s not implemented on this architecture", s);
    return -1;
}

#define debug(fmt, ...) do { mwlog(MWLOG_DEBUG1, "%s(%d): " fmt, __FUNCTION__, __LINE__, ## __VA_ARGS__); } while(0)


static double
constant(char *name, int arg)
{
   debug ("adding constant %s", name);
    errno = 0;
    switch (*name) {
    case 'A':
	break;
    case 'B':
	break;
    case 'C':
	break;
    case 'D':
	break;
    case 'E':
	break;
    case 'F':
	if (strEQ(name, "FALSE"))
#ifdef FALSE
	    return FALSE;
#else
	    goto not_there;
#endif
	break;
    case 'G':
	break;
    case 'H':
	break;
    case 'I':
	break;
    case 'J':
	break;
    case 'K':
	break;
    case 'L':
	break;
    case 'M':
	if (strEQ(name, "MWCLIENT"))
#ifdef MWCLIENT
	    return MWCLIENT;
#else
	    goto not_there;
#endif
	if (strEQ(name, "MWCLIENTMASK"))
#ifdef MWCLIENTMASK
	    return MWCLIENTMASK;
#else
	    goto not_there;
#endif
	if (strEQ(name, "MWCONV"))
#ifdef MWCONV
	    return MWCONV;
#else
	    goto not_there;
#endif
	    if (strEQ(name, "MWEVSTRING"))
#ifdef MWEVSTRING
            return MWEVSTRING;
#else
            goto not_there;
#endif
	    if (strEQ(name, "MWEVGLOB"))
#ifdef MWEVGLOB
            return MWEVGLOB;
#else
            goto not_there;
#endif
	    if (strEQ(name, "MWEVREGEXP"))
#ifdef MWEVREGEXP
            return MWEVREGEXP;
#else
            goto not_there;
#endif
	    if (strEQ(name, "MWEVEREGEXP"))
#ifdef MWEVEREGEXP
            return MWEVEREGEXP;
#else
            goto not_there;
#endif
	if (strEQ(name, "MWFAIL"))
#ifdef MWFAIL
	    return MWFAIL;
#else
	    goto not_there;
#endif
	if (strEQ(name, "MWFASTPATH"))
#ifdef MWFASTPATH
	    return MWFASTPATH;
#else
	    goto not_there;
#endif
	if (strEQ(name, "MWGATEWAY"))
#ifdef MWGATEWAY
	    return MWGATEWAY;
#else
	    goto not_there;
#endif
	if (strEQ(name, "MWGATEWAYMASK"))
#ifdef MWGATEWAYMASK
	    return MWGATEWAYMASK;
#else
	    goto not_there;
#endif
	if (strEQ(name, "MWINDEXMASK"))
#ifdef MWINDEXMASK
	    return MWINDEXMASK;
#else
	    goto not_there;
#endif
	if (strEQ(name, "MWLOG_ALERT"))
#ifdef MWLOG_ALERT
	    return MWLOG_ALERT;
#else
	    goto not_there;
#endif
	if (strEQ(name, "MWLOG_DEBUG"))
#ifdef MWLOG_DEBUG
	    return MWLOG_DEBUG;
#else
	    goto not_there;
#endif
	if (strEQ(name, "MWLOG_DEBUG1"))
#ifdef MWLOG_DEBUG1
	    return MWLOG_DEBUG1;
#else
	    goto not_there;
#endif
	if (strEQ(name, "MWLOG_DEBUG2"))
#ifdef MWLOG_DEBUG2
	    return MWLOG_DEBUG2;
#else
	    goto not_there;
#endif
	if (strEQ(name, "MWLOG_DEBUG3"))
#ifdef MWLOG_DEBUG3
	    return MWLOG_DEBUG3;
#else
	    goto not_there;
#endif
	if (strEQ(name, "MWLOG_DEBUG4"))
#ifdef MWLOG_DEBUG4
	    return MWLOG_DEBUG4;
#else
	    goto not_there;
#endif
	if (strEQ(name, "MWLOG_ERROR"))
#ifdef MWLOG_ERROR
	    return MWLOG_ERROR;
#else
	    goto not_there;
#endif
	if (strEQ(name, "MWLOG_FATAL"))
#ifdef MWLOG_FATAL
	    return MWLOG_FATAL;
#else
	    goto not_there;
#endif
	if (strEQ(name, "MWLOG_INFO"))
#ifdef MWLOG_INFO
	    return MWLOG_INFO;
#else
	    goto not_there;
#endif
	if (strEQ(name, "MWLOG_WARNING"))
#ifdef MWLOG_WARNING
	    return MWLOG_WARNING;
#else
	    goto not_there;
#endif
	if (strEQ(name, "MWMAXNAMELEN"))
#ifdef MWMAXNAMELEN
	    return MWMAXNAMELEN;
#else
	    goto not_there;
#endif
	if (strEQ(name, "MWMAXSVCNAME"))
#ifdef MWMAXSVCNAME
	    return MWMAXSVCNAME;
#else
	    goto not_there;
#endif
	if (strEQ(name, "MWMORE"))
#ifdef MWMORE
	    return MWMORE;
#else
	    goto not_there;
#endif
	if (strEQ(name, "MWMULTIPLE"))
#ifdef MWMULTIPLE
	    return MWMULTIPLE;
#else
	    goto not_there;
#endif
	if (strEQ(name, "MWNOBLOCK"))
#ifdef MWNOBLOCK
	    return MWNOBLOCK;
#else
	    goto not_there;
#endif
	if (strEQ(name, "MWNOREPLY"))
#ifdef MWNOREPLY
	    return MWNOREPLY;
#else
	    goto not_there;
#endif
	if (strEQ(name, "MWNOTCLIENT"))
#ifdef MWNOTCLIENT
	    return MWNOTCLIENT;
#else
	    goto not_there;
#endif
	if (strEQ(name, "MWNOTIME"))
#ifdef MWNOTIME
	    return MWNOTIME;
#else
	    goto not_there;
#endif
	if (strEQ(name, "MWNOTRAN"))
#ifdef MWNOTRAN
	    return MWNOTRAN;
#else
	    goto not_there;
#endif
	if (strEQ(name, "MWSAFEPATH"))
#ifdef MWSAFEPATH
	    return MWSAFEPATH;
#else
	    goto not_there;
#endif
	if (strEQ(name, "MWSERVER"))
#ifdef MWSERVER
	    return MWSERVER;
#else
	    goto not_there;
#endif
	if (strEQ(name, "MWSERVERMASK"))
#ifdef MWSERVERMASK
	    return MWSERVERMASK;
#else
	    goto not_there;
#endif
	if (strEQ(name, "MWSERVERONLY"))
#ifdef MWSERVERONLY
	    return MWSERVERONLY;
#else
	    goto not_there;
#endif
	if (strEQ(name, "MWSERVICEMASK"))
#ifdef MWSERVICEMASK
	    return MWSERVICEMASK;
#else
	    goto not_there;
#endif
	if (strEQ(name, "MWSIGRST"))
#ifdef MWSIGRST
	    return MWSIGRST;
#else
	    goto not_there;
#endif
	if (strEQ(name, "MWSTDIO"))
#ifdef MWSTDIO
	    return MWSTDIO;
#else
	    goto not_there;
#endif
	if (strEQ(name, "MWSUCCESS"))
#ifdef MWSUCCESS
	    return MWSUCCESS;
#else
	    goto not_there;
#endif
	if (strEQ(name, "MWUNIQUE"))
#ifdef MWUNIQUE
	    return MWUNIQUE;
#else
	    goto not_there;
#endif
	break;
    case 'N':
	break;
    case 'O':
	break;
    case 'P':
	break;
    case 'Q':
	break;
    case 'R':
	break;
    case 'S':
	break;
    case 'T':
	if (strEQ(name, "TRUE"))
#ifdef TRUE
	    return TRUE;
#else
	    goto not_there;
#endif
	break;
    case 'U':
	if (strEQ(name, "UNASSIGNED"))
#ifdef UNASSIGNED
	    return UNASSIGNED;
#else
	    goto not_there;
#endif
	break;
    case 'V':
	break;
    case 'W':
	break;
    case 'X':
	break;
    case 'Y':
	break;
    case 'Z':
	break;
    }
    errno = EINVAL;
    return 0;

not_there:
    errno = ENOENT;
    return 0;
}

/************************************************************************
 *
 * task helpers, the SV point to the perl wrapper func, and the 
 * perlcalltaskwrapper call it. 

 * Whats happening here is almost pure magic, see perlcall(3)
 */

static SV * perl_task_dispatch = (SV*)NULL;
static SV * perl_event_dispatch = (SV*)NULL;

static int 
perlcalltaskwrapper(PTask pt)
{
   int rc;
   dSP ;
   int count ;

   ENTER ;
   SAVETMPS ;
   
   PUSHMARK(SP) ;
   XPUSHs(sv_2mortal(newSViv((IV)pt)));
   PUTBACK ;    	


   debug ("calling task wrapper with pt = %ld perl_task_dispatch = %p", pt, perl_task_dispatch);

   count = call_sv(perl_task_dispatch, G_SCALAR) ;  

   debug ("couht = %d", count);

   SPAGAIN ;

   if (count != 1)
      croak("Big trouble\n") ;
   
   rc = POPi;
   debug("rc = %d", rc);
   
   PUTBACK ;
   
   FREETMPS ;
   LEAVE ;
   return 0;
};

static void 
perl_event_hdl(int subid, char * event, char * data, int datalen)
{
   int rc;
   dSP ;
   
   debug("enter: subid %d event %s data \"%s\"", subid, event, data);
   ENTER ;
   SAVETMPS ;
   
   PUSHMARK(SP) ;
   XPUSHs(sv_2mortal(newSViv(subid)));
   XPUSHs(sv_2mortal(newSVpv(event, strlen(event))));
   XPUSHs(sv_2mortal(newSVpv(data, datalen)));
   PUTBACK ;    	


   debug ("calling EVENT wrapper");

   call_sv(perl_event_dispatch, G_DISCARD) ;  

   FREETMPS ;
   LEAVE ;
   debug("leave");
   return;
};

// THe module begin in ernest. 

MODULE = MidWay		PACKAGE = MidWay		

PROTOTYPES: ENABLE

double
constant(name,arg)
	char *		name
	int		arg

######################################################################
# Here we begin the API interfaces
#
# first attach/detach
######################################################################

int 
attach(addr = NULL, name = NULL, flags = 0)
char * addr;
char * name;
int flags;
CODE:
        if (addr && strlen(addr) == 0) addr = NULL;
	RETVAL = mwattach(addr, name, flags);
OUTPUT:
	RETVAL

int 
setcred(authtype, username = NULL, ...)
int authtype;
char * username; 
PREINIT:
	STRLEN sl;
	int errorflag = 0;
	int rc;
CODE:
	/*
		if (items > 0) {
			addr = (char *)SvPV(ST(0), sl);
			if (sl == 0) addr = NULL;
		}
		if (items > 1) {
			name = (char *)SvPV(ST(1), sl);
			if (sl == 0) name = NULL;
		}	
		if (items > 2)  {
			username = (char *)SvPV(ST(2), sl);
			if (sl == 0) username = NULL;
		}
		if (items > 3) {
			password = (char *)SvPV(ST(3), sl);
			if (sl == 0) password = NULL;
		}
		if (items > 4) {
			if (SvIOK(ST(4))) {
				flags = (int) SvIV(ST(4));
			} else if (SvNOK(ST(4))) {
				flags = (double) SvIV(ST(4));
			} else {
				errorflag = EINVAL;
			}
		}
	*/
	RETVAL = mwsetcred(authtype, username);
	OUTPUT:
	RETVAL


int 
detach()
CODE:
	RETVAL = mwdetach();
	OUTPUT:
	RETVAL

######################################################################
# Client API - service calling part 
######################################################################

void
listservices(glob = "*")
char *  glob;
PREINIT:
int flags = 0;
int rc, i;
char ** plist;
PPCODE:
	rc = mwlistsvc(glob, &plist, flags);
        mwlog(MWLOG_DEBUG, "list = %d", rc);
        if (rc > 0) { 
	   EXTEND(SP, rc);
	   for (i = 0; i < rc; i++) {
	      PUSHs(sv_2mortal(newSVpv(plist[i], 0)));
	   };
	   free(plist);

	};

void
call(service, ...)
char * service;
PREINIT:
char * data = NULL;
STRLEN datalen = 0;
char * rdata = NULL;
STRLEN rdatalen = 0;
int apprc = 0;
int rc;
int flags = 0;
int error;
PPCODE:
	if (items > 1) {
		data = (char *)SvPV(ST(1), datalen);
		if (datalen == 0) data = NULL;
 	};
	if (items > 2) {
		flags = (int) SvIV(ST(2));
 	};
	rc = mwcall(service, data, datalen, &rdata, &rdatalen, &apprc, flags);

       	EXTEND(SP, 3);	
	PUSHs(sv_2mortal(newSViv(rc)));
	if (rdata != NULL)
		PUSHs(sv_2mortal(newSVpv(rdata, rdatalen)));	
	else 
		PUSHs(sv_2mortal(&PL_sv_undef));
	PUSHs(sv_2mortal(newSViv(apprc)));	

	if (rdata != NULL) mwfree(rdata);

int
acall(service, ...)
char * service;
PREINIT:
char * data;
STRLEN datalen = 0;
int flags;
int error;
CODE:
	if (items > 1) {
		data = (char *)SvPV(ST(1), datalen);
		if (datalen == 0) data = NULL;
 	};
	if (items > 2) {
		flags = (int) SvIV(ST(2));
 	};
	RETVAL = mwacall(service, data, datalen, flags);
OUTPUT:
	RETVAL


void
fetch(handle, flags = 0)
int handle;
int flags;
PREINIT:
char * rdata = NULL;
STRLEN rdatalen = 0;
int apprc = 0;
int rc;
int error;
PPCODE:
	if (items > 1) {
		flags = (int) SvIV(ST(1));
 	};
	rc = mwfetch(&handle, &rdata, &rdatalen, &apprc, flags);

	EXTEND(SP, 3);
	PUSHs(sv_2mortal(newSViv(rc)));
	PUSHs(sv_2mortal(newSVpv(rdata, rdatalen)));
	PUSHs(sv_2mortal(newSViv(apprc)));	

	if (rdata == NULL) mwfree(rdata);

######################################################################
# The Tasklet API 
######################################################################


void
set_task_dispatch(perlfunc)
SV * perlfunc;
CODE:
	/* Take a copy of the callback */
    	if (perl_task_dispatch == (SV*)NULL)
    	    /* First time, so create a new SV */
    	    perl_task_dispatch = newSVsv(perlfunc) ;
    	else
    	    /* Been here before, so overwrite */
    	    SvSetSV(perl_task_dispatch, perlfunc) ;
	debug ("perl_task_dispatch = %p", perl_task_dispatch);

long 
addtaskdelayed(interval, delay)
int interval;
int delay;
CODE:
	RETVAL = mwaddtaskdelayed(perlcalltaskwrapper, interval, delay);
OUTPUT:
	RETVAL

int 
waketask(task)
IV task;
CODE:
	RETVAL = mwwaketask(task);
OUTPUT:
	RETVAL

int
settaskinterval(task, interval)
IV task;
int interval;
CODE:
	RETVAL = mwsettaskinterval (task, interval);
OUTPUT:
	RETVAL


void
dotasks()
CODE:
	mwdotasks();

######################################################################
# The Event API 
######################################################################

void
set_event_dispatch(perlfunc)
SV * perlfunc;
CODE:
	/* Take a copy of the callback */
    	if (perl_event_dispatch == (SV*)NULL)
    	    /* First time, so create a new SV */
    	    perl_event_dispatch = newSVsv(perlfunc) ;
    	else
    	    /* Been here before, so overwrite */
    	    SvSetSV(perl_event_dispatch, perlfunc) ;
	debug ("perl_event_dispatch = %p", perl_event_dispatch);


int 
event(event, ...)
char * event;
PREINIT:
char * data;
STRLEN datalen, sl;
char * user;
char * name;
CODE:
	if (items > 1) {
	   data = (char *)SvPV(ST(1), datalen);
	   if (datalen == 0) data = NULL;
	};
	if (items > 2) {
	   user = (char *)SvPV(ST(2), sl);
	   if (sl == 0) user = NULL;
 	};
	if (items > 3) {
	   name = (char *)SvPV(ST(3), sl);
	   if (sl == 0) name = NULL;
 	};


	RETVAL = mwevent(event, data, datalen, user, name);
	debug ("event \"%s\" data \"%s\"(%d) user %p name %p => %d", event, data, datalen, user, name, RETVAL);
OUTPUT:
	RETVAL

int 
subscribe(pattern, flags, subid)
char * pattern;
int flags;
int subid;
CODE:
	RETVAL = _mwsubscribe(pattern, subid, flags);
	debug ("subscribe \"%s\" subid %d flags %d => %d", pattern, subid, flags, RETVAL);
OUTPUT:
	RETVAL

int 
unsubscribe(subid)
int subid;
CODE:
	RETVAL = mwunsubscribe(subid);
OUTPUT:
	RETVAL


void
recvevents()
CODE:
	mwrecvevents();


######################################################################
# The API for accessing the logging API
######################################################################

void
openlog(progname, filepf = NULL, loglevel = MWLOG_INFO)
char * progname;
char * filepf;
int loglevel;
CODE:
	if (strlen(filepf) == 0) filepf == NULL;

    	mwopenlog(progname, filepf, loglevel);



int 
setloglevel(level = -1)
int level
CODE:

	RETVAL = mwsetloglevel(level);
OUTPUT:
	RETVAL

void
mw_log(loglevel, logmesg)
int loglevel;
char * logmesg;
CODE:
	mwlog(loglevel, logmesg);


######################################################################
# 
######################################################################

int
begin(sec, flags = 0)
double sec; 
int flags;
CODE:
	RETVAL = mwbegin(sec, flags);
OUTPUT:
	RETVAL

int 
commit()
CODE:
	RETVAL = mwcommit();

int 
abort()
CODE:
	RETVAL = mwabort();
OUTPUT:
	RETVAL

######################################################################
# here we do the server API.
# Some top level things are done in MidWay.pm
# First of all, the list of provided services are stored in Perl.
# we still call _mwGetServiceRequest(), but not _mwCallCService Function().
# thus mwservicerequest() and mwMainLoop() are all imp ed in perl.
# thsi has consequences for mw(un)provide().
# mwreply() and mwforward are unchanged.
######################################################################

int 
systemstate()
CODE:
	RETVAL = _mwsystemstate();
OUTPUT:
	RETVAL

int
isattached()
CODE:
	RETVAL = _mw_isattached();
OUTPUT:
	RETVAL


void 
incprovided()
PREINIT:
void _mw_incprovided(void);
CODE:
	_mw_incprovided();

void 
decprovided()
PREINIT:
void _mw_decprovided(void);
CODE:
	_mw_decprovided();

int 
ipc_provide(service)
char * service;
CODE:
	RETVAL = _mw_ipc_provide(service,0);
OUTPUT:
	RETVAL

int 
ipc_unprovide(service, svcid)
char * service;
int svcid
CODE:
	RETVAL = _mw_ipc_unprovide(service,svcid);
OUTPUT:
	RETVAL

int
forward(service, ...)
char * service;
PREINIT:
char * data;
STRLEN datalen = 0;
int flags;
CODE:
	if (items > 1) {
		data = (char *)SvPV(ST(1), datalen);
		if (datalen == 0) data = NULL;
 	};
	if (items > 2) {
		flags = (int) SvIV(ST(2));
 	};
	RETVAL = mwforward(service, data, datalen, flags);
	OUTPUT:
	RETVAL


int
reply(...)
PREINIT:
char * data;
STRLEN datalen = 0;
int flags;
int rc;
int apprc;
CODE:
	if (items > 0) {
		data = (char *)SvPV(ST(0), datalen);
		if (datalen == 0) data = NULL;
 	} else {
		data = NULL;
		datalen = 0;
	}

	if (items > 1) {
		rc = (int) SvIV(ST(1));
 	} else {
		rc = MWSUCCESS;
	}

	if (items > 2) {
		apprc = (int) SvIV(ST(2));
	} else {
		apprc = 0;
	}

	if (items > 3) {
		flags = (int) SvIV(ST(3));
	} else {
		flags = 0;
	}

	RETVAL = mwreply(data, datalen, rc, apprc, flags);
	OUTPUT:
	RETVAL


void 
GetServiceRequest(flags = 0)
int flags
PREINIT:
mwsvcinfo * si;
PPCODE:
	errno = 0;
	si = _mwGetServiceRequest(flags);
	if (si == NULL) {
		EXTEND(SP, 2);
		PUSHs(sv_2mortal(newSVpv("errorcode", 0)));
		PUSHs(sv_2mortal(newSViv(errno)));
	} else {
		EXTEND(SP, 22);
		PUSHs(sv_2mortal(newSVpv("handle", 0)));
		PUSHs(sv_2mortal(newSViv(si->handle)));
		PUSHs(sv_2mortal(newSVpv("data", 0)));
		PUSHs(sv_2mortal(newSVpv(si->data, si->datalen)));
		PUSHs(sv_2mortal(newSVpv("flags", 0)));
		PUSHs(sv_2mortal(newSViv(si->flags)));	
		PUSHs(sv_2mortal(newSVpv("service", 0)));
		PUSHs(sv_2mortal(newSVpv(si->service, 0)));	
		PUSHs(sv_2mortal(newSVpv("deadline", 0)));
		PUSHs(sv_2mortal(newSViv(si->deadline)));	
		PUSHs(sv_2mortal(newSVpv("clientid", 0)));
		PUSHs(sv_2mortal(newSViv(si->cltid)));
		PUSHs(sv_2mortal(newSVpv("serverid", 0)));
		PUSHs(sv_2mortal(newSViv(si->srvid)));
		PUSHs(sv_2mortal(newSVpv("serviceid", 0)));
		PUSHs(sv_2mortal(newSViv(si->svcid)));
		PUSHs(sv_2mortal(newSVpv("username", 0)));
		PUSHs(sv_2mortal(newSVpv(si->username, 0)));
		PUSHs(sv_2mortal(newSVpv("clientname", 0)));
		PUSHs(sv_2mortal(newSVpv(si->clientname, 0)));
		PUSHs(sv_2mortal(newSVpv("authentication", 0)));
		PUSHs(sv_2mortal(newSViv(si->authentication)));		
	}


######################################################################
# 
######################################################################
BOOT:
	debug ("Boot block");
        _mw_register_event_handler(perl_event_hdl);
