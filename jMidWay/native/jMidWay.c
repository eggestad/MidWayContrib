
#include "jMidWay.h"
#include <MidWay.h>

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>

#define DEBUG(m, ...) mwlog(MWLOG_DEBUG, "%s(%d)" m, __FILE__, __LINE__, ##__VA_ARGS__)

static JavaVM * jvm = NULL;


static JNIEnv * get_JNIEnv(void) 
{
   JNIEnv * env;
   (*jvm)->GetEnv(jvm, (void **) &env, JNI_VERSION_1_2);
   return env;
}

static void throwMWException(JNIEnv * env, char * failedop) 
{
   jclass newExcCls;
   int e = errno;
   char * estr = malloc(strlen(failedop) + 256);
   strcpy(estr, failedop);	   
   strcat(estr, ", reason: ");
   strcat(estr, strerror(e));
   newExcCls = (*env)->FindClass(env, "org/midway/MidWayException");
   if (newExcCls == 0) { /* Unable to find the new exception class, give up. */
      return;
   }
   (*env)->ThrowNew(env, newExcCls, estr);
   free(estr);
   return;
};


////////////////////////////////////////////////////////////////////////
//
// Instance connectiong and info
//
////////////////////////////////////////////////////////////////////////

/*
 * Class:     MidWay
 * Method:    setCredPassword
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_org_midway_MidWay_setCredPassword
(
 JNIEnv *env,
 jclass mw,
 jstring username,
 jstring password
 ) 
{
   jint rc = -1;
   char * user = (char*) (*env)->GetStringUTFChars(env, username, 0);
   char * pass = (char*) (*env)->GetStringUTFChars(env, password, 0);

   if ((*env)->ExceptionCheck(env))
      goto out;

   DEBUG("user=%s, pass=%s", user, pass);
   rc = mwsetcred(MWAUTH_PASSWORD, user, pass);

 out:
   if (user != NULL)
      (*env)->ReleaseStringUTFChars(env, username, user);
   if (pass != NULL)
      (*env)->ReleaseStringUTFChars(env, password, pass);

   return rc;
};


/*
 * Class:     org_midway_MidWay
 * Method:    attach
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_org_midway_MidWay_attach
(
 JNIEnv *env,
 jclass cls,
 jstring jname,
 jstring jurl,
 jint flags
 )
{
   jint rc;
   DEBUG("start env=%p name=%p url=%p", env, jname, jurl);
   char * url = NULL;
   char * name = NULL;


   if (jurl != NULL)
      url = (char*) (*env)->GetStringUTFChars(env, jurl, NULL);

   if (jname != NULL) 
      name = (char*) (*env)->GetStringUTFChars(env, jname, NULL);
    
   if ((*env)->ExceptionCheck(env))
      goto out;

   DEBUG("ATTACH (%s, %s)", url, name);
   rc = mwattach(url, name, flags);

 out:
   if (name != NULL)
      (*env)->ReleaseStringUTFChars(env, jname, name);
   if (url != NULL)
      (*env)->ReleaseStringUTFChars(env, jurl, url);

   DEBUG("end rc=%d", rc);

   if ((*env)->ExceptionCheck(env))
      return rc;

   if (rc != 0) {
      throwMWException(env, "Unable to attach to MidWay instance");
   };
   return rc;
};

/*
 * Class:     org_midway_MidWay
 * Method:    detach
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_midway_MidWay_detach (JNIEnv * env, jclass cls) 
{
   jint rc;
   rc = mwdetach();
   DEBUG("DETACH");
   return rc;
};

/*
 * Class:     org_midway_MidWay
 * Method:    getURL
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_midway_MidWay_getURL(JNIEnv * env, jclass cls) 
{
   char * url = mwgeturl();
   if (url == NULL) return NULL;
   jstring jurl = (*env)->NewStringUTF(env, url);
   return jurl;
}


////////////////////////////////////////////////////////////////////////
//
// LOGGING
//
////////////////////////////////////////////////////////////////////////

/*
 * Class:     org_midway_MidWay
 * Method:    mwlog
 * Signature: (ILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_midway_MidWay_log
(
 JNIEnv * env,
 jclass cls,
 jint level,
 jstring jmsg
 ) 
{
   char * msg = NULL;
   if (jmsg != NULL)
      msg = (char*) (*env)->GetStringUTFChars(env, jmsg, NULL);

   mwlog(level, msg);

   if (msg != NULL)
      (*env)->ReleaseStringUTFChars(env, jmsg, msg);

}
    

/*
 * Class:     org_midway_MidWay
 * Method:    mwloglevel
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_org_midway_MidWay_setloglevel
(
 JNIEnv * env,
 jclass cls,
 jint level
 )
{
   return mwsetloglevel  (level);
};

/*
 * Class:     org_midway_MidWay
 * Method:    mwopenlog
 * Signature: (Ljava/lang/String;Ljava/lang/String;I)V
 */
JNIEXPORT void JNICALL Java_org_midway_MidWay_openlog
(
 JNIEnv * env,
 jclass cls,
 jstring jprog,
 jstring jprefix,
 jint level
 ) 
{
   char * prog = NULL;
   char * prefix = NULL;
   if (jprog != NULL)
      prog = (char*) (*env)->GetStringUTFChars(env, jprog, NULL);
   if (jprefix != NULL)
      prefix = (char*) (*env)->GetStringUTFChars(env, jprefix, NULL);
  
   mwopenlog(prog, prefix, level);

   if (prog != NULL)
      (*env)->ReleaseStringUTFChars(env, jprog, prog);
   if (prefix != NULL)
      (*env)->ReleaseStringUTFChars(env, jprefix, prefix);
}

////////////////////////////////////////////////////////////////////////
//
// Service CALLS
//
////////////////////////////////////////////////////////////////////////

/*
 * Class:     org_midway_MidWay
 * Method:    listServices
 * Signature: (Ljava/lang/String;)[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_org_midway_MidWay_listServices 
(
 JNIEnv * env,
 jclass cls,
 jstring match
 ) 
{
   char * glob = NULL; 
   char ** list;
   jobjectArray svcList;
   jclass stringCls;
   int rc, i;

   if (match != NULL)
      glob = (char*) (*env)->GetStringUTFChars(env, match, NULL);

   rc = mwlistsvc(glob, &list, 0);

   if (rc < 0) {
      throwMWException(env, "Failed to get list of services");
      return NULL;
   };

   if (match != NULL)
      (*env)->ReleaseStringUTFChars(env, match, glob);
    
   stringCls = (*env)->FindClass(env, "java/lang/String");

   svcList = (*env)->NewObjectArray(env, rc, stringCls, NULL);

   for (i = 0; i < rc; i++) {
      jstring svc = (*env)->NewStringUTF(env, list[i]);
      (*env)->SetObjectArrayElement(env, svcList, i, svc);
   };

   return svcList;
}

/**
   Perform a asynchronous call
   @param service  the name of the service to call
   @param data a byte array to send (this is the most effient form)
   @param flags is oone of NOREPLY, NOBLOCK
   @return a handle to be used with fetch() to get replies
   @throws TODO
*/
/*
 * Class:     org_midway_MidWay
 * Method:    acall
 * Signature: (Ljava/lang/String;[BI)I
 */
JNIEXPORT jint JNICALL Java_org_midway_MidWay_acall
(
 JNIEnv * env,
 jclass cls,
 jstring service,
 jbyteArray data,
 jint flags
 )
{
   jint hdl;
   char * svc;
   char * cdata = NULL;
   jsize datalen = 0;
   jint rc = 0;

   if (service == NULL) {
      errno = ENOENT;
      throwMWException(env, "Service call failed (no service name given");
      return -1;
   }
   
   svc = (char*) (*env)->GetStringUTFChars(env, service, NULL);

   if (data != NULL) {
      cdata = (char *) (*env)->GetByteArrayElements(env, data, 0);
      datalen = (*env)->GetArrayLength(env, data);
   };
   
   if ((*env)->ExceptionCheck(env)) goto out;

   DEBUG("Doing call svc=%s data=%*.*s(%d) flags=%d", svc, datalen, datalen, cdata, datalen, flags);
   rc = mwacall(svc, cdata, datalen, flags);

 out:
   (*env)->ReleaseStringUTFChars(env, service, svc);
   if (cdata != NULL) {
      (*env)->ReleaseByteArrayElements(env, data, (jbyte*) cdata, 0);
   }
   

   if (rc < 0) {
      errno = -rc;
      throwMWException(env, "Service call failed");
      return -1;
   };

   DEBUG("Good return acall %d", rc);
   return rc;
};

/**
   get a reply to an async call
*/
/*
 * Class:     org_midway_MidWay
 * Method:    fetch
 * Signature: (ILorg/midway/CallReply;I)Z
 */
JNIEXPORT jboolean JNICALL Java_org_midway_MidWay_fetch 
(
 JNIEnv * env,
 jclass cls,
 jint jHandle,
 jobject rplObj,
 jint flags
 )
{
   jint rc = 0;
   jint rlen = 0;
   char * rdata = NULL;
   jint appreturncode = 0;
   int handle = jHandle;

   if (rplObj == NULL) throwMWException(env, "trying to get reply data with no CallReply recipient");

   DEBUG("doing fetch with handle %x", handle);
   rc =  mwfetch(&handle, &rdata, &rlen, &appreturncode, flags);
   
   if (rc < 0) {
      DEBUG("mwfetch returned %d", rc);
      DEBUG("data %*.*s", rlen, rlen, rdata);
      errno = -rc;
      if (errno == EFAULT)  return JNI_FALSE;
      throwMWException(env, "Fetch call reply failed");

   };

   if (rc == 0) {
      return JNI_FALSE;
   };
      
   //   replyCls = (*env)->FindClass(env, "org/midway/CallReply");
   
   jfieldID fid;
   jclass rplCls = (*env)->GetObjectClass(env, rplObj);
   
   
   fid = (*env)->GetFieldID(env, rplCls, "handle", "I");
   if (fid != 0) {
      DEBUG("setting rc handle %x", handle);
      (*env)->SetIntField(env, rplObj, fid, handle);
   };
   
   fid = (*env)->GetFieldID(env, rplCls, "returncode", "I");
   if (fid != 0) {
      DEBUG("setting rc %d", rc);
      (*env)->SetIntField(env, rplObj, fid, rc);
   };
   
   fid = (*env)->GetFieldID(env, rplCls, "appreturncode", "I");
   if (fid != 0) {
      DEBUG("setting app rc %d", appreturncode);
      (*env)->SetIntField(env, rplObj, fid, appreturncode);
   };


   fid = (*env)->GetFieldID(env, rplCls, "data", "[B");
   if (fid != 0) {
      
      if (rlen == 0) {
	 (*env)->SetObjectField(env, rplObj, fid, NULL);
      } else {
	 jbyteArray dataArr = (*env)->NewByteArray(env, rlen);
	 DEBUG("setting data %*.*s len=%d", rlen, rlen, rdata, rlen);
	 (*env)->SetByteArrayRegion(env, dataArr, 0, rlen, (jbyte*) rdata);
	 (*env)->SetObjectField(env, rplObj, fid, dataArr);
      };
   };
   
   return JNI_TRUE;
};
   

////////////////////////////////////////////////////////////////////////
//
// Service provider 
//
////////////////////////////////////////////////////////////////////////

struct provision {
   SERVICEID serviceid;
   jobject provider;

   jfieldID serviceIdFID;
   jfieldID deadlineFID;
   jfieldID authenticationFID;
   jfieldID usernameFID;
   jfieldID clientnameFID;
   jmethodID serviceHandlerMethod;

   struct provision * next;
};

static struct provision * provisionings = NULL;

static struct provision * allocProvision(void) 
{
   struct provision * provInfo;
   int n = sizeof(struct provision);
   provInfo = malloc(n);
   memset(provInfo, 0, n);
   provInfo->next = provisionings;
   provisionings = provInfo;
};

// this should be thread local
static int replypending = 0;

static struct provision * releaseProvision(SERVICEID serviceid) 
{
   struct provision ** pprovInfo;

   pprovInfo = &provisionings;
   while (*pprovInfo != NULL) {
      
      if ((*pprovInfo)->serviceid == serviceid) {
	 *pprovInfo = (*pprovInfo)->next;
	 return *pprovInfo;
      };
      pprovInfo =  & ((*pprovInfo)->next);
   };
   return NULL;
};

static struct provision * getProvision(SERVICEID serviceid) 
{
   struct provision * provInfo;

   provInfo = provisionings;
   while (provInfo != NULL) {
      
      if (provInfo->serviceid == serviceid) {
	 return provInfo;
      };
      provInfo =  provInfo->next;
   };
   return NULL;
};

/*
 * Class:     org_midway_MidWay
 * Method:    provide
 * Signature: (Ljava/lang/String;Lorg/midway/ServiceProvider;)I
 */
JNIEXPORT jint JNICALL Java_org_midway_MidWay_provide 
(
 JNIEnv *env ,
 jclass cls,
 jstring serviceName,
 jobject provider
 )
{
   SERVICEID  svcid = UNASSIGNED; 
   char * name = NULL;
   int rc;
   struct provision * prov; 

   DEBUG("start");

   rc = _mw_isattached();
   if (rc != 1) {
      DEBUG("provide aborts we're not attached to an instance");
      throwMWException(env, "Unable to attach to MidWay instance");
      return -1;
   };
   rc = _mwsystemstate();
   if (rc != 0) {
      throwMWException(env, "MidWay instance is shuting down");
      DEBUG("provide aborts because instance is in shutdown state");
      return -1;
   };

   if (serviceName == NULL) {
      errno = EINVAL;
      throwMWException(env, "forward failed (no fowarding service name given");
      return -1;
   }

   name = (char*) (*env)->GetStringUTFChars(env, serviceName, NULL);

   if ( (*env)->ExceptionCheck(env))  goto errout;
   

      
   prov = allocProvision();
   prov->provider = provider;
      
   static jclass serviceProviderCls = NULL;      
   serviceProviderCls = (*env)->GetObjectClass(env, provider);

   prov->serviceIdFID = (*env)->GetFieldID(env, serviceProviderCls, "service", "Ljava/lang/String;");
   DEBUG("field id ptr=%p", prov->serviceIdFID);
   if (prov->serviceIdFID == NULL) goto errout;

   prov->deadlineFID = (*env)->GetFieldID(env, serviceProviderCls, "mdeadline", "J");
   DEBUG("field id ptr=%p", prov->deadlineFID);
   if (prov->deadlineFID == NULL) goto errout;

   prov->authenticationFID = (*env)->GetFieldID(env, serviceProviderCls, "authentication", "I");
   DEBUG("field id ptr=%p", prov->authenticationFID);
   if (prov->authenticationFID == NULL) goto errout;

   prov->usernameFID = (*env)->GetFieldID(env, serviceProviderCls, "username", "Ljava/lang/String;");
   DEBUG("field id ptr=%p", prov->usernameFID);
   if (prov->usernameFID == NULL) goto errout;

   prov->clientnameFID = (*env)->GetFieldID(env, serviceProviderCls, "clientname", "Ljava/lang/String;");
   DEBUG("field id ptr=%p", prov->clientnameFID);
   if (prov->clientnameFID == NULL) goto errout;

   prov->serviceHandlerMethod =  (*env)->GetMethodID(env, serviceProviderCls , "_serviceHandler", "([B)I");
   DEBUG("serviceHandler is ptr=%p", prov->serviceHandlerMethod);
   if (prov->serviceHandlerMethod == NULL) goto errout;

   if ( (*env)->ExceptionCheck(env))  goto errout;
   DEBUG("provide=%s ", name);

   svcid = _mw_ipc_provide(name, 0);      

   if (svcid == UNASSIGNED) {
      throwMWException(env, "Unable to provide service to MidWay instance");
      goto errout;
   } else {
      _mw_incprovided();

      prov->serviceid = svcid;
      prov->provider = (*env)->NewGlobalRef(env, provider);

      DEBUG("new glob ref %p old glob ref %p", prov->provider, provider);
      (*env)->DeleteLocalRef(env, provider);
   }
   
   if (name)   
      (*env)->ReleaseStringUTFChars(env, serviceName, name);
   DEBUG("svcid=%x", svcid);
   return svcid;

 errout:
   if (name)   
      (*env)->ReleaseStringUTFChars(env, serviceName, name);
   
   if (prov) {
      releaseProvision(prov->serviceid);
      free(prov);
   }
   
}

/*
 * Class:     org_midway_MidWay
 * Method:    unprovide
 * Signature: (Ljava/lang/String;J)I
 */
JNIEXPORT jint JNICALL Java_org_midway_MidWay_unprovide 
(
 JNIEnv *env, 
 jclass cls, 
 jstring serviceName, 
 jlong serviceID
 ) 
{
   SERVICEID  svcid = UNASSIGNED; 
   char * name = NULL;
   struct provision * prov; 
   jint rc = -1;

   DEBUG("start");

   if (serviceName == NULL) {
      errno = EINVAL;
      throwMWException(env, "forward failed (no fowarding service name given");
      return -1;
   }
   name = (char*) (*env)->GetStringUTFChars(env, serviceName, NULL);
   prov = releaseProvision(serviceID);
   free(prov);
   
   DEBUG("unprovide=%s %x ", name, serviceID);
   rc = _mw_ipcsend_unprovide(name, serviceID);

   (*env)->ReleaseStringUTFChars(env, serviceName, name);
   return rc;
}

/*
 * Class:     org_midway_MidWay
 * Method:    doServiceRequest
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_org_midway_MidWay_doServiceRequest
(
 JNIEnv * env, 
 jclass cls, 
 jint flags
 )
{
   mwsvcinfo * svcinfo;
   
   DEBUG("flags=%x", flags);
   svcinfo = _mwGetServiceRequest(flags);

   if (svcinfo == NULL) {
      throwMWException(env, "no service request available");
      return -errno;
   };

   //
   // Getting ServiceProvider class and fields
   //
   // first of all get the the class and field ID and cache them 
   // the referance and id is valid as long the VM has loaded them 
   // and it's a clear recommendation in Java Native Iterface to cache them for performance
   //


   struct provision * prov;
   
   DEBUG("Serviceid = %x", svcinfo->svcid);
   prov = getProvision(svcinfo->svcid);
   
   if (prov == NULL) {
      throwMWException(env, "no service provided with currect ID");
      return -errno;
   };

   //
   // setting the fields
   //

   // service
   jstring jsvcname = (*env)->NewStringUTF(env, svcinfo->service);
   jstring jsvcnameGlob = (*env)->NewGlobalRef(env, jsvcname);
   DEBUG("new glob ref %p old glob ref %p", jsvcnameGlob, jsvcname);
   (*env)->DeleteLocalRef(env, jsvcname);
   (*env)->SetObjectField(env, prov->provider, prov->serviceIdFID, jsvcnameGlob);

   // mdeadline
   long deadline = svcinfo->deadline;
   deadline *= 1000;
   deadline += svcinfo->udeadline/1000;
   (*env)->SetLongField(env, prov->provider, prov->deadlineFID, deadline);

   // authentication
   (*env)->SetIntField(env, prov->provider, prov->authenticationFID, svcinfo->authentication);


   // username
   jstring jusername = (*env)->NewStringUTF(env, svcinfo->username);
   jstring jusernameGlob = (*env)->NewGlobalRef(env, jusername);
   DEBUG("new glob ref %p old glob ref %p", jusernameGlob, jusername);
   (*env)->DeleteLocalRef(env, jusername);
   (*env)->SetObjectField(env, prov->provider, prov->usernameFID, jusernameGlob);

   // clientname
   jstring jclientname = (*env)->NewStringUTF(env, svcinfo->clientname);

   jstring jclientnameGlob = (*env)->NewGlobalRef(env, jclientname);
   DEBUG("new glob ref %p old glob ref %p", jclientnameGlob, jclientname);
   (*env)->DeleteLocalRef(env, jclientname);
   (*env)->SetObjectField(env, prov->provider, prov->clientnameFID, jclientnameGlob);

   jbyteArray svcData = NULL;
   if (svcinfo->datalen > 0) {
      svcData = (*env)->NewByteArray(env, svcinfo->datalen);
      (*env)->SetByteArrayRegion(env, svcData, 0, svcinfo->datalen, (jbyte*) svcinfo->data);
   };
   
   //
   // now calling the handler
   //
   DEBUG("calling service handler");
   jint rc;
   replypending = 1;
   rc = (*env)->CallIntMethod(env, prov->provider, prov->serviceHandlerMethod, svcData);
   DEBUG("done rc = %d reply pending=%d", rc, replypending);
   
   if (replypending) 
      rc = mwreply(NULL, 0,  rc?MWFAIL:MWSUCCESS,  0, 0);
   if (svcinfo->data) mwfree(svcinfo->data);
   free(svcinfo);
   return;
}
/*
 * Class:     org_midway_MidWay
 * Method:    forward
 * Signature: (Ljava/lang/String;[B)I
 */
JNIEXPORT jint JNICALL Java_org_midway_MidWay_forward 
(
 JNIEnv * env,
 jclass cls,
 jstring service,
 jbyteArray data
 )
{
   char * svcname = NULL;
   char * cdata = NULL;
   jsize datalen = 0;
   jint rc;

   DEBUG("start");
   if (service == NULL) {
      errno = EINVAL;
      throwMWException(env, "forward failed (no fowarding service name given");
      return -1;
   }
   
   svcname = (char*) (*env)->GetStringUTFChars(env, service, NULL);
   if (svcname == NULL) return;

   if (data != NULL) {
      cdata = (char *) (*env)->GetByteArrayElements(env, data, 0);
      datalen = (*env)->GetArrayLength(env, data);
   };

   if (! (*env)->ExceptionCheck(env)) {
      DEBUG("forward to=%s (%d) ", svcname, datalen);
      rc = mwforward(svcname, cdata, datalen, 0);
      replypending = 0;
   }

   (*env)->ReleaseStringUTFChars(env, service, svcname);

   if (data != NULL) {
      (*env)->ReleaseByteArrayElements(env, data, (jbyte*) cdata, 0);
   }

   return rc;
}   

/*
 * Class:     org_midway_MidWay
 * Method:    reply
 * Signature: ([BII)I
 */
JNIEXPORT jint JNICALL Java_org_midway_MidWay_reply 
(
 JNIEnv * env,
 jclass cls,
 jbyteArray data,
 jint returnCode,
 jint appRC
 ) 
{
   char * svcname = NULL;
   char * cdata = NULL;
   jsize datalen = 0;
   jint rc;
   
   if (data != NULL) {
      cdata = (char *) (*env)->GetByteArrayElements(env, data, 0);
      datalen = (*env)->GetArrayLength(env, data);
   };

   if (! (*env)->ExceptionCheck(env)) {
      DEBUG("reply (%d) rc=%d apprc=%d", datalen, returnCode,  appRC);
      rc = mwreply(cdata, datalen,  returnCode,  appRC, 0);
      if (returnCode != MWMORE) replypending = 0;
   }

   if (data != NULL) {
      (*env)->ReleaseByteArrayElements(env, data, (jbyte*) cdata, 0);
   }

   return rc;
}  

////////////////////////////////////////////////////////////////////////
//
// EVENTS
//
////////////////////////////////////////////////////////////////////////

/*
 * Class:     org_midway_MidWay
 * Method:    event
 * Signature: (Ljava/lang/String;[BLjava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_org_midway_MidWay_event
(
 JNIEnv * env,
 jclass cls,
 jstring eventname,
 jbyteArray data,
 jstring username,
 jstring clientname
 )
{
   jint hdl;
   char * ev;
   char * cdata = NULL;
   char * uname = NULL;
   char * cname = NULL;
   jsize datalen = 0;
   jint rc = 0;

   if (eventname == NULL) {
      errno = EINVAL;
      throwMWException(env, "Eventname call failed (no eventname name given");
      return -1;
   }
   
   ev = (char*) (*env)->GetStringUTFChars(env, eventname, NULL);

   if (data != NULL) {
      cdata = (char *) (*env)->GetByteArrayElements(env, data, 0);
      datalen = (*env)->GetArrayLength(env, data);
   };

   if (username != NULL)
      uname = (char*) (*env)->GetStringUTFChars(env, username, NULL);
   if (clientname != NULL)
      cname = (char*) (*env)->GetStringUTFChars(env, clientname, NULL);

   if (! (*env)->ExceptionCheck(env)) {
      DEBUG("Doing event name=%s data=%*.*s(%d) username=%s clientname=%s", ev, datalen, datalen, cdata, datalen, uname, cname);
      rc = mwevent(ev, cdata, datalen, uname, cname);
   };

   (*env)->ReleaseStringUTFChars(env, eventname, ev);

   if (data != NULL) {
      (*env)->ReleaseByteArrayElements(env, data, (jbyte*) cdata, 0);
   }
   
   if (uname != NULL)
      (*env)->ReleaseStringUTFChars(env, username, uname);
   if (cname != NULL)
      (*env)->ReleaseStringUTFChars(env, clientname, cname);

   if (rc < 0) {
      errno = -rc;
      throwMWException(env, "Eventname call failed");
      return -1;
   };

   DEBUG("Good return event %d", rc);
   return rc;
};

struct eventInfo {
   int subid;
   char * name;
   jobject eventListener;
   struct eventInfo * next;
};
static int nsubs = 0, lastsubid = 42;

struct eventInfo * subscriptions = NULL;

static struct eventInfo * allocEventInfo(void) 
{
   struct eventInfo * evInfo;
   
   evInfo = malloc(sizeof(struct eventInfo));

   evInfo->subid = lastsubid++;
   DEBUG("assigned free subent subid %d", evInfo->subid);
   evInfo->next = subscriptions;
   subscriptions = evInfo;
};

static struct eventInfo * releaseEventInfo(int subid) 
{
   struct eventInfo ** pevInfo;

   pevInfo = &subscriptions;
   while (*pevInfo != NULL) {
      
      if ((*pevInfo)->subid == subid) {
	 *pevInfo = (*pevInfo)->next;
	 free( (*pevInfo)->name );
	 return *pevInfo;
      };
      pevInfo =  & ((*pevInfo)->next);
   };
   return NULL;
};

/*
 * Class:     org_midway_MidWay
 * Method:    subscribe
 * Signature: (Ljava/lang/String;Lorg/midway/IEventListener;)I
 */
JNIEXPORT jint JNICALL Java_org_midway_MidWay_subscribe
(
 JNIEnv * env,
 jclass cls,
 jstring eventname,
 jobject evHdl,
 jint flags
 )
{
   struct eventInfo * evInfo;
   char * name;
   int rc;

   DEBUG("subscribe to event");
   if (eventname == NULL) {
      errno = EINVAL;
      throwMWException(env, "Eventname call failed (no eventname name given");
      return -1;
   }
   
   if (evHdl == NULL) {
      errno = EINVAL;
      throwMWException(env, "Eventname call failed (no event handler given");
      return -1;
   }
   
   evInfo = allocEventInfo();
   name = (char*) (*env)->GetStringUTFChars(env, eventname, NULL);
   
   DEBUG("subscribe to event %s", name);
   evInfo->name = strdup(name);
   evInfo->eventListener = (*env)->NewGlobalRef(env, evHdl);
   DEBUG("new glob ref %p old glob ref %p", evInfo->eventListener, evHdl);
   (*env)->DeleteLocalRef(env, evHdl);
   
   rc = _mwsubscribe(evInfo->name, evInfo->subid, flags);
   if (rc < 0) {
      errno = -rc;
      throwMWException(env, "Event subscription failed");
      return -1;
   };
   return evInfo->subid;
};

//
// This function have a prototype that interfaces the MidWay library. 
// it will be called on reception of every subscribed event.
//
static void _mw_doevent_java(int subid, char * event, char * data, int datalen) 
{
   jclass eventHandlerClass;
   jmethodID doEventMethod;
   JNIEnv * env = get_JNIEnv();
   jobject eventInst;
   jobject jobj;
   struct eventInfo * evInfo = subscriptions;

   DEBUG("got an event id=%d name=%s", subid, event);

   while(evInfo != NULL) {
      if (evInfo->subid == subid)  break;
      evInfo = evInfo->next;
   };
   
   if (evInfo == NULL) {
      mwlog(MWLOG_ERROR, "received unexcpected event %s id %d", event, subid);
      return;
   }

   //
   // Get the method in the event handler
   //
   eventHandlerClass = (*env)->GetObjectClass(env, evInfo->eventListener);
   DEBUG("event handler cls ptr=%p", eventHandlerClass);
   if (eventHandlerClass == NULL) return;

   doEventMethod = (*env)->GetMethodID(env, eventHandlerClass, "doEvent", "(Lorg/midway/Event;)V");
   DEBUG("event handler method ptr=%p", doEventMethod);
   if (doEventMethod == NULL) return;

   //
   // Getting Event class and fields/constructor
   //
   // first of all get the the class and field ID and cache them 
   // the referance and id is valid as long the VM has loaded them 
   // and it's a clear recommendation in Java Native Iterface to cache them for performance
   //
   static jclass eventCls = NULL;      
   if (eventCls == NULL) {
      eventCls = (*env)->FindClass(env, "org/midway/Event");
      DEBUG("cls ptr=%p", eventCls);
      if (eventCls == NULL) return;
   };

   static jfieldID subIdFID = NULL;
   static jfieldID eventNameFID = NULL;
   static jfieldID eventDataFID = NULL;
   static jmethodID constructorID = NULL;

   if (subIdFID == NULL) {
      subIdFID = (*env)->GetFieldID(env, eventCls, "subscriptionID", "I");
      DEBUG("field id ptr=%p", subIdFID);
      if (subIdFID == NULL) return;
   }

   if (eventNameFID == NULL) {
      eventNameFID = (*env)->GetFieldID(env, eventCls, "eventName", "Ljava/lang/String;");
      DEBUG("field id ptr=%p", eventNameFID);
      if (eventNameFID == NULL) return;
   }
   
   if (eventDataFID == NULL) {
      eventDataFID = (*env)->GetFieldID(env, eventCls, "eventData", "[B");
      DEBUG("field id ptr=%p", eventDataFID);
      if (eventDataFID == NULL) return;
   }

   if (constructorID == NULL) {
      constructorID = (*env)->GetMethodID(env, eventCls, "<init>", "()V");
      DEBUG("constructor ptr=%p", constructorID);
      if (constructorID == NULL) return;
   }

   //
   // make new Event instance 
   //
   eventInst = (*env)->NewObject(env, eventCls, constructorID);
   DEBUG("event object ptr=%p", eventInst);
   if (eventInst == NULL) return;

   //
   // setting the fields
   //
   (*env)->SetIntField(env, eventInst, subIdFID, subid);
   jobj = (*env)->NewStringUTF(env, event);

   jstring jname = (*env)->NewGlobalRef(env, jobj);
   DEBUG("new glob ref %p old glob ref %p", jname, jobj);
   (*env)->DeleteLocalRef(env, jobj);

   (*env)->SetObjectField(env, eventInst, eventNameFID,  jname);

   if (data != NULL && datalen > 0) {
      jbyteArray dataArr, eventData;
      dataArr = (*env)->NewByteArray(env, datalen);

      eventData = (*env)->NewGlobalRef(env, dataArr);
      DEBUG("new glob ref %p old glob ref %p", eventData, dataArr);
      (*env)->DeleteLocalRef(env, dataArr);

      (*env)->SetByteArrayRegion(env, eventData, 0, datalen, (jbyte*) data);
      (*env)->SetObjectField(env, eventInst, eventDataFID,  eventData);
   };

   //
   // now calling the handler
   //
   DEBUG("calling event handler");
   (*env)->CallVoidMethod(env, evInfo->eventListener, doEventMethod, eventInst);

   DEBUG("done");
   return;
}

/*
 * Class:     org_midway_MidWay
 * Method:    recvevents
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_midway_MidWay_recvevents (JNIEnv * env, jclass cls)
{
   DEBUG("calling recvevents");
   mwrecvevents();
   DEBUG("done");
}


JNIEXPORT jint JNICALL JNI_OnLoad (JavaVM * _jvm, void * reserved)
{
   DEBUG("Library load function jvm=%p", _jvm);

   jvm = _jvm;

   _mw_register_event_handler(_mw_doevent_java);

   return JNI_VERSION_1_2;
};


/* Emacs C indention
   Local variables:
   c-basic-offset: 3
   End:
*/
