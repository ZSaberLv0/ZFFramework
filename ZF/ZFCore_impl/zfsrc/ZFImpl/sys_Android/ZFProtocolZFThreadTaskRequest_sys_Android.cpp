#include "ZFImpl_sys_Android_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFThreadTaskRequest.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFThreadTaskRequest ZFImpl_sys_Android_JNI_ID(ZFCore_1impl_ZFThreadTaskRequest)
#define ZFImpl_sys_Android_JNI_NAME_ZFThreadTaskRequest ZFImpl_sys_Android_JNI_NAME(ZFCore_impl.ZFThreadTaskRequest)

// ============================================================
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFThreadTaskRequestImpl_sys_Android, ZFThreadTaskRequest, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Android:Handler")

public:
    zfoverride
    virtual void protocolOnInit(void)
    {
        zfsuper::protocolOnInit();
        JNIEnv *jniEnv = JNIGetJNIEnv();
        jobject tmp = zfnull;

        tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFThreadTaskRequest).c_str());
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
public:
    virtual void taskRequest(ZF_IN const ZFListener &task,
                             ZF_IN ZFObject *param0,
                             ZF_IN ZFObject *param1)
    {
        ZFListenerHolder *listenerData = zfAlloc(ZFListenerHolder, task,
            ZFListenerData().param0(param0).param1(param1));
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_taskRequest",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIPointerJNIType)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv, this->jclsOwner, jmId,
            JNIConvertZFObjectToJNIType(jniEnv, listenerData));
    }
private:
    jclass jclsOwner;
ZFPROTOCOL_IMPLEMENTATION_END(ZFThreadTaskRequestImpl_sys_Android)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFThreadTaskRequestImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFThreadTaskRequest,
                         void, native_1doExecute,
                         JNIPointer zfjniPointerToken)
{
    ZFListenerHolder *listenerData = ZFCastZFObject(ZFListenerHolder *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerToken));
    listenerData->runnableExecute();
    zfRelease(listenerData);
}
JNI_METHOD_DECLARE_END()

#endif // #if ZF_ENV_sys_Android

