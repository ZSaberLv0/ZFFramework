#include "ZFImpl_sys_Android_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFTimer.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFTimer ZFImpl_sys_Android_JNI_ID(ZFCore_1impl_ZFTimer)
#define ZFImpl_sys_Android_JNI_NAME_ZFTimer ZFImpl_sys_Android_JNI_NAME(ZFCore_impl.ZFTimer)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFTimerImpl_sys_Android, ZFTimer, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Android:Object:Timer")

public:
    zfoverride
    virtual void protocolOnInit(void)
    {
        zfsuper::protocolOnInit();
        JNIEnv *jniEnv = JNIGetJNIEnv();
        jobject tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFTimer).c_str());
        this->jclsOwner = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);
    }
    zfoverride
    virtual void protocolOnDealloc(void)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        JNIUtilDeleteGlobalRef(jniEnv, this->jclsOwner);
        zfsuper::protocolOnDealloc();
    }

    virtual void *nativeTimerCreate(ZF_IN ZFTimer *timer)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_nativeTimerCreate",
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
            ).c_str());
        jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv, this->jclsOwner, jmId);
        jobject ret = JNIUtilNewGlobalRef(jniEnv, tmp);
        JNIUtilDeleteLocalRef(jniEnv, tmp);
        return ret;
    }
    virtual void nativeTimerDestroy(ZF_IN ZFTimer *timer,
                                    ZF_IN void *nativeTimer)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        jobject nativeTimerTmp = ZFCastStatic(jobject, nativeTimer);
        JNIUtilDeleteGlobalRef(jniEnv, nativeTimerTmp);
    }

    virtual void timerStart(ZF_IN ZFTimer *timer)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_timerStart",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
                .add(JNIPointerJNIType)
                .add(JNIType::S_long)
                .add(JNIType::S_long)
                .add(JNIType::S_boolean)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            ZFCastStatic(jobject, timer->nativeTimer()),
            JNIConvertZFObjectToJNIType(jniEnv, timer),
            (jlong)timer->timerDelay(),
            (jlong)timer->timerInterval(),
            timer->timerActivateInMainThread());
    }
    virtual void timerStop(ZF_IN ZFTimer *timer)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_timerStop",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId, ZFCastStatic(jobject, timer->nativeTimer()));
    }

private:
    jclass jclsOwner;
ZFPROTOCOL_IMPLEMENTATION_END(ZFTimerImpl_sys_Android)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFTimerImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFTimer,
                         void, native_1notifyTimerStart,
                         JNIPointer zfjniPointerToken)
{
    ZFTimer *timer = ZFCastZFObject(ZFTimer *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerToken));
    ZFPROTOCOL_ACCESS(ZFTimer)->notifyTimerStart(timer);
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFTimer,
                         void, native_1notifyTimerActivate,
                         JNIPointer zfjniPointerToken)
{
    ZFTimer *timer = ZFCastZFObject(ZFTimer *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerToken));
    ZFPROTOCOL_ACCESS(ZFTimer)->notifyTimerActivate(timer);
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFTimer,
                         void, native_1notifyTimerStop,
                         JNIPointer zfjniPointerToken)
{
    ZFTimer *timer = ZFCastZFObject(ZFTimer *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerToken));
    ZFPROTOCOL_ACCESS(ZFTimer)->notifyTimerStop(timer);
}
JNI_METHOD_DECLARE_END()
#endif // #if ZF_ENV_sys_Android

