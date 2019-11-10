
    package org.midway;

    /**
       This is the java interface to the MidWay API. Please refer to the C
       man pages for the exact details.  The MidWay C API currently do not
       have an explisit context, only a single implied one, hence you cannot
       instantiate the MidWay class for multiple MidWay instances. For
       this reason all methods are class methods.

       @author Terje Eggestad
    */

    public class MidWay {

        /**
           Service call return value.
           Service calls shall return only one of FAIL, SUCCESS, or MORE.
           In future XA distributed transaction will be rolled back on FAIL.
         */
        public static final int FAIL = 0;
        /**
           Service call return value.
           Service calls shall return only one of FAIL, SUCCESS, or MORE.
        */
        public static final int SUCCESS = 1;
        /**
           Service call return value.
           Service calls shall return only one of FAIL, SUCCESS, or MORE.
           MORE mean that there are more replies comming, this may only happen iff the acall() have the MULTIPLE flags set.
        */
        public static final int MORE = 2;


        /**
           Flag for acall(), indicated that there shall be no reply to the call.
           This also mean that there will not be an indication on failure.
         */
        public static final int NOREPLY =   0x00000001;
        /**
           Flag to indicate that an API method shall not block if it meet
           a blocking condition, but rather return with an exception.
         */
        public static final int NOBLOCK =   0x00000002;
        public static final int NOTIME =    0x00000004;
        /**
           Flag to indicate that if a native call is interrupted, it shall
           restart, but return with an exception.
         */
        public static final int SIGRST =    0x00000008;
        public static final int NOTRAN =    0x00000010;
        public static final int UNIQUE =    0x00000020;
        /**
           Flag for acall, indicates that is the service handler call
           reply many times, each reply shall be returned seperatly with
           fetch.  Otherwise all the replies is concatenated into a single
           reply.
         */
        public static final int MULTIPLE =  0x00000040;


        /**
           The IPC only flag was introduced for the GW, inorder to ensure that
           incoming service calls aren't routed back out. As such this flag may
           disappear whenever remote domains are implemented.

           However it *might* be a nice feature for high performance apps,
           that want to make sure local services only are called.  (in
           this case the service in question should begin with . )
        */
        public static final int IPCONLY =   0x00000080; /* only allow local IPC services to e selected */;


    /* Flags for conv */
        public static final int CONV =      0x00010000;
        public static final int STDIO =     0x00020000;

    /* flags for events */
        public static final int EVSTRING =  0x00000000; // exact matches;
        public static final int EVGLOB =    0x00100000; // matches according to normal shell filename wildcards ?* etc (see fnmatch);
        public static final int EVREGEXP =  0x00200000; // POSIX regular expression (see regex(7));
        public static final int EVEREGEXP = 0x00400000; // extended regular express ----- || -----;

    /* flags for mwattach() */
        public static final int CLIENT =    0x00000000;
        public static final int NOTCLIENT = 0x01000000;
        public static final int SERVER =    0x02000000;
        public static final int SERVERONLY =0x03000000;
        /**
           Internal use only
         */
        public static final int GATEWAY =   0x04000000;

        public static final int AUTH_NONE     = 0;  /** user has not been authenticated */
        public static final int AUTH_PASSWORD = 1;  /** plain old password */
        public static final int AUTH_X509     = 2;  /** x509 certificate (ssl/tls level auth) */
        public static final int AUTH_KRB5     = 3;  /** kerberos */

        public static final int LOG_FATAL     = 0;
        public static final int LOG_ERROR     = 1;
        public static final int LOG_WARNING   = 2;
        public static final int LOG_ALERT     = 3;
        public static final int LOG_INFO      = 4;
        public static final int LOG_DEBUG     = 5;
        public static final int LOG_DEBUG1    = 6;
        public static final int LOG_DEBUG2    = 7;
        public static final int LOG_DEBUG3    = 8;
        public static final int LOG_DEBUG4    = 9;

        public static final int BROKERPORT = 1102;

        private force_java_impl = false;

        public String clientname = "java";
        public String midwayurl = null;
        private int flags = 0;
        private MidWayImpl impl = null;
        public MidWay() {

        }

        public MidWay(String name, String url, int flags) {
            clientname = name;
            midwayurl = url;
            this.flags = flags;
        }


        /**
           Attaches to a MidWay instance, note that the MidWay C library
           currently only support connecting to one, that's the reason all
           the methods here are class methods.

           See also man mwattach(3C)
           @param name the client name
           @param url the url for the MidWay instance to attach to, format are ipc:<b>ipckey</b> or srbp:<b>[//hostname]/domain</b>. If null, autodetection is attempted, see man mwattach(3C)
           @return 0 on success
           @throws MidWayException
        */
        public void attach(String name, String url, int flags) throws MidWayException {
            if (! force_java_client) MidWayImpl impl = load_native_impl() ;
        }

        /**
         * Try to attach to a MidWay instance using default parameters

           Attaches to a MidWay instance, note that the MidWay C library
           currently only support connecting to one, that's the reason all
           the methods here are class methods.

           See also man mwattach(3C).
           @return 0 on success
           @throws MidWayException
        */
        public void attach() throws MidWayException {
            attach(clientname, midwayurl, flags);
        }

        /**
           Attaches to a MidWay instance, note that the MidWay C library
           currently only support connecting to one, that's the reason all
           the methods here are class methods.
           See also man mwattach(3C)

           @param url the url for the MidWay instance to attach to, format are ipc:<b>ipckey</b> or srbp:<b>[//hostname]/domain</b>. If null, autodetection is attempted.
           @return 0 on success
           @throws MidWayException
        */
        public void attach(String url) throws MidWayException {
            attach(url, null, 0);
        };

        public void attachServer() throws MidWayException {
            attach(null, null, MidWay.SERVER);
        };

        public void attachServer(String url) throws MidWayException {
            attach(url, null, MidWay.SERVER);
        };



        /**
           Detaches from attached instance.

           See also man mwdetach(3C)
         */
        public int detach();

        /**
           Get the actuall URL attached to after attach().
           @return the URL
         */
        public String getURL();

        /**
            list all available services matching glob
            Hidden services which start with . are not included by default.

            @param glob a glob expression, may be null (see man glob(7)
            @return a list of services or null if there are no services matching
        */
        public String[] listServices(String glob) throws MidWayException ;

        /**
            list all available services (this do not include hidden services which start with .)

            @param glob a glob expression, may be null (see man glob(7)
            @return a list of services or null if there are no services matching
        */
        public String[] listServices() throws MidWayException  {
            return listServices(null);
        };


        /************************************************************************
         *
         * service call API
         *
         ************************************************************************/

        /**
           Perform a synchronous service call.

           @param service  the name if the service to call
           @param data a byte array to send (this is the most effient form)
           @param flags is oone of NOREPLY, NOBLOCK, SIGRST

           @return a CallReply object of null on failure
           @throws MidWayException
         */
        public CallReply call(String service, byte[] data, int flags) throws MidWayException {
            int hdl = acall(service, data, flags);

            if ((flags & MidWay.NOREPLY) != 0) return null;
            CallReply cr = new CallReply();

            boolean brc = MidWay.fetch(hdl, cr, 0);
            return cr;
        };


        public CallReply call(String service) throws MidWayException {
            return call(service, (byte []) null, 0);
        };

        public CallReply call(String service, String data, int flags) throws MidWayException {
            return call(service, data.getBytes(), flags);
        };

        public CallReply call(String service, String data) throws MidWayException {
            return call(service, data.getBytes(), 0);
        };

        public CallReply call(String service, int flags) throws MidWayException {
            return call(service, (byte []) null, flags);
        };

        /**
           Perform a asynchronous call
           @param service the name of the service to call
           @param data a byte array to send (this is the most effient form)
           @param flags   NOREPLY, NOBLOCK
           @return a handle to be used with fetch() to get replies
           @throws MidWayException
         */
        public int acall(String service, byte[] data, int flags) throws MidWayException;

        public int acall(String service) throws MidWayException {
            return acall(service, (byte [])null, 0);
        };

        public int acall(String service, String data, int flags) throws MidWayException {
            return acall(service, data.getBytes(), flags);
        };

        public int acall(String service, String data) throws MidWayException {
            return acall(service, data.getBytes(), 0);
        };

        public int acall(String service, int flags) throws MidWayException {
            return acall(service,(byte []) null, flags);
        };

        /**
           Fetches replies from an acall.

           @param handle the handle returned by acall, or -1 for the first available, the CallReply will always hold the actual handle
           @param flags  may be MULTIPLE if not MULTIPLE all the replies from the server is concatenated into one fetch, else you get the same number of replies that the server sent
           @return the CallReply.returncode
        */
        public boolean fetch(int handle, CallReply rpl, int flags) throws MidWayException;


    //     private class CallListener {
    //         int handle;
    //         ICallListener listener;
    //     }

    //     static private int callListenerSize = 0;
    //     static private CallListener[] callListners = null;

    //     static public void addReplyListener(int handle, ICallListener listener) {

    //         if (listener == null) {
    //             throw new IllegalArgumentException();
    //         }

    //         if (callListenerSize == 0) {
    //             callListners = new CallListener[8];
    //         } else {
    //             // check for duplicates using identity
    //             for (int i = 0; i < callListenerSize; ++i) {
    //                 if (callListners[i] != null && callListners[i].handle == handle) {
    //                     return;
    //                 }
    //             }
    //             // grow array if necessary
    //             if (callListenerSize == callListners.length) {
    //                 System.arraycopy(callListners, 0,
    //                         callListners = new CallListener[callListenerSize * 2], 0, callListenerSize);
    //             }
    //         }

    //         callListners[callListenerSize].listener = listener;
    //         callListenerSize++;
    //     };

    //     static public void removeReplyListener(int handle) {

    //         for (int i = 0; i < callListenerSize; ++i) {
    //             if (callListners[i] != null && callListners[i].handle == handle) {
    //                 if (callListenerSize == 1) {
    //                     callListners = null;
    //                     callListenerSize = 0;
    //                 } else {
    //                     System.arraycopy(callListners, i + 1, callListners, i, --callListenerSize
    //                                     - i);
    //                     callListners[callListenerSize] = null;
    //                 }
    //                 return;
    //             }
    //         }
    //     }

        /************************************************************************
         *
         * Server Service provider  API
         *
         ************************************************************************/

        public int provide(String servicename, ServiceProvider provider) throws MidWayException;
        public int unprovide(String servicename, long subscriptionID);

        public int forward(String service, byte[] reply) throws MidWayException;
        public int reply(byte[] reply, int returnCode, int applicationReturnCode) throws MidWayException;

        public int doServiceRequest(int flags);

        private Object mainloopmutex = new Object();

        public void mainServerLoop() throws MidWayException {
            while(true) {
                doServiceRequest(0);
            }
        }
        /************************************************************************
         *
         * event API
         *
         ************************************************************************/

        public native int event(String eventname, byte[] data, String username, String clientname);

        public int event(String eventname, byte[] data) {
            return event(eventname, data, null, null);
        };

        public int event(String eventname, String data) {
            return event(eventname, data.getBytes(), null, null);
        };

        public int event(String eventname, String data, String username, String clientname) {
            return event(eventname, data.getBytes(), username, clientname);
        };

        public int event(String eventname, String username, String clientname) {
            return event(eventname, new byte[0], username, clientname);
        };


        public native int subscribe(String event, IEventListener evl, int flags) throws MidWayException;

        public int subscribe(String event, IEventListener evl) throws MidWayException {
            return subscribe(event, evl, 0);
        }


        public native void recvevents();


        /************************************************************************
         *
         * logging API
         *
         ************************************************************************/


        /**
           log a message thru the MidWay logging API.
           @param level the Log level, one of the constants MidWay.LOG_*
           @param message the message to add to the log
        */
        public void log(int level, String message);

        /**
           log a message thru the MidWay logging API at loglevel  MidWay.LOG_FATAL
           Fatal messgages are intened to log cause for an application going down abnormally.
           @param message the message to add to the log
        */
        public void fatal(String msg) {
            log(LOG_FATAL, msg);
        }

        /**
           log a message thru the MidWay logging API at loglevel  MidWay.LOG_ERROR
           Error messages are used to indicate an error condition the application can recover from.
           @param message the message to add to the log
        */
        public void error(String msg) {
            log(LOG_ERROR, msg);
        }

        /**
           log a message thru the MidWay logging API at loglevel  MidWay.LOG_WARNING
           Warning messages are indended for messages that indicate that unnormal condition exist,
           one that may create errors if worsens, but the application did not produce any wrong result.
           @param message the message to add to the log
        */
        public void warning(String msg) {
            log(LOG_WARNING, msg);
        }

        /**
           log a message thru the MidWay logging API at loglevel  MidWay.LOG_ALERT
           Alert messages are intended for the system admin of condition that may cause problems in imminent future,
           like running out or low on critical resources.
           @param message the message to add to the log
        */
        public void alert(String msg) {
            log(LOG_ALERT, msg);
        }

        /**
           log a message thru the MidWay logging API at loglevel  MidWay.LOG_INFO
           Info messages are intended for application wanting to have a trace of normal execution.
           @param message the message to add to the log
        */
        public void info(String msg) {
            log(LOG_INFO, msg);
        }

        /**
           log a message thru the MidWay logging API at loglevel  MidWay.LOG_DEBUG
           Intended for application debugging, this logvlevel is intended for use by the application for standard debugging trace.
           @param message the message to add to the log
        */
        public void debug(String msg) {
            log(LOG_DEBUG, msg);
        }

        /**
           log a message thru the MidWay logging API at loglevel  MidWay.LOG_DEBUG1
           This loglevel is to be considered reserved by the MidWay middleware, all standard debugging from the MidWay libraries happen on this level.
           @param message the message to add to the log
        */
        public void debug1(String msg) {
            log(LOG_DEBUG1, msg);
        }

        /**
           log a message thru the MidWay logging API at loglevel  MidWay.LOG_DEBUG2
           Intended for application debugging, this logvlevel is intended for use by the application for verbose debugging trace.
           @param message the message to add to the log
        */
        public void debug2(String msg) {
            log(LOG_DEBUG2, msg);
        }

        /**
           log a message thru the MidWay logging API at loglevel  MidWay.LOG_DEBUG3
           This loglevel is to be considered reserved by the MidWay middleware, all verbose debugging from the MidWay libraries happen on this level.
           @param message the message to add to the log
        */
        public void debug3(String msg) {
            log(LOG_DEBUG3, msg);
        }

        /**
           log a message thru the MidWay logging API at loglevel  MidWay.LOG_DEBUG4
           Intended for application debugging, this logvlevel is intended for use by the application for extra verbose debugging trace.
           @param message the message to add to the log
        */
        public void debug4(String msg) {
            log(LOG_DEBUG4, msg);
        }

        /**
           changes the loglevel, it returns the old loglevel.
           @param level one of the MidWay.LOG_* constants, or -1 which do not changed the loglevel while return the current.
           @return the loglevel before it's changed.
        */
        public int setloglevel(int level);

        /**
           Initialize the MidWay logging API. See man mwopenlog(3C)

           @param progname The progname is prefixed each logline.
           @param fileprefix if an relative path the logfile will appear in the instance log dir, else use an absolute path.
           @param loglevel logging messages at this level and below is logged, messages above are ignored.
         */
        public void openlog(String progname, String fileprefix, int level);



    };
