package org.midway.impl.jna_native;

import org.midway.MidWay;
import org.midway.MidWayEventListener;
import org.midway.MidWayServiceReplyListener;
import org.midway.impl.IMidWayImpl;
import org.midway.impl.java.MidWayPendingReply;
import org.midway.impl.java.SRBConnectionEndPoint;
import org.midway.impl.java.SRBMessage;
import org.midway.impl.java.Timber;

import java.io.IOException;
import java.net.*;
import java.text.ParseException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.regex.Pattern;


public class MidWayCImpl implements IMidWayImpl {


	public MidWayCImpl() {

	}
	
	/**
	 * Implementation of MidWay.attach
	 * @param uri
	 * @param name
	 * @param useThreads
	 * @throws Exception
	 */
	public final synchronized void attach(URI uri, String name, boolean useThreads) throws Exception{


	}

	/**
	 * Implements MaidWay.detach
	 * @throws Exception
	 */
	public final synchronized void detach() throws Exception{


	}


	HashMap<Long, MidWayPendingReply> pendingServiceCalls = new HashMap<Long, MidWayPendingReply>();

	/**
	 * Implements MidWay.call
	 * 
	 * @param servicename
	 * @param data
	 * @param listener
	 * @param flags
	 * @return
	 * @throws Exception
	 */
	public synchronized long acall(String servicename, byte[] data,
			MidWayServiceReplyListener listener, int flags) throws Exception {
		

	}
	/**
	 * Implementation of MidWay.fetch
	 * @param handle
	 * @return
	 * @throws Exception
	 */
	public boolean fetch(long handle) throws Exception {


	}
//	public MidWayReply fetchx(long handle) throws Exception {
//
//		MidWayPendingReply pending = pendingServiceCalls.get(handle);
//		MidWayReply rpl;
//
//		srbEndPoint.getNextSRBMessage();
//
//		synchronized(pending) {
//
//			while (!(pending.isReady())) {
//				if (useThreads) {                
//					pending.wait();
//				} else {
//					srbEndPoint.getNextSRBMessage();
//				}
//
//			}
//
//			if (!pending.more)
//				pendingServiceCalls.remove(handle);
//			rpl = pending.getReply();
//
//		}
//		return rpl;
//	}


	public final synchronized void subscribe(Pattern regex, MidWayEventListener listener) {


	}


	public final synchronized void unsubscribe(MidWayEventListener listener) {

	}


}
