#include <string.h>
#include <jni.h>
#include <android/log.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LOG_TAG "hello-jni.cpp"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

static JavaVM *jvm = NULL;
static jclass mClass = NULL;
static jmethodID mMethodID = NULL;
static jobject mObject = NULL;

JNIEnv* getJNIEnv() {
    JNIEnv* env = NULL;
    int res = jvm->GetEnv((void**)&env, JNI_VERSION_1_4);

    if (res == JNI_EDETACHED) {
        //LOGI("GetEnv: not attached");
        if (jvm->AttachCurrentThread(&env, NULL) != 0) {
            LOGI("Failed to attach");
            return NULL;
        }
    } else if (res == JNI_OK) {
        //
    } else if (res == JNI_EVERSION) {
        LOGI("GetEnv: version not supported");
        return NULL;
    }
    return env;
}

jstring Java_com_example_hellojni_HelloJni_stringFromJNI(JNIEnv* env, jobject thiz)
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
    jstring jMsg = env->NewStringUTF(msg);

    env->CallVoidMethod(thiz, mMethodID, jMsg);
    return jMsg;
}

jintArray Java_com_example_hellojni_HelloJni_intArrayFromJni(JNIEnv* env, jobject thiz) {
    int ints[1] = {0};
    jintArray jints = env->NewIntArray(1);
    env->SetIntArrayRegion(jints, 0, 1, (jint*)ints);

    jclass mClass = env->GetObjectClass(thiz);
    if( !mClass ){
        LOGE("get object class fail");
        return jints;
    }
    jmethodID mid = env->GetMethodID(mClass, "intArrayCallFromJni", "([I)Z");
    if(mid == NULL ){
        LOGE("get intArrayCallFromJni MethodID fail");
        return jints;
    }

    jboolean b = env->CallBooleanMethod(thiz, mid, jints);

    return jints;
}

jobject Java_com_example_hellojni_HelloJni_fooFromJni(JNIEnv* env, jobject thiz) {

    //JNIEnv* env = getJNIEnv();
    jclass fooClass = env->FindClass("com/example/hellojni/HelloJni$Foo");
    if( !fooClass ){
        LOGE("get foo class fail");
        return NULL;
    }
    jmethodID fooConstructorMethodID = env->GetMethodID(fooClass, "<init>", "()V");

    jclass mClass = env->GetObjectClass(thiz);
    if( !mClass ){
        LOGE("get object class fail");
        return NULL;
    }

    jmethodID mid = env->GetMethodID(mClass, "fooCallFromJni", "(Lcom/example/hellojni/HelloJni$Foo;)Lcom/example/hellojni/HelloJni$Foo;");
    if(mid == NULL ){
        LOGE("get fooCallFromJni MethodID fail");
        return NULL;
    }

    jobject jfoo = env->NewObject(fooClass, fooConstructorMethodID);
    jobject mObject = env->CallObjectMethod(thiz, mid, jfoo);

    jclass mObjectCalss = env->GetObjectClass(mObject);
    if( !mClass ){
        LOGE("get mObject class fail");
        return NULL;
    }

    jfieldID fooNameFieldId = env->GetFieldID(mObjectCalss, "name", "Ljava/lang/String;");
    if(fooNameFieldId == NULL ){
        LOGE("get Foo.name FieldID fail");
        return NULL;
    }

    jfieldID fooNumFieldId = env->GetFieldID(mObjectCalss, "num", "I");
    if(fooNumFieldId == NULL ){
        LOGE("get Foo.num FieldID fail");
        return NULL;
    }
    jstring mObjectName = (jstring)env->GetObjectField(mObject, fooNameFieldId);
    const char* name = env->GetStringUTFChars(mObjectName, 0);
    jint num = env->GetIntField(mObject, fooNumFieldId);
    LOGI("mObject Foo name: %s num: %d", name, num);

    jstring jname = env->NewStringUTF("Kentpon");
    env->SetObjectField(mObject, fooNameFieldId, jname);
    env->SetIntField(mObject, fooNumFieldId, 1);

    env->DeleteLocalRef(mObjectName);
    env->DeleteLocalRef(jname);
    env->DeleteLocalRef(jfoo);

    //jvm->DetachCurrentThread();
    return mObject;
}

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    LOGI("JNI_OnLoad");
    JNIEnv* env = NULL;
    jint result = -1;
    if(vm->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK){
        return result;
    }

    jclass clazz;
    clazz = env->FindClass("com/example/hellojni/HelloJni");
    if(!clazz){
        LOGE("get object class fail");
        return result;
    }
    mClass = static_cast<jclass>(env->NewGlobalRef(clazz));
    if(!mClass){
       LOGE("get global object class fail");
       return result;
    }
    mMethodID = env->GetMethodID(mClass, "callFromJni", "(Ljava/lang/String;)V");
    if(mMethodID == NULL ){
        LOGE("get global MethodID fail");
        return result;
    }

    jvm = vm;
    LOGI("JNI_VERSION_1_4");
    return JNI_VERSION_1_4;
}

#ifdef __cplusplus
}
#endif