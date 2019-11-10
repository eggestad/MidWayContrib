package org.midway.impl.java;

import java.io.ByteArrayOutputStream;
import java.io.UnsupportedEncodingException;
import java.net.URLDecoder;
import java.text.ParseException;
import java.util.Arrays;
import java.util.HashMap;


/**
 * 
 * This class hold a SRB Protocol message in a form that allow us easily to 
 * inspect and create messages.
 *  
 *  ** Please see the Service Request Broker (SRC) Protocol specification. 
 *  
 * This class also parse and encode the SRB messages received of the transport layer.
 * 
 * This is a subclass of HashMap which hold the key value pairs in a SRB message, 
 * as well and fields for holding the SRB command word and marker
 * 
 * @author terje
 *
 */

public class SRBMessage extends HashMap<String, byte[]> {
	// The SRB version number, required in SRB INIT. 
    public static final String SRBPROTOCOLVERSION =    "0.9";

    // The maximum total length of an SRB message, liable to increase. 
    public static final int  SRBMESSAGEMAXLEN  =        3500;

    // all the legal SRB commands
    public static final String SRB_INIT       = "SRB INIT";
    public static final String SRB_READY      = "SRB READY";
    public static final String SRB_SVCCALL  =   "SVCCALL"  ;
    public static final String SRB_SVCDATA  =   "SVCDATA"  ;
    public static final String SRB_TERM  =      "TERM"  ;
    public static final String SRB_PROVIDE  =   "PROVIDE"  ;
    public static final String SRB_UNPROVIDE  = "UNPROVIDE";
    public static final String SRB_HELLO  =     "HELLO";
    public static final String SRB_EVENT  =     "EVENT";
    public static final String SRB_SUBSCRIBE  = "SUBSCRIBE";
    public static final String SRB_UNSUBSCRIBE ="UNSUBSCRIBE";
    public static final String SRB_REJECT  =    "REJECT";

    // all the legal SRB markers
    public static final char SRB_REQUESTMARKER  =       '?';
    public static final char SRB_RESPONSEMARKER  =      '.';
    public static final char SRB_NOTIFICATIONMARKER  =  '!';
    public static final char SRB_REJECTMARKER  =        '~';

    // all the parameters that may possible occur in all the possible SRB commands. 
    public static final String SRB_PARAM_AGENT =  "AGENT";
    public static final String SRB_PARAM_AGENTVERSION =  "AGENTVERSION";
    public static final String SRB_PARAM_APPLICATIONRC =  "APPLICATIONRC";
    public static final String SRB_PARAM_AUTHENTICATION =  "AUTHENTICATION";
    public static final String SRB_PARAM_CAUSE =  "CAUSE";
    public static final String SRB_PARAM_CAUSEFIELD =  "CAUSEFIELD";
    public static final String SRB_PARAM_CAUSEVALUE =  "CAUSEVALUE";
    public static final String SRB_PARAM_CLIENTNAME =  "CLIENTNAME";
    public static final String SRB_PARAM_CONVERSATIONAL =  "CONVERSATIONAL";
    public static final String SRB_PARAM_DATA =  "DATA";
    public static final String SRB_PARAM_DATATOTAL =  "DATATOTAL";
    public static final String SRB_PARAM_DOMAIN =  "DOMAIN";
    public static final String SRB_PARAM_EVENTNAMEEVENTID =  "EVENTNAMEEVENTID";
    public static final String SRB_PARAM_GLOBALTRANID =  "GLOBALTRANID";
    public static final String SRB_PARAM_GRACE =  "GRACE";
    public static final String SRB_PARAM_HANDLE =  "HANDLE";
    public static final String SRB_PARAM_HOPS =  "HOPS";
    public static final String SRB_PARAM_INSTANCE =  "INSTANCE";
    public static final String SRB_PARAM_LOCALTIME =  "LOCALTIME";
    public static final String SRB_PARAM_MAXHOPS =  "MAXHOPS";
    public static final String SRB_PARAM_MESSAGE =  "MESSAGE";
    public static final String SRB_PARAM_MORE =  "MORE";
    public static final String SRB_PARAM_MULTIPLE =  "MULTIPLE";
    public static final String SRB_PARAM_NAME =  "NAME";
    public static final String SRB_PARAM_OFFSET =  "OFFSET";
    public static final String SRB_PARAM_OS =  "OS";
    public static final String SRB_PARAM_PASSWORD =  "PASSWORD";
    public static final String SRB_PARAM_REASON =  "REASON";
    public static final String SRB_PARAM_REASONCODE =  "REASONCODE";
    public static final String SRB_PARAM_REJECTS =  "REJECTS";
    public static final String SRB_PARAM_REMOTETIME =  "REMOTETIME";
    public static final String SRB_PARAM_RETURNCODE =  "RETURNCODE";
    public static final String SRB_PARAM_REVERSE =  "REVERSE";
    public static final String SRB_PARAM_SECTOLIVE =  "SECTOLIVE";
    public static final String SRB_PARAM_SVCNAME =  "SVCNAME";
    public static final String SRB_PARAM_TYPE =  "TYPE";
    public static final String SRB_PARAM_UNIQUE =  "UNIQUE";
    public static final String SRB_PARAM_USER =  "USER";
    public static final String SRB_PARAM_VERSION =  "VERSION";
    
    public static final String SRB_MESSAGEBOUNDRY  = "\r\n";

    protected String command ;
    protected char marker;
    
    /**
     * clear this instance as if it was just created with new SRBMessage()
     */
    public void clear() {
        super.clear();
        this.command = null;
        this.marker = ' ';
    }
    
    /** 
     * A simple check if the instance contains the bare minimum of what it takes to make a 
     * properly formated SRB message.  
     * @return
     * @throws IllegalStateException
     */
    protected boolean isValid()  throws IllegalStateException {
        if (command == null)
            throw new IllegalStateException("missing command");
        // TODO: check for legal commands

        if (this.marker == SRB_REQUESTMARKER
            || this.marker == SRB_RESPONSEMARKER
            || this.marker == SRB_NOTIFICATIONMARKER
            || this.marker == SRB_REJECTMARKER) 
            throw new IllegalStateException("illegal marker command");
        return true;
    }
    private static  String regexMarker = "[" + "\\" + SRB_NOTIFICATIONMARKER + 
    		"\\" + SRB_REQUESTMARKER + 
    			"\\"  + SRB_RESPONSEMARKER + 
    			"\\"  + SRB_REJECTMARKER + 
    			"]";
    
    /** 
     * Take an SRB message from the transport layer, parse it and return an instance with the 
     * message
     * @param messagestream the SRB message without the trailing \r\n
     * @return 
     * @throws ParseException
     */
    public int parse(String messagestream) throws ParseException {
    	clear();
    	//regexMarker = "[^\\s]";
    	//regexMarker = "[^A-Z ]";
    	try {
			//Timber.d("parsing %s %s", messagestream, regexMarker);		
			String[] firstsplit = messagestream.split(regexMarker, 2);
			if (firstsplit.length != 2) throw new ParseException("Message have no marker", 0);
			command = firstsplit[0];
			marker = messagestream.charAt(command.length());
			
			String fields = URLDecoder.decode(firstsplit[1], "utf-8");
			command = firstsplit[0];
			
			String[] params = firstsplit[1].split("&");
			for (String s : params) {
				String[] kv = s.split("=");
				put(kv[0], URLDecoder.decode(kv[1], "utf-8").getBytes());
			}
			//Timber.d("parsed %s", this);
		} catch (UnsupportedEncodingException e) {
			Timber.e("can't happen, Java RE don't understand utf-8");
		}
    	return -1;
    	
    }
   
    
//    public void send(BufferedOutputStream bbos) throws IOException {
//    	Timber.d("sending message %s", this);
//    	try {
//			Timber.d("datalen %d", this.getData().length);
//		} catch (Exception e) {
//			 
//		}
//
//    	byte[] byteArray = encode();
//    	Timber.d("sending message (%d) %s", byteArray.length, new String(byteArray));
//    	bbos.write(byteArray);
//    	bbos.flush();
//
//    }

    /**
     * Take the instance and format it to a byte[] to be sent on transport layer
     * @return
     */
	public byte[] encode() {
		ByteArrayOutputStream bos = new ByteArrayOutputStream();

    	byte[] barr;
    	barr = command.getBytes();
    	bos.write(barr, 0, barr.length);
    	bos.write(marker);
    	boolean first = true;
    	for (String k : keySet()) {
    		if (first) 
    			first = false;
    		else
    			bos.write('&');

    		barr = k.getBytes();
    		bos.write(barr, 0, barr.length);
    		bos.write('=');
    		barr = get(k);
    		String s = byteArrayToURLString(barr);
    		barr = s.getBytes();
    		bos.write(barr, 0, barr.length);
    	}
    	bos.write('\r');
    	bos.write('\n');
    	byte[] byteArray = bos.toByteArray();
		return byteArray;
	}

    protected void setCommand(String command) {
        this.command = command;
    }
    protected String getCommand() {
        return this.command;
    }
     
    public void put(String value, int i) {
		put(value, String.format("%d", i));		
	}

	public void put(String key, String value) {		
		put(key, value.getBytes());
	}
   
	
	// methods to make legal SRB messages 
	// see SRB protocol spec.   
	
	/**
	 * make a SRB TERM message.
	 * @param grace
	 * @return an instance of SRBMessage with the given parameters
	 */
    public static SRBMessage  makeTerm(int grace) {
    	SRBMessage msg = new SRBMessage();
    	msg.command = SRB_TERM;
    	msg.marker = SRB_NOTIFICATIONMARKER;
    	msg.put(SRB_PARAM_GRACE, grace);
    	return msg;
    }

    /**
     * make a SRB INIT request
     * @param name
     * @param domain
     * @param instance
     * @return an instance of SRBMessage with the given parameters
     */
    public static SRBMessage  makeInitReq(String name, String domain, String instance) {
    	if (name == null) name = "";
    	SRBMessage msg = new SRBMessage();
    	msg.command = SRB_INIT;
    	msg.marker = SRB_REQUESTMARKER;
    	msg.put(SRB_PARAM_TYPE, "client");
    	msg.put(SRB_PARAM_VERSION, SRBPROTOCOLVERSION);
    	
    	if (name != null)
    		msg.put(SRB_PARAM_NAME, name); 
    	if (instance != null)
    		msg.put(SRB_PARAM_INSTANCE, instance);
    	if (domain != null)
    		msg.put(SRB_PARAM_DOMAIN, domain);
    	
    	return msg;
    }

    /**
    * Make a SRB SVCCALL message, 
    * @param svcname the service name, may not be null
    * @param data data passed to the service, may be null
    * @param datatotal the total length of data for the call. Shall be 0 if data contain the 
    * full data, or null
    * @param handle a unique (at least there can not be any other calls in progress with the 
    * same handle) handle. may be zero if this is a call that shall not get a reply.
    * @param multiple flag to indicate that the server responding may send multiple replies.
    * @param secstolive 
    * @return an instance of SRBMessage with the given parameters
    */
    public static SRBMessage  makeCallReq(String svcname, byte[] data, int datatotal, 
    		long handle, boolean multiple, int secstolive) {
    	
    	Timber.d("makeing call req mesg: %s dlen=%d, chunks=%d, handle=%d", svcname, data.length, datatotal, handle);
    	if (svcname == null) throw new IllegalArgumentException("service name missing");
    	if (data == null && datatotal > 0) throw new IllegalArgumentException("no data with chunks");
    	if (handle == 0 && datatotal > 0) throw new IllegalArgumentException("no handle with chunks");
    	
    	SRBMessage msg = new SRBMessage();
    	msg.command = SRB_SVCCALL;
    	msg.marker = SRB_REQUESTMARKER;
		msg.put(SRB_PARAM_SVCNAME, svcname);
		msg.put(SRB_PARAM_HOPS, 0);

    	if (data != null && data.length > 0)
    		msg.put(SRB_PARAM_DATA, data);
    	if (datatotal > 0)
    		msg.put(SRB_PARAM_DATATOTAL, datatotal);
    	if (multiple)
    		msg.put(SRB_PARAM_MULTIPLE, "yes");
    	if (secstolive > 0)
    		msg.put(SRB_PARAM_SECTOLIVE, secstolive);
    	if (handle != 0) 
    		msg.put(SRB_PARAM_HANDLE, String.format("%08x", handle));
    	else 
    		msg.marker = SRB_NOTIFICATIONMARKER;
    	Timber.d("made %s", msg);
    	return msg;
    }

    /**
     * make a SRB SVCDATA message
     * @param svcname MidWay service name
     * @param data application data, may be binary
     * @param handle the handle used in the SVCCALL message 
     * @return an instance of SRBMessage with the given parameters
     */
    public static SRBMessage  makeData(String svcname, byte[] data, long handle) {
    	if (svcname == null) throw new IllegalArgumentException("service name missing");
    	if (data == null) throw new IllegalArgumentException("no data with data message");
    	if (handle == 0) throw new IllegalArgumentException("no handle with datamessage");
    	SRBMessage msg = new SRBMessage();
    	msg.command = SRB_SVCDATA;
    	msg.marker = SRB_NOTIFICATIONMARKER;
		msg.put(SRB_PARAM_DATA, data);
		msg.put(SRB_PARAM_HANDLE, String.format("%08x", handle));
 
    	return msg;
    }
    
    /**
     * Make a SRB SUBSCRIBE message
     * @param regexp the regexp that 
     * @param unsubscribe if true cancel a previous subscribe
     * @return
     */
    public static SRBMessage  makeSubscribeReq(String regexp, boolean unsubscribe) {
    	if (regexp == null) throw new IllegalArgumentException("missing regexp");
    	SRBMessage msg = new SRBMessage();
    	if(unsubscribe)
    		msg.command = SRB_UNSUBSCRIBE;
    	else 
    		msg.command = SRB_SUBSCRIBE;
    	msg.marker = SRB_NOTIFICATIONMARKER;
    	msg.put(SRB_PARAM_NAME, regexp);
    	
    	return msg;
    }

    // helper functions to do urlencoding and url decoding    
	protected  static byte[] byteArrayFromURLString(String s) {
    	byte[] in = s.getBytes();
    	byte[] res = new byte[in.length];
    	int reslen = 0;
    	for (int i = 0; i < in.length; i++) {
    		if (in[i] == '+') res[reslen++] = ' ';
    		else if (in[i] == '%') {
    			byte high = nibble(in[++i]);
    			byte low = nibble(in[++i]);
    			res[reslen++] = (byte) (low | (byte) high << 4 );    		
    		}
    		else res[reslen++] = in[i];
    	}
    	System.out.println("in len " + s.length() +" reslen " + reslen);
    	return Arrays.copyOf(res, reslen);
    }

	/**
	 * Helper function that give us the binary value of a HEX character   
	 * @param b a byte in the range of 0-9 or A-F
	 * @return a value between 0 and 0xf
	 */
    protected static byte nibble(byte b) {
    	switch (b) {

    	case '0':
    		return 0;
    	case '1':
    		return 1;
    	case '2':
    		return 2;
    	case '3':
    		return 3;
    	case '4':
    		return 4;
    	case '5':
    		return 5;
    	case '6':
    		return 6;
    	case '7':
    		return 7;
    	case '8':
    		return 8;
    	case '9':
    		return 9;


    	case 'A':
    		return 0xA;
    	case 'B':
    		return 0xB;
    	case 'C':
    		return 0xC;
    	case 'D':
    		return 0xD;
    	case 'E':
    		return 0xE;
    	case 'F':
    		return 0xF;

    	}
    	throw new NumberFormatException("Not a hex char: " + b ); 
    }
    
    /**
     * escape data according to SRB protpcol spec using URL encoding. 
     * @param in byte array that can contain binary data
     * @return url encoded string
     */
    protected static String byteArrayToURLString(byte in[]) {
        byte ch = 0x00;
        int i = 0;
        if (in == null || in.length <= 0)
          return null;

        String pseudo[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
            "A", "B", "C", "D", "E", "F" };
        StringBuffer out = new StringBuffer(in.length * 2);

        while (i < in.length) {
          // First check to see if we need ASCII or HEX
          if ((in[i] >= '0' && in[i] <= '9')
              || (in[i] >= 'a' && in[i] <= 'z')
              || (in[i] >= 'A' && in[i] <= 'Z')  
              || in[i] == '_' || in[i] == '.'
              ) {
            out.append((char) in[i]);
            i++;
          } else if ( in[i] == ' ') {
              out.append('+');
              i++;
          } else {
            out.append('%');
            ch = (byte) (in[i] & 0xF0); // Strip off high nibble
            ch = (byte) (ch >>> 4); // shift the bits down
            ch = (byte) (ch & 0x0F); // must do this is high order bit is
            // on!
            out.append(pseudo[ch]); // convert the nibble to a
            // String Character
            ch = (byte) (in[i] & 0x0F); // Strip off low nibble
            out.append(pseudo[ch]); // convert the nibble to a
            // String Character
            i++;
          }
        }

        String rslt = new String(out);

        return rslt;

      }

    // getters
    
	public long getHandle() {
		byte[] sHdl = get(SRB_PARAM_HANDLE);
		if (sHdl == null) return 0;
		String s = new String(sHdl);
		return Long.parseLong(s, 16);
	}

	public byte[] getData() {
		return get(SRB_PARAM_DATA);		
	}

	public Integer getDataTotal() {
		byte[] sHdl = get(SRB_PARAM_DATATOTAL);
		if (sHdl == null) return null;
		String s = new String(sHdl);
		return Integer.parseInt(s);
	}

	public Integer getAppRC() {
		byte[] sHdl = get(SRB_PARAM_APPLICATIONRC);
		if (sHdl == null) return null;
		String s = new String(sHdl);
		return Integer.parseInt(s);	
	}

	public Integer getReturnCode() {
		byte[] sHdl = get(SRB_PARAM_APPLICATIONRC);
		if (sHdl == null) return null;
		String s = new String(sHdl);
		return Integer.parseInt(s);	
	}

	public boolean getMore() {
		// 
		return false;
	}
	
	
	 
	@Override
	public String toString() {
		
		StringBuilder sb = new StringBuilder();
		sb.append(command);
		sb.append(marker);
		boolean first = true;
		for (String k : keySet()) {

            if (first) 
                first = false;
            else
            	sb.append('&');
        	sb.append(k);
        	sb.append("=");
        	sb.append(new String(get(k)));
        }
        return sb.toString();
	}
}
