package org.midway;

/**

@author Terje Eggestad
 */
public class Event {
    public int subscriptionID;
    public String eventName;
    public byte[] eventData;

    public String toString() {
        
        return String.format("Event: %s id=%d data=%s", eventName, subscriptionID, new String(eventData));
    }
};
