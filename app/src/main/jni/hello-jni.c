#include <string.h>
#include <jni.h>
#include <android/log.h>

#define LOG_TAG "hello-jni.c"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

static JavaVM *jvm = NULL;
//static jobject mObject = NULL;
static jclass mClass = NULL;
static jmethodID mMethodID = NULL;

jstring
Java_com_example_hellojni_HelloJni_stringFromJNI( JNIEnv* env,
                                                  jobject thiz )
{
    LOGI("stringFromJNI");
#if defined(__arm__)
  #if defined(__ARM_ARCH_7A__)
    #if defined(__ARM_NEON__)
      #if defined(__ARM_PCS_VFP)
        #define ABI "armeabi-v7a/NEON (hard-float)"
      #else
        #define ABI "armeabi-v7a/NEON"
      #endif
    #else
      #if defined(__ARM_PCS_VFP)
        #define ABI "armeabi-v7a (hard-float)"
      #else
        #define ABI "armeabi-v7a"
      #endif
    #endif
  #else
   #define ABI "armeabi"
  #endif
#elif defined(__i386__)
   #define ABI "x86"
#elif defined(__x86_64__)
   #define ABI "x86_64"
#elif defined(__mips64)  /* mips64el-* toolchain defines __mips__ too */
   #define ABI "mips64"
#elif defined(__mips__)
   #define ABI "mips"
#elif defined(__aarch64__)
   #define ABI "arm64-v8a"
#else
   #define ABI "unknown"
#endif

    char msg[] = "Hello from JNI !  Compiled with ABI " ABI ".";
    jstring jMsg = (*env)->NewStringUTF(env, msg);

    (*env)->CallVoidMethod(env,thiz,mMethodID,jMsg);
    return jMsg;
}
jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    LOGI("JNI_OnLoad");
    JNIEnv* env = NULL;
    jint result = -1;
    if((*vm)->GetEnv(vm, (void**)&env, JNI_VERSION_1_4) != JNI_OK){
        return result;
    }

    jclass clazz;
    clazz = (*env)->FindClass(env,"com/example/hellojni/HelloJni");
    if( !clazz ){
        LOGE("get object class fail");
        return result;
    }
    mClass = (*env)->NewGlobalRef(env,clazz);
    if(!mClass){
       LOGE("get global object class fail");
       return result;
    }
    mMethodID = (*env)->GetMethodID(env, mClass, "callFromJni", "(Ljava/lang/String;)V" );
    if(mMethodID == NULL ){
        LOGE("get global MethodID fail");
        return result;
    }

    jvm = vm;
    LOGI("JNI_VERSION_1_4");
    return JNI_VERSION_1_4;
}

//char *pt;
//pt = (*env)->GetStringUTFChars(env,msg,NULL);
//...
//(*env)->ReleaseStringUTFChars(env,msg,pt);