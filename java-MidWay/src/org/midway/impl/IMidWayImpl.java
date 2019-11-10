package org.midway.impl;

import org.midway.MidWayEventListener;
import org.midway.MidWayServiceReplyListener;
import org.midway.impl.java.MidWayPendingReply;

import java.net.URI;
import java.util.HashMap;
import java.util.regex.Pattern;


public interface IMidWayImpl {


	/**
	 * Implementation of MidWay.attach
	 * @param uri
	 * @param name
	 * @param useThreads
	 * @throws Exception
	 */
	public void attach(URI uri, String name, boolean useThreads) throws Exception;

	/**
	 * Implements MidWay.detach
	 * @throws Exception
	 */
	public void detach() throws Exception;

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
	public long acall(String servicename, byte[] data,
			MidWayServiceReplyListener listener, int flags) throws Exception ;
	/**
	 * Implementation of MidWay.fetch
	 * @param handle
	 * @return
	 * @throws Exception
	 */
	public boolean fetch(long handle) throws Exception ;

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

	public void subscribe(Pattern regex, MidWayEventListener listener) ;

	public void unsubscribe(MidWayEventListener listener) ;

}
