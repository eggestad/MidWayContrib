package org.midway;


public interface ICallListener {
    public void onReply(int returnCode, byte[] replydata, int applicationReturnCode);
};
