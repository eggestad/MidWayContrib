package org.midway.impl;

import org.midway.impl.jna_native.MidWayCImpl;

import java.lang.reflect.InvocationTargetException;

public class MidWayImplHelper {

    public static MidWayImplHelper load_native_impl() throws MidWayExecption {
        try {
            Class<?> implClass = MidWayCImpl.class;
            MidWayCImpl cmw = (MidWayCImpl) implClass.getDeclaredConstructor().newInstance();
        } catch (InstantiationException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            e.printStackTrace();
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        }

    }
}
