

import org.midway.MidWay;
import org.midway.MidWayException;
import org.midway.Event;
import org.midway.CallReply;
import org.midway.IEventListener;
import junit.framework.TestCase;
import junit.framework.Assert;

public class test extends TestCase implements IEventListener {
    Event lastEvent = null;

    public void doEvent(Event ev) {
        System.out.println("event = "+ ev);
        lastEvent = ev;
    };


    static {
        try {
            MidWay.attach();
        } catch (Exception e) {
            
        };
    }
    protected void setUp() throws MidWayException{ 
        int logLevel = MidWay.LOG_INFO;
        logLevel = MidWay.LOG_DEBUG3;
        MidWay.openlog("test.java", "./testlog", logLevel);

        System.out.println(MidWay.class);

    }


    protected void tearDown() {
        //        int i = MidWay.detach();
        //System.out.println("detach = "+ i);
    }

    public void testGetUrl() {
        MidWay.info("+++++++++ doing testGetUrl");

        String url =  MidWay.getURL();
        assertNotNull(url);
        System.out.println("url = "+ url);

        MidWay.info("=== SUCCESS === testGetUrl");
    }

    public void testListServices() throws MidWayException {

        MidWay.info("+++++++++ doing testListServices");
        
        String[] svc = MidWay.listServices();
        assertNotNull(svc);
        System.out.print(svc.length);
        assertTrue(svc.length  > 0);

        System.out.print("[");
        for (String s : svc) {
            System.out.print(s + " ");
        }
        System.out.println("]");
        
        svc = MidWay.listServices("ping*");
        assertNotNull(svc);
        System.out.print(svc.length);
        assertTrue(svc.length  > 0);
        System.out.print("[");
        for (String s : svc) {
            System.out.print(s + " ");
            }
        System.out.println("]");


        svc = MidWay.listServices("nomatch");
        assertNotNull(svc);
        System.out.print(svc.length);
        assertTrue(svc.length == 0);

        MidWay.info("=== SUCCESS === testListServices");
    }
    
    public void testCall() throws MidWayException {

        MidWay.info("+++++++++ doing testCall");

        int hdl  = MidWay.acall("shell", "uptime");
        System.out.println("handle = "+ hdl);
        
        CallReply cr = new CallReply();

        boolean brc = MidWay.fetch(hdl, cr, MidWay.MULTIPLE);
        
        System.out.println("brc = "+ brc +" rc= " +cr.returncode);
        System.out.println("Data: \""  + new String(cr.data) + "\"");
        
        brc = MidWay.fetch(hdl, cr, MidWay.MULTIPLE);
        
        System.out.println("brc = "+ brc +" rc= " +cr.returncode);
        System.out.println("Data: \""  + new String(cr.data) + "\"");
        
        
        cr = MidWay.call("shell", "id");
        System.out.println(" rc= " +cr.returncode);
        System.out.println("Data: \""  + new String(cr.data) + "\"");

        MidWay.info("=== SUCCESS === testCall");
        
    }

    public void testCall2() throws MidWayException {
        testCall();
        testCall();
        testCall();
    }

    public void testEvent() throws MidWayException, InterruptedException {

        MidWay.info("+++++++++ doing testEvent");

        int i;
        final String eventname = "event1";        
        final String eventdata = "event data";

        MidWay.event("hei", "data");
        
        i = MidWay.subscribe("event1", this);
        System.out.println("subscribe = "+ i);

        MidWay.event(eventname, eventdata);        
        Thread.sleep(5000);
        
        MidWay.recvevents();
        System.out.println("done");
        assertNotNull(lastEvent);
        
        assertTrue(i == lastEvent.subscriptionID);
        assertTrue(eventname.equals(lastEvent.eventName));
        assertTrue(eventdata.equals(new String(lastEvent.eventData)));

        MidWay.info("=== SUCCESS === testEvent");
        
    }


    public static void main(String args[]) {

        MidWayException ee = new MidWayException("test");  
        System.out.println(ee);
        System.out.println(MidWay.class);

    };
};
