/*
  MidWay
  Copyright (C) 2018 Terje Eggestad

  MidWay is free software; you can redistribute it and/or
  modify it under the terms of the GNU  General Public License as
  published by the Free Software Foundation; either version 2 of the
  License, or (at your option) any later version.
  
  MidWay is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.
  
  You should have received a copy of the GNU General Public License 
  along with the MidWay distribution; see the file COPYING.  If not,
  write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
  Boston, MA 02111-1307, USA. 
*/
   
 
#include <MidWay.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <assert.h>

void cleanup(void)
{
  mwdetach();
};

void sighandler(int sig)
{
   printf("exit in signal %d", sig);
   exit(-sig);
};

#define Debug(fmt, ...)    mwlog(MWLOG_DEBUG, "%s(%d) " fmt, __FUNCTION__, __LINE__, ## __VA_ARGS__ )
#define Error(fmt, ...)    mwlog(MWLOG_ERROR, "%s(%d) " fmt, __FUNCTION__, __LINE__, ## __VA_ARGS__ )
#define Info(fmt, ...)    mwlog(MWLOG_INFO, "%s(%d) " fmt, __FUNCTION__, __LINE__, ## __VA_ARGS__ )


void usage(void) 
{
  fprintf(stderr, "mw_shell_server [-l loglevel] [-A uri] [-L logprefix] [-n name] {service cmdasnndline}...\n");
  fprintf(stderr, "    loglevel is one of error, warning, info, debug, debug1, debug2\n");
  fprintf(stderr, "    uri is the address of the MidWay instance e.g. ipc:12345\n");
  fprintf(stderr, "    logprefix is the path inclusive the beginning of the file name the logfile shall have.\n");
  fprintf(stderr, "    service commandline ties a service to the commandline.\n");
  exit(-1);
};

extern char *optarg;
extern int optind, opterr, optopt;
char * uri = NULL;
char * servername = NULL;
char * libdir, * rundir;

struct svc_cmd_pair {
   char * svcname;
   char * commandline;
   struct svc_cmd_pair * next;
} * servicelist = NULL;

char * replybuffer = NULL;
size_t replybufferlen = 0;

int svc_wrapper (mwsvcinfo * msi) {

   mwlog(MWLOG_INFO,"starting service call for service %s", msi->service);
   int rc;
   
   struct svc_cmd_pair * elm = servicelist;
   while (elm != NULL) {

      // found entry, now execute 
      if (strncmp(elm->svcname, msi->service, MWMAXSVCNAME) == 0) {

	 Info("found commandline %s", elm->commandline);
	 char command[strlen(elm->commandline) + 100];
	 
	 char * tempfile = NULL;

	 if (msi->data != NULL) {

	    Info("writing data to tempfile");
	    tempfile = tmpnam(NULL);
	    int fd = open (tempfile, O_CREAT | O_WRONLY|O_TRUNC, 0600);
	    assert(fd > 0);
	    
	    rc = write(fd, msi->data, msi->datalen);
	    assert (rc == msi->datalen);
	    rc = close(fd);
	    assert (rc == 0);
	    sprintf(command, "cat %s | %s", tempfile, elm->commandline);
	 } else {
	    sprintf(command, "%s", elm->commandline);
	 }

	 Debug("command is %s", command);

	 FILE * fp  = popen(command, "r");

	 size_t readbytes = 0, offset = 0;
	 Debug("read reply of len %d of bufferlen, %d error=%d eof=%d errno=%d %lx", readbytes, replybufferlen, ferror(fp), feof(fp), errno, replybuffer);

	 readbytes = fread(replybuffer, sizeof(char), replybufferlen, fp);
	 Debug("read reply of len %d %d error=%d eof=%d errno=%d", readbytes, replybufferlen, ferror(fp), feof(fp), errno);

	 while (readbytes == replybufferlen) {
	    Debug("resizing reply buffer for next bit");
	    offset = readbytes;
	    size_t nextlen = replybufferlen;
	    replybufferlen *= 2;
	    replybuffer = realloc(replybuffer, replybufferlen);
	    readbytes += fread(replybuffer+offset, sizeof(char), nextlen, fp);
	    Debug("read reply of total len %d", readbytes);
	 };
	 replybuffer[readbytes] = '\0';
	 rc = pclose(fp);
	 
	 Info( "replying %d bytes of data pclose returned %d", readbytes, rc);
	 
	 char * combuf = mwalloc(readbytes);
	 memcpy(combuf, replybuffer, readbytes);
	 
	 rc = mwreply(combuf, readbytes, MWSUCCESS, rc, 0);
	 if (rc != 0)
	    Error("mwreply returned %d", rc);

	 return MWSUCCESS;
      }
      elm = elm->next;
   }
   mwlog(MWLOG_ERROR,"unable to find service handler");
   return MWFAIL;
}

	    
   
int main(int argc, char ** argv)
{
  char option;
  int rc, i, loglevel;
  char * svcname, * funcname, * tmpp, * logprefix = "mw_shell_server";
  char  * mwhome, * instance;
  
#ifdef DEBUGGING
  loglevel = MWLOG_DEBUG2;
#else 
  loglevel = MWLOG_INFO;
#endif
  replybufferlen = 1024;
  replybuffer = malloc(replybufferlen);

       
  while ((option = getopt(argc, argv, "vl:L:A:n:")) != EOF) {
    
    switch (option) {
      
    case 'l':
       rc = _mwstr2loglevel(optarg);
       if (rc == -1) usage();
       loglevel = rc;
       break;

    case 'v':
       mwsetloglevel(MWLOG_DEBUG);
       loglevel = MWLOG_DEBUG;
       Debug("mwserver client starting");
       break;

    case 'A':
      uri = strdup(optarg);
      break;

    case 'L':
      logprefix = optarg;
      break;

    case 'n':
      servername = optarg;
      break;

    case '?':
      usage();
    }
  }
  Debug("coptind %d argc %d", optind, argc);

  mwopenlog(argv[0], logprefix, loglevel);

  Debug("doptind %d argc %d", optind, argc);
  /* all the options are parsed, whar now remain on the comamnd line is the libraries.
     If there are none complain. */
  if (optind >= argc) {
  Debug("optind %d argc %d", optind, argc);
    Error("No services specified.");
    usage();
  };
  Debug("optind %d argc %d", optind, argc);

  if ((argc - optind) % 2 != 0)   {
    Error(" services must be in name commandline pairs.");
    usage();
    exit(-1);
  }

  
  signal(SIGTERM, sighandler);
  signal(SIGQUIT, sighandler);
  signal(SIGINT, sighandler);

  atexit(cleanup);

  if (servername == NULL) {
    servername = (char *) malloc(20);
    sprintf(servername, "([%d])", getpid());
  };

  rc = mwattach(uri, servername, MWSERVER);  
  Debug("mwattached on uri=\"%s\" returned %d\n", uri, rc);
  if (rc < 0) {
    Error("attached failed");
    exit(rc);
  };


  Debug("optind %d argc %d", optind, argc);
  for (i = optind; i < argc; i += 2 ) {
     Debug(" %s => %s", argv[i], argv[i+1]);
    
     struct svc_cmd_pair * newelm = malloc(sizeof(struct svc_cmd_pair));
     newelm->svcname = argv[i];
     newelm->commandline = argv[1+i];
     newelm->next = servicelist ;
     servicelist = newelm;

     mwprovide( argv[i], svc_wrapper, 0);
  };
  
  mwMainLoop(0);
  
  mwdetach();
  Debug("detached\n");
}









