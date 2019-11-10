package org.midway;

/**

*/

import java.util.Date;

public abstract class ServiceProvider {

    private String service;

    private long mdeadline;
    
    private int authentication;
    private String username;
    private String clientname;


    private boolean isProvided = false;
    private long providedServiceID = -1L; 
    private String providedName;

    public ServiceProvider(String serviceName) throws MidWayException {
        super();
        provide(serviceName);
    }
    
    public String getServiceName() {
        return service;
    }

    public Date getDeadLine() {
        return new Date(mdeadline/1000);
    }

    public double getTimeOut() {
        double d = Double.MAX_VALUE;
        if (this.mdeadline == 0) return d;

        long now = System.currentTimeMillis();
        return now - mdeadline;
    }
    
    public int getAuthentication() {
        return authentication;
    }

    public String getUserName() {
        return username;
    }

    public String getClientName() {
        return clientname;
    }


    /**
       This is the default formater for exceptions in service
       handlers. On exception from the service handler we send a replymessage
       with FAIL returncode and the output from this method as data.
       Applications should override this to format a message that adhere to
       the error message format defined in the client-server protocol.
       
       @param e The excepion thrown from an ServiceHandler
       
       @return a message about the error that the client will understand. In this implementation, a human understandable message. 
    */
    protected String encodeException(Exception e) {
        return "Service call threw Exception " + e.getClass().toString() + ", Cause: " + e.getMessage();
    }

    /**
       This is the method that subclasses of ServiceProvider must implement. It will be called on the reception of a service request.  
    */
    protected abstract int serviceHandler(byte[] callData) throws MidWayException ;
    
    /**
       This is the wrapper method the JNI C code call for a service
       request. We have this wrapper to deal with exceptions from the
       serviceHandler in Java.
     */
    protected final int _serviceHandler(byte[] callData) {
        
        try {
            return serviceHandler(callData) ;
        } catch (Exception e) {
            String errorReply = encodeException(e);
            try {
                replyFail(errorReply);
            } catch (Exception e1) {
                // pass
            }
            return 0;
        }
    }


    
    protected synchronized void provide() throws MidWayException {
        String fullClsName = this.getClass().getName();
        String parts[] = fullClsName.split("\\.");
        String clsName = parts[parts.length-1];
        provide (clsName);        
    };
        
    protected synchronized void provide(String servicename) throws MidWayException {
        if (isProvided) throw new MidWayException("Already provided");
        providedName = servicename;
        providedServiceID = MidWay.provide(servicename, this);
        isProvided = false;
    };
        
    protected synchronized void unprovide() throws MidWayException {
        if (!isProvided) throw new MidWayException("not provided");
        MidWay.unprovide(providedName, providedServiceID);
        isProvided = false;        
    };
        
    protected void reply(byte[] replyData, int returnCode, int applicationReturnCode) throws MidWayException {
        MidWay.reply(replyData, returnCode, applicationReturnCode);
    }

    protected void reply(String replyData, int returnCode, int applicationReturnCode) throws MidWayException {
        reply(replyData.getBytes(), returnCode, applicationReturnCode);
    }

    protected void reply(byte[] replyData, int returnCode) throws MidWayException {
        MidWay.reply(replyData, returnCode, 0);
    }

    protected void reply(String replyData, int returnCode) throws MidWayException {
        reply(replyData.getBytes(), returnCode, 0);
    }

    protected void replyMore(byte[] replyData, int applicationReturnCode) throws MidWayException {
        MidWay.reply(replyData, MidWay.MORE, applicationReturnCode);
    }

    protected void replyMore(String replyData, int applicationReturnCode) throws MidWayException {
        reply(replyData.getBytes(), MidWay.MORE, applicationReturnCode);
    }

    protected void replyMore(byte[] replyData) throws MidWayException {
        MidWay.reply(replyData, MidWay.MORE, 0);
    }

    protected void replyMore(String replyData) throws MidWayException {
        reply(replyData.getBytes(), MidWay.MORE, 0);
    }

    protected void replySuccess(byte[] replyData, int applicationReturnCode) throws MidWayException {
        MidWay.reply(replyData, MidWay.SUCCESS, applicationReturnCode);
    }

    protected void replySuccess(String replyData, int applicationReturnCode) throws MidWayException {
        reply(replyData.getBytes(), MidWay.SUCCESS, applicationReturnCode);
    }

    protected void replySuccess(byte[] replyData) throws MidWayException {
        MidWay.reply(replyData, MidWay.SUCCESS, 0);
    }

    protected void replySuccess(String replyData) throws MidWayException {
        reply(replyData.getBytes(), MidWay.SUCCESS, 0);
    }

    protected void replyFail(byte[] replyData, int applicationReturnCode) throws MidWayException {
        MidWay.reply(replyData, MidWay.FAIL, applicationReturnCode);
    }

    protected void replyFail(String replyData, int applicationReturnCode) throws MidWayException {
        reply(replyData.getBytes(), MidWay.FAIL, applicationReturnCode);
    }

    protected void replyFail(byte[] replyData) throws MidWayException {
        MidWay.reply(replyData, MidWay.FAIL, 0);
    }

    protected void replyFail(String replyData) throws MidWayException {
        reply(replyData.getBytes(), MidWay.FAIL, 0);
    }

    protected void finalize() {
        try {
            unprovide();
        } catch (MidWayException e) {
            ;
        }
        
        try {
            super.finalize();
        } catch (Throwable e) {
            ;
        }
    }
}
