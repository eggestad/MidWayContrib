package org.midway;

public abstract class MidWayCallRequester implements org.midway.ICallListener {

    private boolean nonblocking = false;
    private boolean noreply = false;
    private boolean multiple = false;

    private int requestHandle;
 
    private byte[] requestData;
    private String serviceName;

    public void sendRequest() {
        int flags = 0;
        flags |= (nonblocking ? MidWay.NONBLOCK : 0);
        flags |= (noreply ? MidWay.NOREPLY : 0);
        flags |= (multiple ? MidWay.MULTPLE : 0);

        requestHandle = MidWay.acall(serviceName, requestData, flags);
        
        if (! noreply) {
            MidWay.addReplyListener(requestHandle, this);
    };


};
