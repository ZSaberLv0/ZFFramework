#include "ZFImpl_sys_Android_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFThread.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"

#if ZF_ENV_sys_Android

ZF_NAMESPACE_GLOBAL_BEGIN

#define ZFImpl_sys_Android_JNI_ID_ZFThread ZFImpl_sys_Android_JNI_ID(ZFCore_1impl_ZFThread)
#define ZFImpl_sys_Android_JNI_NAME_ZFThread ZFImpl_sys_Android_JNI_NAME(ZFCore_impl.ZFThread)

// ============================================================
// global type
zfclassNotPOD _ZFP_ZFThreadImpl_sys_Android_ExecuteData
{
public:
    ZFThread *ownerZFThread;
    ZFListener runnable;
    ZFObject *param0;
    ZFObject *param1;

public:
    _ZFP_ZFThreadImpl_sys_Android_ExecuteData(ZF_IN ZFThread *ownerZFThread,
                                              ZF_IN ZFListener runnable,
                                              ZF_IN ZFObject *param0,
                                              ZF_IN ZFObject *param1)
    : ownerZFThread(zfnull)
    , runnable(runnable)
    , param0(param0)
    , param1(param1)
    {
    }
};

// ============================================================
// global data
typedef jlong _ZFP_ZFThreadImpl_sys_Android_NativeThreadIdType;
typedef jint _ZFP_ZFThreadImpl_sys_Android_ExecuteDataIdType;
typedef zfstlmap<_ZFP_ZFThreadImpl_sys_Android_NativeThreadIdType, ZFThread *> _ZFP_ZFThreadImpl_sys_Android_ThreadMapType;
typedef zfstlmap<_ZFP_ZFThreadImpl_sys_Android_ExecuteDataIdType, _ZFP_ZFThreadImpl_sys_Android_ExecuteData *> _ZFP_ZFThreadImpl_sys_Android_ExecuteDataMapType;

static jclass _ZFP_ZFThreadImpl_sys_Android_jclsOwner = NULL;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFThreadImpl_sys_Android_jclsHolder, ZFLevelZFFrameworkStatic)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    jobject tmp = JNIUtilFindClass(jniEnv, JNIConvertClassNameForFindClass(ZFImpl_sys_Android_JNI_NAME_ZFThread).c_str());
    _ZFP_ZFThreadImpl_sys_Android_jclsOwner = (jclass)JNIUtilNewGlobalRef(jniEnv, tmp);
    JNIUtilDeleteLocalRef(jniEnv, tmp);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFThreadImpl_sys_Android_jclsHolder)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    JNIUtilDeleteGlobalRef(jniEnv, _ZFP_ZFThreadImpl_sys_Android_jclsOwner);
    _ZFP_ZFThreadImpl_sys_Android_jclsOwner = NULL;
}
ZF_GLOBAL_INITIALIZER_END(ZFThreadImpl_sys_Android_jclsHolder)

static _ZFP_ZFThreadImpl_sys_Android_NativeThreadIdType _ZFP_ZFThreadImpl_sys_Android_getNativeThreadId(void)
{
    JNIEnv *jniEnv = JNIGetJNIEnv();
    static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, _ZFP_ZFThreadImpl_sys_Android_jclsOwner, "native_currentThread",
        JNIGetMethodSig(JNIType::S_long, JNIParamTypeContainer()
        ).c_str());
    _ZFP_ZFThreadImpl_sys_Android_NativeThreadIdType ret = JNIUtilCallStaticLongMethod(jniEnv,
        _ZFP_ZFThreadImpl_sys_Android_jclsOwner,
        jmId);
    return ret;
}

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFThreadImpl_sys_Android_DataHolder, ZFLevelZFFrameworkHigh)
{
    mainThread = zfAlloc(ZFThreadMainThread);
    syncObj = zfAlloc(ZFObject);
    threadMap[_ZFP_ZFThreadImpl_sys_Android_getNativeThreadId()] = mainThread;
    executeId = 1;
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFThreadImpl_sys_Android_DataHolder)
{
    zfRelease(syncObj);
    zfRelease(mainThread);
    mainThread = zfnull;
}
public:
    ZFThread *mainThread;
    ZFObject *syncObj;
    _ZFP_ZFThreadImpl_sys_Android_ThreadMapType threadMap;
    _ZFP_ZFThreadImpl_sys_Android_ExecuteDataMapType executeDataMap;
    _ZFP_ZFThreadImpl_sys_Android_ExecuteDataIdType executeId;
ZF_GLOBAL_INITIALIZER_END(ZFThreadImpl_sys_Android_DataHolder)
#define _ZFP_ZFThreadImpl_sys_Android_syncObj (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFThreadImpl_sys_Android_DataHolder)->syncObj)
#define _ZFP_ZFThreadImpl_sys_Android_mainThreadInstance (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFThreadImpl_sys_Android_DataHolder)->mainThread)
#define _ZFP_ZFThreadImpl_sys_Android_threadMap (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFThreadImpl_sys_Android_DataHolder)->threadMap)
#define _ZFP_ZFThreadImpl_sys_Android_executeDataMap (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFThreadImpl_sys_Android_DataHolder)->executeDataMap)
#define _ZFP_ZFThreadImpl_sys_Android_executeId (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFThreadImpl_sys_Android_DataHolder)->executeId)

// ============================================================
static void _ZFP_ZFThreadImpl_sys_Android_updateExecuteId(void)
{
    ++_ZFP_ZFThreadImpl_sys_Android_executeId;
    if(_ZFP_ZFThreadImpl_sys_Android_executeId > 30000)
    {
        _ZFP_ZFThreadImpl_sys_Android_executeId = 1;
    }
}

// ============================================================
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFThreadImpl_sys_Android, ZFThread, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Android:Thread")
public:
    zfoverride
    virtual void protocolOnInit(void)
    {
        zfsuper::protocolOnInit();
        (void)ZF_GLOBAL_INITIALIZER_INSTANCE(ZFThreadImpl_sys_Android_jclsHolder);
        this->jclsOwner = _ZFP_ZFThreadImpl_sys_Android_jclsOwner;
    }

    virtual void *nativeThreadRegister(ZF_IN ZFThread *ownerZFThread)
    {
        _ZFP_ZFThreadImpl_sys_Android_NativeThreadIdType *token = zfnew(_ZFP_ZFThreadImpl_sys_Android_NativeThreadIdType);
        *token = _ZFP_ZFThreadImpl_sys_Android_getNativeThreadId();
        zfCoreAssertWithMessage(_ZFP_ZFThreadImpl_sys_Android_threadMap.find(*token) == _ZFP_ZFThreadImpl_sys_Android_threadMap.end(),
            "thread already registered: %s", ownerZFThread->objectInfo().cString());
        _ZFP_ZFThreadImpl_sys_Android_threadMap[*token] = ownerZFThread;
        return ZFCastStatic(void *, token);
    }
    virtual void nativeThreadUnregister(ZF_IN void *token)
    {
        _ZFP_ZFThreadImpl_sys_Android_NativeThreadIdType *threadId = ZFCastStatic(_ZFP_ZFThreadImpl_sys_Android_NativeThreadIdType *, token);
        _ZFP_ZFThreadImpl_sys_Android_threadMap.erase(*threadId);
        zfdelete(ZFCastStatic(_ZFP_ZFThreadImpl_sys_Android_NativeThreadIdType *, token));
    }
    virtual ZFThread *threadForToken(ZF_IN void *token)
    {
        _ZFP_ZFThreadImpl_sys_Android_ThreadMapType::iterator it = _ZFP_ZFThreadImpl_sys_Android_threadMap.find(
            *ZFCastStatic(_ZFP_ZFThreadImpl_sys_Android_NativeThreadIdType *, token));
        if(it != _ZFP_ZFThreadImpl_sys_Android_threadMap.end())
        {
            return it->second;
        }
        return zfnull;
    }

    virtual ZFThread *mainThread(void)
    {
        return _ZFP_ZFThreadImpl_sys_Android_mainThreadInstance;
    }
    virtual ZFThread *currentThread(void)
    {
        zfsynchronize(_ZFP_ZFThreadImpl_sys_Android_syncObj);
        _ZFP_ZFThreadImpl_sys_Android_ThreadMapType::const_iterator it =
            _ZFP_ZFThreadImpl_sys_Android_threadMap.find(_ZFP_ZFThreadImpl_sys_Android_getNativeThreadId());
        if(it == _ZFP_ZFThreadImpl_sys_Android_threadMap.end())
        {
            return zfnull;
        }
        return it->second;
    }

    virtual void sleep(ZF_IN zftimet miliSecs)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_sleep",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_long)
            ).c_str());
        JNIUtilCallStaticVoidMethod(jniEnv,
            this->jclsOwner,
            jmId,
            ZFCastStatic(jlong, miliSecs));
    }

    virtual void *executeInMainThread(ZF_IN zfidentity taskId,
                                      ZF_IN const ZFListener &runnable,
                                      ZF_IN ZFObject *param0,
                                      ZF_IN ZFObject *param1)
    {
        zfsynchronizeLock(_ZFP_ZFThreadImpl_sys_Android_syncObj);
        _ZFP_ZFThreadImpl_sys_Android_ExecuteData *d = zfnew(_ZFP_ZFThreadImpl_sys_Android_ExecuteData,
            _ZFP_ZFThreadImpl_sys_Android_mainThreadInstance,
            runnable,
            param0,
            param1);
        _ZFP_ZFThreadImpl_sys_Android_updateExecuteId();
        _ZFP_ZFThreadImpl_sys_Android_ExecuteDataIdType curId = _ZFP_ZFThreadImpl_sys_Android_executeId;
        _ZFP_ZFThreadImpl_sys_Android_executeDataMap[curId] = d;
        zfsynchronizeUnlock(_ZFP_ZFThreadImpl_sys_Android_syncObj);

        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_executeInMainThread",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_int)
            ).c_str());

        JNIUtilCallStaticVoidMethod(jniEnv,
            this->jclsOwner,
            jmId,
            curId);
        return zfnull;
    }
    virtual void executeInMainThreadCancel(ZF_IN zfidentity taskId,
                                           ZF_IN void *nativeToken)
    {
        // nothing to do
    }

    virtual void *executeInNewThread(ZF_IN zfidentity taskId,
                                     ZF_IN ZFThread *ownerZFThread,
                                     ZF_IN const ZFListener &runnable,
                                     ZF_IN ZFObject *param0,
                                     ZF_IN ZFObject *param1)
    {
        zfsynchronizeLock(_ZFP_ZFThreadImpl_sys_Android_syncObj);
        _ZFP_ZFThreadImpl_sys_Android_ExecuteData *d = zfnew(_ZFP_ZFThreadImpl_sys_Android_ExecuteData,
            ownerZFThread,
            runnable,
            param0,
            param1);
        _ZFP_ZFThreadImpl_sys_Android_updateExecuteId();
        _ZFP_ZFThreadImpl_sys_Android_ExecuteDataIdType curId = _ZFP_ZFThreadImpl_sys_Android_executeId;
        _ZFP_ZFThreadImpl_sys_Android_executeDataMap[curId] = d;
        zfsynchronizeUnlock(_ZFP_ZFThreadImpl_sys_Android_syncObj);

        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_executeInNewThread",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_int)
            ).c_str());

        JNIUtilCallStaticVoidMethod(jniEnv,
            this->jclsOwner,
            jmId,
            curId);
        return zfnull;
    }
    virtual void executeInNewThreadCancel(ZF_IN zfidentity taskId,
                                          ZF_IN void *nativeToken)
    {
        // nothing to do
    }

    virtual void *executeInMainThreadAfterDelay(ZF_IN zfidentity taskId,
                                                ZF_IN zftimet delay,
                                                ZF_IN const ZFListener &runnable,
                                                ZF_IN ZFObject *param0,
                                                ZF_IN ZFObject *param1)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_executeInMainThreadAfterDelay",
            JNIGetMethodSig(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object), JNIParamTypeContainer()
                .add(JNIType::S_long)
                .add(JNIPointerJNIType)
            ).c_str());

        ZFListenerHolder *nativeData = zfAlloc(ZFListenerHolder, runnable,
            ZFListenerData().param0(param0).param1(param1));
        jobject tmp = JNIUtilCallStaticObjectMethod(jniEnv,
            this->jclsOwner,
            jmId,
            (jlong)delay,
            JNIConvertZFObjectToJNIType(jniEnv, nativeData));
        JNIBlockedDeleteLocalRefWithEnv(tmp, jniEnv);
        return JNIUtilNewGlobalRef(jniEnv, tmp);
    }
    virtual void executeInMainThreadAfterDelayCancel(ZF_IN zfidentity taskId,
                                                     ZF_IN void *nativeToken)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        static jmethodID jmId = JNIUtilGetStaticMethodID(jniEnv, this->jclsOwner, "native_executeInMainThreadAfterDelayCancel",
            JNIGetMethodSig(JNIType::S_void, JNIParamTypeContainer()
                .add(JNIType::S_object(ZFImpl_sys_Android_JNI_NAME_Object))
            ).c_str());

        JNIUtilCallStaticVoidMethod(jniEnv,
            this->jclsOwner,
            jmId,
            ZFCastStatic(jobject, nativeToken));
    }
    virtual void executeInMainThreadAfterDelayCleanup(ZF_IN zfidentity taskId,
                                                      ZF_IN void *nativeToken)
    {
        JNIEnv *jniEnv = JNIGetJNIEnv();
        jobject tmp = ZFCastStatic(jobject, nativeToken);
        JNIUtilDeleteGlobalRef(jniEnv, tmp);
    }

private:
    jclass jclsOwner;
ZFPROTOCOL_IMPLEMENTATION_END(ZFThreadImpl_sys_Android)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFThreadImpl_sys_Android)

ZF_NAMESPACE_GLOBAL_END

// ============================================================
static _ZFP_ZFThreadImpl_sys_Android_ExecuteData *_ZFP_ZFThreadImpl_sys_Android_getExecuteData(_ZFP_ZFThreadImpl_sys_Android_ExecuteDataIdType executeDataId)
{
    zfsynchronize(_ZFP_ZFThreadImpl_sys_Android_syncObj);
    _ZFP_ZFThreadImpl_sys_Android_ExecuteDataMapType::iterator it = _ZFP_ZFThreadImpl_sys_Android_executeDataMap.find(executeDataId);
    if(it == _ZFP_ZFThreadImpl_sys_Android_executeDataMap.end())
    {
        zfCoreCriticalShouldNotGoHere();
        return zfnull;
    }
    _ZFP_ZFThreadImpl_sys_Android_ExecuteData *d = it->second;
    _ZFP_ZFThreadImpl_sys_Android_executeDataMap.erase(it);
    return d;
}
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFThread,
                         void, native_1doExecuteInMainThread,
                         jint executeDataId, jobject nativeThread)
{
    _ZFP_ZFThreadImpl_sys_Android_ExecuteData *d = _ZFP_ZFThreadImpl_sys_Android_getExecuteData(executeDataId);

    d->runnable.execute(ZFListenerData().param0(d->param0).param1(d->param1));
    zfdelete(d);
}
JNI_METHOD_DECLARE_END()

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFThread,
                         void, native_1doExecuteInNewThread,
                         jint executeDataId, _ZFP_ZFThreadImpl_sys_Android_NativeThreadIdType nativeThread)
{
    _ZFP_ZFThreadImpl_sys_Android_ExecuteData *d = _ZFP_ZFThreadImpl_sys_Android_getExecuteData(executeDataId);

    zfsynchronizeLock(_ZFP_ZFThreadImpl_sys_Android_syncObj);
    _ZFP_ZFThreadImpl_sys_Android_threadMap[nativeThread] = d->ownerZFThread;
    zfsynchronizeUnlock(_ZFP_ZFThreadImpl_sys_Android_syncObj);

    d->runnable.execute(ZFListenerData().param0(d->param0).param1(d->param1));
    zfdelete(d);

    zfsynchronizeLock(_ZFP_ZFThreadImpl_sys_Android_syncObj);
    _ZFP_ZFThreadImpl_sys_Android_threadMap.erase(nativeThread);
    zfsynchronizeUnlock(_ZFP_ZFThreadImpl_sys_Android_syncObj);
}
JNI_METHOD_DECLARE_END()

JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFThread,
                         void, native_1doExecuteInMainThreadAfterDelay,
                         JNIPointer zfjniPointerNativeData)
{
    ZFListenerHolder *nativeDataTmp = ZFCastZFObjectUnchecked(ZFListenerHolder *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerNativeData));
    nativeDataTmp->runnableExecute();
}
JNI_METHOD_DECLARE_END()
JNI_METHOD_DECLARE_BEGIN(ZFImpl_sys_Android_JNI_ID_ZFThread,
                         void, native_1executeInMainThreadAfterDelayCleanup,
                         JNIPointer zfjniPointerNativeData)
{
    ZFListenerHolder *nativeDataTmp = ZFCastZFObjectUnchecked(ZFListenerHolder *, JNIConvertZFObjectFromJNIType(jniEnv, zfjniPointerNativeData));
    zfRelease(nativeDataTmp);
}
JNI_METHOD_DECLARE_END()

#endif // #if ZF_ENV_sys_Android

