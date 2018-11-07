

import org.midway.*;

public class TestServer extends ServiceProvider {

    public int  serviceHandler(byte[] callData) throws MidWayException {
        try {
            String data = "";
            if (callData != null)  
                data = new String(callData);
            System.out.println("got call data: " + data);
            
            System.out.println("auth:=" + getAuthentication() + " user:=" + getUserName()  + " client:=" + getClientName());
            
            replySuccess ("This is my first reply from java" + data, 42);
        } catch (MidWayException e) {
            e.printStackTrace();
            replyFail ("This is my fail first reply from java", 42);
        }
        return MidWay.FAIL;
    }

    public TestServer(String serviceName) throws MidWayException {
        super(serviceName);
    }

    public static void main(String args[]) {

        int logLevel = MidWay.LOG_INFO;
        //        logLevel = MidWay.LOG_DEBUG;
        MidWay.openlog("TestServer", "./testserverlog", logLevel);

        
        try {
            
            MidWay.attachServer();
            TestServer ts = new TestServer("javasvc");
            MidWay.mainServerLoop();
        } catch (MidWayException e) {
            e.printStackTrace();
        }
    }
}
