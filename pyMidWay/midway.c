/*
  The MidWay API for Python
  Copyright (C) 2004 Terje Eggestad

  The MidWay API is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public License as
  published by the Free Software Foundation; either version 2 of the
  License, or (at your option) any later version.
  
  The MidWay API is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.
  
  You should have received a copy of the GNU Library General Public
  License along with the MidWay distribution; see the file COPYING.  If not,
  write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
  Boston, MA 02111-1307, USA. 
*/

/*
 * $Id: midway.c,v 1.2 2004/07/06 14:55:45 eggestad Exp $
 * $Name:  $
 * 
 * $Log: midway.c,v $
 * Revision 1.2  2004/07/06 14:55:45  eggestad
 * * fix for abort after N runs
 * * added forward()
 *
 * Revision 1.1.1.1  2004/07/06 14:00:55  eggestad
 * Python MidWay bindings
 *
 */

#include <Python.h>

#include <string.h>
#include <errno.h>

#include <MidWay.h>


#define debug(fmt, ...) do { mwlog(MWLOG_DEBUG, fmt, ## __VA_ARGS__);} while(0)
#define Error(fmt, ...) do { mwlog(MWLOG_ERROR, fmt, ## __VA_ARGS__);} while(0)

static PyObject * attach(PyObject * self, PyObject * args)
{
   char * url = NULL, * cltname = NULL;
   int rc, flags = 0;

   if (!PyArg_ParseTuple(args, "|ssi", &url, &cltname, &flags))
   {
      return NULL;
   } 
   debug ("hei url=%s clientname=%s flags = %x", url, cltname, flags);
   if (strlen(url) == 0) url = NULL;

   rc = mwattach(url, cltname, flags);
   debug("attach = %d", rc);

   if (rc < 0) {
      // throw an exception 
   };
   return Py_BuildValue("i", rc);
};

static PyObject * detach(PyObject * self, PyObject * args)
{
   int rc; 

   if (!PyArg_ParseTuple(args, ""))
   {
      return NULL;
   } 

   rc = mwdetach();
   debug ("detach = %d", rc);

   return Py_BuildValue("i", rc);
};

static PyObject * geturl(PyObject * self, PyObject * args)
{
   char * url;
   
   if (!PyArg_ParseTuple(args, ""))
   {
      return NULL;
   } 

   url = mwgeturl();
   if (url == NULL) {
      Py_INCREF(Py_None); 
      return Py_None;
   };
   return Py_BuildValue("s", url);
};

/************************************************************************/

static PyObject * call(PyObject * self, PyObject * args)
{
   char * svc;
   char * data = NULL, *rdata = NULL;
   int dlen = 0, rlen = 0, apprc = 0;
   int rc, flags = 0;

   if (!PyArg_ParseTuple(args, "s|s#i", &svc, &data, &dlen, &flags))
   {
      return NULL;
   } 

   debug ("call %s with %s(%d)", svc, data, dlen);

   rc = mwcall(svc, data, dlen, &rdata, &rlen, &apprc, flags);
   debug ("call = %d\n", rc);

   if (rc >= 0) 
      return Py_BuildValue("(iis#)", rc, apprc, rdata, rlen);
   else
      return Py_BuildValue("(i)", rc);
   
};

static PyObject * acall(PyObject * self, PyObject * args)
{
   char * svc;
   char * data;
   int dlen;
   int rc, flags = 0;

   if (!PyArg_ParseTuple(args, "s|s#i", &svc, &data, &dlen, &flags))
   {
      return NULL;
   } 

   rc = mwacall(svc, data, dlen, flags);
   debug ("acall = %d\n", rc);

   return Py_BuildValue("i", rc);
};

static PyObject * fetch(PyObject * self, PyObject * args)
{
   char * rdata = NULL;
   int hdl = 0, rlen = 0, apprc = 0;
   int rc, flags = 0;

   if (!PyArg_ParseTuple(args, "i|i", &hdl, &flags))
   {
      return NULL;
   } 

   rc = mwfetch(&hdl, &rdata, &rlen, &apprc, flags);
   debug ("fetch = %d hdl=%d\n", rc, hdl);
   
   if (rc >= 0) 
      return Py_BuildValue("(iiis#)", rc, hdl, apprc, rdata, rlen);
   else
      return Py_BuildValue("(i)", rc);
};

static PyObject * listsvc(PyObject * self, PyObject * args)
{
   char * glob = NULL;
   char ** list;
   int i;
   int rc, flags = 0;
   PyObject* rettuple, *value;

   if (!PyArg_ParseTuple(args, "|si", &glob, &flags))
   {
      return NULL;
   } 

   if (glob == NULL) glob = "*";

   rc = mwlistsvc(glob, &list, flags);
   if (rc < 0) {
      return NULL;
   };
   
   debug ("rc = %d\n", rc);
   rettuple = PyTuple_New(rc);
   for (i = 0; i < rc; i++) {
      value = Py_BuildValue("s", list[i]);
      PyTuple_SET_ITEM(rettuple, i, value);
   };

   return rettuple;
};

/************************************************************************/

static PyObject * openlog(PyObject * self, PyObject * args)
{
   char * prog, * file = NULL;
   int rc, level = MWLOG_INFO;

   if (!PyArg_ParseTuple(args, "s|si", &prog, &file, &level))
   {
      return NULL;
   } 

   mwopenlog(prog, file, level);
   Py_INCREF(Py_None); 
   return Py_None;
};

static PyObject * setloglevel(PyObject * self, PyObject * args)
{
   int rc, level = -1;

   if (!PyArg_ParseTuple(args, "|i", &level))
   {
      return NULL;
   } 

   rc = mwsetloglevel(level);
   return Py_BuildValue("i", rc);
};

static PyObject * mw_log(PyObject * self, PyObject * args)
{
   char * mesg;
   int level;

   if (!PyArg_ParseTuple(args, "is", &level, &mesg))
   {
      return NULL;
   } 

   mwlog(level, mesg);
   Py_INCREF(Py_None); 
   return Py_None; 
};


/************************************************************************/

struct pyprovide {
   int svcid;
   char service[MWMAXSVCNAME];
   PyObject * pyfunc;
};

static struct pyprovide * providedlist = NULL;
static int numprovided = 0;

static PyObject * provide(PyObject * self, PyObject * args)
{
   char * service;
   PyObject * pyfunc;
   int rc, svcid, idx;

   rc = _mw_isattached();
   if (rc != 1) {
      debug("provide aborts we're not attached to an instance");
      return Py_BuildValue("i", rc);
   };
   rc = _mwsystemstate();
   if (rc != 0) {
      debug("provide aborts because instance is in shutdown state");
      return Py_BuildValue("i", rc);
   };

   if (!PyArg_ParseTuple(args, "sO", &service, &pyfunc)) {
      return NULL;
   } 

   svcid = _mw_ipc_provide(service,0);
   idx = numprovided++;
   debug("svc %s has id %x adding in entry %d", service, svcid, idx);
   _mw_incprovided();

   providedlist = realloc(providedlist, sizeof(struct pyprovide) * numprovided);
   providedlist[idx].svcid = svcid;
   strncpy(providedlist[idx].service, service, MWMAXSVCNAME);
   providedlist[idx].pyfunc = pyfunc;
   Py_XINCREF(pyfunc);

   return Py_BuildValue("i", svcid);
};

static PyObject * unprovide(PyObject * self, PyObject * args)
{
   int rc, svcid, idx, i;
   char * service;

   rc = _mw_isattached();
   if (rc != 1) {
      debug("provide aborts we're not attached to an instance");
      return Py_BuildValue("i", rc);
   };
   rc = _mwsystemstate();
   if (rc != 0) {
      debug("provide aborts because instance is in shutdown state");
      return Py_BuildValue("i", rc);
   };

   if (!PyArg_ParseTuple(args, "s", &service))
   {
      return NULL;
   } 

   idx = -1;
   for (i = 0; i < numprovided; i++) {
      if (strcmp(providedlist[i].service, service) == 0) {
	 idx = i;
	 break;
      };
   };
   if (idx == -1) {
      debug("provide aborts because service is not provided");
      return Py_BuildValue("i", -ENOENT);
   };
   debug("found provided entry at %d: SVC=%s svcid = %x", 
	 idx, providedlist[idx].service, providedlist[idx].svcid);

   Py_XDECREF(providedlist[idx].pyfunc);
   rc = _mw_ipc_unprovide(providedlist[idx].service, providedlist[idx].svcid);
   debug("ipc unprovide = %d", rc);

   numprovided--;
   if (numprovided == 0) {
      debug("last provide, removing list");
      free(providedlist);
      providedlist = NULL;
   } else {
      debug("moving last provide entry from %d to %d", numprovided, idx);
      memcpy(&providedlist[idx], &providedlist[numprovided], sizeof(struct pyprovide));
   };
   
   _mw_decprovided();
   return Py_BuildValue("i", rc);
};

static int replypending = 0; 

static PyObject * forward(PyObject * self, PyObject * args)
{
   char * svc;
   char * data;
   int dlen;
   int rc, flags = 0;

   if (!PyArg_ParseTuple(args, "s|s#i", &svc, &data, &dlen, &flags))
   {
      return NULL;
   } 

   rc = mwforward(svc, data, dlen, flags);
   debug ("forward = %d\n", rc);
   replypending = 0;
   
   return Py_BuildValue("i", rc);
};

static PyObject * reply(PyObject * self, PyObject * args)
{
   char * data = NULL; 
   int dlen = 0, apprc = 0, rcode = MWSUCCESS, rc, flags = 0;

   if (!PyArg_ParseTuple(args, "i|s#i", &rcode, &data, &dlen, &apprc))
   {
      return NULL;
   } 
   
   if ( (rcode == MWSUCCESS) || (rcode == MWFAIL) ) {
      debug("replypending is now 0");
      replypending = 0;
   };

   debug ("replying: %d %s (%d) %d %d", rcode, data, dlen, apprc, flags);
   rc =  mwreply(data, dlen, rcode, apprc, flags);
   debug ("replypending = %d", replypending);
   return Py_BuildValue("i", rc);
};
  
static inline int do_service(int flags)
{
   PyObject * pysi;
   mwsvcinfo * si;
   int i, rc;
   long rcl;
   struct pyprovide * pyprov = NULL;
   PyObject *result;
   mwsvcinfo * _mwGetServiceRequest(int);

   si = _mwGetServiceRequest(flags);
   if (si == NULL) {
      return -errno;
   };
   
   for (i = 0; i < numprovided; i++) {
      if (providedlist[i].svcid == si->svcid) {
	 pyprov = & providedlist[i];
	 break;
      }
   };

   if (!pyprov) return -ENOENT;

   pysi = Py_BuildValue("({s:i,s:s#,s:i,s:s,s:i,s:i,s:i,s:i,s:i,s:s,s:s})", 
			"handle", si->handle,
			"data", si->data, si->datalen, 
			"flags", si->flags,
			"service", si->service, 
			"deadline", si->deadline,
			"udeadline", si->udeadline,
			"clientid", si->cltid,
			"serviceid", si->svcid, 
			"authentication", si->authentication,
			"username", si->username, 
			"clientname", si->clientname);

   replypending = 1;
   debug("calling service handler");
   result = PyEval_CallObject(pyprov->pyfunc, pysi);
   
   Py_DECREF(pysi);
   debug("done result=%p replypending=%d", result, replypending);

   if (!result) {
      Error("Failed to call Service handler!");
      mwreply(NULL, 0, MWFAIL, 0, 0);
      replypending = 0;
      return 0;
   };

   if (PyInt_Check(result)) {
      rcl = PyInt_AsLong(result);
   } else if ( PyLong_Check(result)) {
      rcl = PyLong_AsLong(result);
   } else if (PyFloat_Check(result)) {
      rcl = (long) PyFloat_AsDouble(result);
   } else if (Py_None == result) {
      rcl = 0;
   } else {
      Error("Service handler didn't return a valid type!");
      return -EINVAL;
   }
   Py_DECREF(result);
   rc = (int) rcl;
   
   debug("rc = %d pending = %d", rc, replypending);
   if (replypending) {
      mwreply(NULL, 0, rc, 0, 0);
   };
   
   return rc;
};

static PyObject * servicerequest(PyObject * self, PyObject * args)
{
   int flags = 0, rc;
   
   if (!PyArg_ParseTuple(args, "|i", &flags)) {
      return NULL;
   } 
   
   rc = do_service(flags);   
   return Py_BuildValue("i", rc);
};


static PyObject * mainloop(PyObject * self, PyObject * args)
{
   int flags = 0, rc;

   if (!PyArg_ParseTuple(args, "|i", &flags)) {
      return NULL;
   } 

   while(1) {
      rc = do_service(flags);   
      
      if (rc < 0) {	 
	 if (rc == -EINTR) {

	    if (_mw_tasksignalled()) {
	       mwdotasks();
	       continue;
	    };
	    if (flags & MWSIGRST) continue;
	 };
	 return Py_BuildValue("i", rc);
      };
   };
};
      
// PyObject* PyErr_SetFromErrno(PyObject *type)


/************************************************************************/

struct subscribed_events {
   char * pattern;
   int subscriptionid;   
   int flags;
   PyObject * function;
} * subs = NULL;

typedef  struct subscribed_events  subscribed_events_t;

static int nsubs = 0, lastsubid = 0;

static inline subscribed_events_t * findsubent(int subid)
{
    int i;
    if (subs == NULL) return NULL;

    for (i = 0; i < nsubs; i++) {
       if (subs[i].subscriptionid == subid) {
	  return &subs[i];
       };
    };
    return NULL;
};

static inline subscribed_events_t * allocsubent(void)
{
   subscribed_events_t * se = NULL;
   
   se = findsubent(-1);

   if (se) {
      se->subscriptionid = lastsubid++;
      debug("assigned free subent subid %d", se->subscriptionid);
      return se;
   };

   subs = realloc(subs, sizeof(subscribed_events_t) * (nsubs+1));
   debug("alloced new subent with index %d to subid %d", nsubs, subs[nsubs].subscriptionid);
   subs[nsubs].subscriptionid = lastsubid++;
   return &subs[nsubs++];
};
 
static inline void releasesubent(int subid)
{
   subscribed_events_t * se = NULL;
   
   se = findsubent(subid);
   if (se) {
	 debug("sub with id %d found ", subid);
	 se->subscriptionid = -1;
	 free(se->pattern);
	 Py_XDECREF(se->function);
	 return;
   };

   debug("no sub with id %d found", subid);
   return;
};

static PyObject * subscribe(PyObject * self, PyObject * args)
{
   char * pattern; 
   int flags; 
   int error = 0;
   int rc;
   PyObject * pyfunc;
   subscribed_events_t * se;

   rc = _mwsystemstate();
   if (rc != 0) {
      debug("provide aborts because instance is in shutdown state");
      return Py_BuildValue("i", rc);
   };

   if (!PyArg_ParseTuple(args, "sO|i", &pattern, &pyfunc, &flags)) {
      return NULL;
   } 

   debug("SUBSCRIBE %p %x", pattern, flags);
   
   se = allocsubent();

   debug ("%s %d %d", pattern, se->subscriptionid, flags);

   rc =  _mwsubscribe(pattern, se->subscriptionid, flags);
   
   if (rc >= 0) {
      debug("subscription OK");
      Py_XINCREF(pyfunc);

      se->pattern = strdup(pattern);
      se->flags = flags;
      se->function = pyfunc;

      return Py_BuildValue("i", se->subscriptionid);
   };

   debug("subscription failed rc=%d", rc);
   error = rc;   
   releasesubent(se->subscriptionid);

   return Py_BuildValue("i", error);
};


static PyObject * unsubscribe(PyObject * self, PyObject * args)
{
  subscribed_events_t * se = NULL;
  int error = 0;
  int i, rc;
  int subid;


  if (!PyArg_ParseTuple(args, "i", &subid)) {
     return NULL;
  } 

  if (subid < 0) {
     return Py_BuildValue("i", -EINVAL);
  };

  se = findsubent(subid);

  if (se == NULL) {
     return Py_BuildValue("i", -ENOENT);
  };

  debug("UNSUBSCRIBE %d", se->subscriptionid);
  rc =  _mwunsubscribe(se->subscriptionid);

  releasesubent(se->subscriptionid);

  if (rc >= 0) {
    debug("unsubscription OK");    
    return Py_BuildValue("i", rc);
  };


  return Py_BuildValue("i", -rc);

};


static PyObject * event(PyObject * self, PyObject * args)
{
   char * event;
   char * data = NULL;
   int datalen = 0;
   char * username = NULL;
   char * clientname = NULL;
   int rc;

   if (!PyArg_ParseTuple(args, "s|s#ss", &event, &data, &datalen, &username, &clientname)) {
      return NULL;
   } 
   
   rc = mwevent(event, data, datalen, username, clientname);
   
   if (rc >= 0) {
      debug("event OK");    
      return Py_BuildValue("i", rc);
   };
   
   return Py_BuildValue("i", -rc);
};

static PyObject * recvevents(PyObject * self, PyObject * args)
{
   if (!PyArg_ParseTuple(args, "")) {
      return NULL;
   } 

   mwrecvevents();

   Py_INCREF(Py_None); 
   return Py_None;
};


/* called after receiving a IPC of SRB event message, actually
   executes the event handler */
static void _mw_doevent_py(int subid, char * event, char * data, int datalen)
{
  PyObject *result;
  PyObject * pyargs;
  subscribed_events_t * se;

  debug("attempting to find callback for event %s subid %d", event, subid);
  
  se = findsubent(subid);

  if (se) {
      
     debug("Found callback calling with data = %p, datalen = %d", data, datalen);
     pyargs = Py_BuildValue("(ss#)", event, data, datalen);

     result = PyEval_CallObject(se->function, pyargs);

     if (!result) {
	Error ("Call of handler for subscription id %d returned NULL", subid);
     } else {
	Py_DECREF(result);
     };
     Py_DECREF(pyargs);

  };
  
  return;
};



/************************************************************************/

// Tasks
static struct _taskmap {
   PTask pt;
   PyObject * function;
} * taskmap = NULL;

typedef  struct _taskmap taskmap_t;

static int ntaskmap = 0;

static inline taskmap_t * findtaskent(PTask pt)
{
    int i;
    if (taskmap == NULL) return NULL;

    for (i = 0; i < ntaskmap; i++) {
       if (taskmap[i].pt == pt) {
	  return &taskmap[i];
       };
    };
    return NULL;
};

static inline taskmap_t * alloctaskent(void)
{
   taskmap_t * te = NULL;
   
   te = findtaskent(-1);

   if (te) {
      debug("assigned free taskmap ent");
      return te;
   };

   taskmap = realloc(taskmap, sizeof(taskmap_t) * (ntaskmap+1));
   debug("alloced new taskmap ent with index %d", ntaskmap);
   return &taskmap[ntaskmap++];
};
 
static inline void releasetaskent(PTask pt)
{
   taskmap_t * te = NULL;
   
   te = findtaskent(pt);
   if (te) {
      debug("task with id %ld found", pt);
      te->pt = -1;
      Py_XDECREF(te->function);
      return;
   };

   debug("no task with id %ld found", pt);
   return;
};

// we route all task via a task wrapper. pt is the task index number +
// PTASKMAGIC, which we use to loop the python task map to find the
// pyobject * to the python function to call.

static int task_dispatcher(PTask pt)
{
   taskmap_t * te = NULL;
   long rcl;
   PyObject *result;
   PyObject * pyargs;
   
   // 
   
   te = findtaskent(pt);

   if (!te) {
      debug("no such task found");
      return 0;
   };

   pyargs = Py_BuildValue("()");
   result = PyEval_CallObject(te->function, pyargs);
   Py_DECREF(pyargs);

   if (!result) {
      mwlog(MWLOG_WARNING, "Task handler did not return a value!");
      return 0;
   }; 
   debug("got result");

   if (PyInt_Check(result)) {
      rcl = PyInt_AsLong(result);
   } else if ( PyLong_Check(result)) {
      rcl = PyLong_AsLong(result);
   } else if (PyFloat_Check(result)) {
      rcl = (long) PyFloat_AsDouble(result);
   } else if (Py_None == result) {
      rcl = 0;
   } else {
      Error("Service handler didn't return a valid type!");
      return -EINVAL;
   }
   Py_DECREF(result);
   debug ("returning %ld", rcl);
   return rcl;
};

static PyObject * addtask(PyObject * self, PyObject * args)
{
   PyObject * pyfunc;
   int interval, delay = 0;
   PTask pt;
   taskmap_t * te = NULL;

   if (!PyArg_ParseTuple(args, "Oi|i", &pyfunc, &interval, &delay)) {
      return NULL;
   } 
   
   pt = _mwaddtaskdelayed (task_dispatcher, "python task", interval, delay);
   
   te = alloctaskent();
   
   Py_INCREF(pyfunc);
   te->function = pyfunc;
   te->pt = pt;
   
   return Py_BuildValue("l", pt);
};

static PyObject * waketask(PyObject * self, PyObject * args)
{
   PTask pt;

   if (!PyArg_ParseTuple(args, "l", &pt)) {
      return NULL;
   } 

   return Py_BuildValue("i", mwwaketask(pt));
};

static PyObject * settaskinterval(PyObject * self, PyObject * args)
{
   PTask pt;
   int interval;
   
   if (!PyArg_ParseTuple(args, "li", &pt, &interval)) {
      return NULL;
   } 
   
   return Py_BuildValue("i", mwsettaskinterval(pt, interval));
};

static PyObject * dotasks(PyObject * self, PyObject * args)
{
   if (!PyArg_ParseTuple(args, "")) {
      return NULL;
   } 
   
   mwdotasks();
   Py_INCREF(Py_None); 
   return Py_None;
};



/************************************************************************/


static struct PyMethodDef midway_methods[] = {
   { "attach",       attach,      METH_VARARGS, "attaching to a midway instance, Takes three optional params, URL, client name and flags." }, 
   { "detach",       detach,      METH_VARARGS, "detach the connection to an attached midway instance, no arguments." }, 
   { "geturl",       geturl,      METH_VARARGS, "lookup of the URL we're attached to, takes no arguments, return the string" }, 
   { "listsvc",      listsvc,     METH_VARARGS, "listing all available services, take one optional argument, a glob for matching service names, default \"*\", return a tuple" }, 
   { "call",         call,        METH_VARARGS, "performing a service call, takes one mandatory argument, the service name, and two optional arguments, data, and flags. Return a tuple, on failure only contain the return code, on success a tuple with return code, application return code and data." }, 
   { "acall",        acall,       METH_VARARGS, "doing an asynchronous service call, take the same one mandatory and two optional arguments as call (service, data, flags) and return the call handle." }, 
   { "fetch",        fetch,       METH_VARARGS, "fetching the reply from an asynchronous service call, take one mandatory argument, the handle returned from a acall, or 0 to get the first available of any reply, and a optional flags argument. Fetch return a tuple, which on failure only contain the return code. On success the tuple contain the returncode, the call handle of the reply, application return code, and data." }, 
   { "openlog",      openlog,     METH_VARARGS, "initiates the log. Some what modeled on syslog(3)/openlog(3). Take one mandatory argument the program name, the optionally a file prefix and a flags argument. See mwlog(3) for details." }, 
   { "setloglevel",  setloglevel, METH_VARARGS, "alters the current loglevel, take one optional argument, the new loglevel, return the previous loglevel. " }, 
   { "log",          mw_log,      METH_VARARGS, "add a log message to the log. Takes two mandatory arguments, the loglevel of the message, and the message string." }, 

   // events
   { "subscribe",    subscribe,   METH_VARARGS, 
     "subscribe to events, takes two mandatory arguments; the event pattern and the event handler method. And optionally a flags argument. Returns the subscription ident. See mwsubscribe(3)" }, 
   { "unsubscribe",  unsubscribe, METH_VARARGS, "Unsubscribes a previous subscribe, takes the subscription ident as argument." }, 
   { "event",        event,       METH_VARARGS, "Post's an event, take one mandatory argument, the event name, then optionally data, a username and clientname. See mwevent(3C) for details." }, 
   { "recvevents",   recvevents,  METH_VARARGS, "Explicitly polls for received events, the handler call the handler function given in subscribe. This is implicitly done in mainloop and in every call/fetch. Take no arguments return nothing" }, 

   // tasks
   { "addtask",      addtask,     METH_VARARGS, "Adds a task to be run on intervals. Takes two mandatory arguments, first of all the method to be called on intervals, and the interval on milliseconds, -1 is legal for never see waketask. This is followed by an optional initial interval before first task. Return an integer, if positive task handle and a negative is a negative errno. The task method gets not arguments and must return either TRUE or FALSE. If TRUE it will be called again immediately (unless there has been event to handle or service requests in the mean time), when returning FALSE, it not called again until next interval, or waketask is called. Note that the tasks are primarily intended for servers, see dotasks()" }, 
   { "waketask",     waketask,    METH_VARARGS, "Explicitly wake a task, take one mandatory argument, the task handle returned by add task." }, 
   { "settaskinterval", settaskinterval, METH_VARARGS, "Change the task interval, take two mandatory arguments, the first the the task handle returned by addtask, and the second is the new interval in milliseconds. Use -1 to suspend a task. Return the old interval, or error if negative." }, 
   { "dotasks",      dotasks,     METH_VARARGS, "Implicitly called by mainloop, runs all the tasks pending, and return when they're done. This method is provided for those that want to roll their own mainloops, like in a client. Takes no arguments, will always return None." }, 

   // server only api:
   { "provide",      provide,     METH_VARARGS, "Provide a service, takes two mandatory arguments the service name, and the python method to be called for this service. Returns the service id." }, 
   { "unprovide",    unprovide,   METH_VARARGS, "Unprovides a previously provided service, takes one mandatory argument, the service id returned by provide." }, 
   { "reply",        reply,       METH_VARARGS, "Sends a reply to service request, this method only make sense within a service call method (the method given to provide. It takes one mandatory argument, return code which must be either midway.MWSUCCESS, midway.MWFAIL, or midway.MWMORE. Followed by two optional arguments, data, and application return code." }, 
   { "forward",      forward,     METH_VARARGS, "forwards a service request onto another service with new data. Takes one mandatory and two optional arguments (like acall) (service, data, flags) and return 0 on success or -errno." }, 
   { "servicerequest", servicerequest, METH_VARARGS, "This is a function needed if you're rolling your own main loop. It does a single service request, then return. It takes one optional flags argument." }, 
   { "mainloop",     mainloop,    METH_VARARGS, "A server's mainloop, return only if an internal error, or if the midway instance is going down. This is what you call after a servers initialization routine where you attach, and provide(s) services. It's good practice to do the unprovides and detach in case of its return." }, 
   
   //   { "", , METH_VARARGS, "[]" }, 
   { NULL, NULL, 0, NULL }
};

// ret values
static PyObject * success, * fail, * more;
// flags
static PyObject * noreply, * noblock, * notime, * sigrst, * notran, * unique, *multiple;
// event flags
static PyObject * evstring, * evglob, * evregexp, * everegexp;
// attach flags
static PyObject * client, * server, * serveronly;

// log levels
static PyObject * log_fatal, 
   * log_error, 
   * log_warning, 
   * log_alert, 
   * log_info, 
   * log_debug, 
   * log_debug1, 
   * log_debug2, 
   * log_debug3, 
   * log_debug4;

void initmidway() 
{
   PyObject * m, *d;
   
   m = Py_InitModule3("midway", midway_methods, "Bindings for MidWay see MidWay(7) and releated sections 3C docs for details");
   d = PyModule_GetDict(m);

   success = Py_BuildValue("i", MWSUCCESS);
   fail = Py_BuildValue("i", MWFAIL);
   more = Py_BuildValue("i", MWMORE);
   PyDict_SetItemString(d, "MWSUCCESS", success);
   PyDict_SetItemString(d, "MWFAIL", fail);
   PyDict_SetItemString(d, "MWMORE", more);

   // flags
   noreply = Py_BuildValue("i", MWNOREPLY);
   PyDict_SetItemString(d, "MWNOREPLY", noreply);
   noblock = Py_BuildValue("i", MWNOBLOCK);
   PyDict_SetItemString(d, "MWNOBLOCK", noblock);
   notime = Py_BuildValue("i", MWNOTIME);
   PyDict_SetItemString(d, "MWNOTIME", notime);
   sigrst = Py_BuildValue("i", MWSIGRST);
   PyDict_SetItemString(d, "MWSIGRST", sigrst);
   notran = Py_BuildValue("i", MWNOTRAN);
   PyDict_SetItemString(d, "MWNOTRAN", notran);
   unique = Py_BuildValue("i", MWUNIQUE);
   PyDict_SetItemString(d, "MWUNIQUE", unique);
   multiple = Py_BuildValue("i", MWMULTIPLE);
   PyDict_SetItemString(d, "MWMULTIPLE", multiple);

   // event flags
   evstring = Py_BuildValue("i", MWEVSTRING);
   PyDict_SetItemString(d, "MWEVSTRING", evstring);
   evglob = Py_BuildValue("i", MWEVGLOB);
   PyDict_SetItemString(d, "MWEVGLOB", evglob);
   evregexp = Py_BuildValue("i", MWEVREGEXP);
   PyDict_SetItemString(d, "MWEVREGEXP", evregexp);
   everegexp = Py_BuildValue("i", MWEVEREGEXP);
   PyDict_SetItemString(d, "MWEVEREGEXP", everegexp);

   // attach flags
   client = Py_BuildValue("i", MWCLIENT);
   PyDict_SetItemString(d, "MWCLIENT", client);
   server = Py_BuildValue("i", MWSERVER);
   PyDict_SetItemString(d, "MWSERVER", server);
   serveronly = Py_BuildValue("i", MWSERVERONLY);
   PyDict_SetItemString(d, "MWSERVERONLY", serveronly);

   // log levels
   log_fatal = Py_BuildValue("i", MWLOG_FATAL);
   PyDict_SetItemString(d, "MWLOG_FATAL", log_fatal);
   log_error = Py_BuildValue("i", MWLOG_ERROR);
   PyDict_SetItemString(d, "MWLOG_ERROR", log_error);
   log_warning = Py_BuildValue("i", MWLOG_WARNING);
   PyDict_SetItemString(d, "MWLOG_WARNING", log_warning);
   log_alert = Py_BuildValue("i", MWLOG_ALERT);
   PyDict_SetItemString(d, "MWLOG_ALERT", log_alert);
   log_info = Py_BuildValue("i",  MWLOG_INFO );
   PyDict_SetItemString(d, "MWLOG_INFO", log_info);
   log_debug = Py_BuildValue("i", MWLOG_DEBUG);
   PyDict_SetItemString(d, "MWLOG_DEBUG", log_debug);
   log_debug1 = Py_BuildValue("i", MWLOG_DEBUG1);
   PyDict_SetItemString(d, "MWLOG_DEBUG1", log_debug1);
   log_debug2 = Py_BuildValue("i", MWLOG_DEBUG2);
   PyDict_SetItemString(d, "MWLOG_DEBUG2", log_debug2);
   log_debug3 = Py_BuildValue("i", MWLOG_DEBUG3);
   PyDict_SetItemString(d, "MWLOG_DEBUG3", log_debug3);
   log_debug4 = Py_BuildValue("i", MWLOG_DEBUG4);
   PyDict_SetItemString(d, "MWLOG_DEBUG4", log_debug4);

   /*
   = Py_BuildValue("i", );
   PyDict_SetItemString(d, "", );
   = Py_BuildValue("i", );
   PyDict_SetItemString(d, "", );
   */

   _mw_register_event_handler(_mw_doevent_py);


};

/* 
 * Local variables:
 *  compile-command: "./setup.py build "
 * End:
 */


