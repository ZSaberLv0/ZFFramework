#include "ZFImpl_default_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFThread.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"

#if ZF_ENV_sys_Windows
    #include <Windows.h>
#elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown
    #include <pthread.h>
    #include <unistd.h>
#endif

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// global type
zfclassNotPOD _ZFP_ZFThreadImpl_default_ExecuteData
{
public:
    ZFThread *ownerZFThread;
    ZFListener runnable;
    ZFObject *param0;
    ZFObject *param1;

public:
    _ZFP_ZFThreadImpl_default_ExecuteData(ZF_IN ZFThread *ownerZFThread,
                                          ZF_IN ZFListener runnable,
                                          ZF_IN ZFObject *param0,
                                          ZF_IN ZFObject *param1)
    : ownerZFThread(ownerZFThread)
    , runnable(runnable)
    , param0(param0)
    , param1(param1)
    {
    }
};

// ============================================================
// native wrapper
#if ZF_ENV_sys_Windows
typedef DWORD _ZFP_ZFThreadImpl_default_NativeThreadIdType;
static _ZFP_ZFThreadImpl_default_NativeThreadIdType _ZFP_ZFThreadImpl_default_getNativeThreadId(void)
{
    return GetCurrentThreadId();
}
static void _ZFP_ZFThreadImpl_default_sleep(zftimet miliSecs)
{
    Sleep((DWORD)miliSecs);
}
static void _ZFP_ZFThreadImpl_default_threadCallback(_ZFP_ZFThreadImpl_default_ExecuteData *data);
static DWORD WINAPI _ZFP_ZFThreadImpl_default_nativeCallback(LPVOID param)
{
    _ZFP_ZFThreadImpl_default_threadCallback(ZFCastStatic(_ZFP_ZFThreadImpl_default_ExecuteData *, param));
    return 0;
}
static void _ZFP_ZFThreadImpl_default_startNativeThread(_ZFP_ZFThreadImpl_default_ExecuteData *data)
{
    CreateThread(NULL, 0, _ZFP_ZFThreadImpl_default_nativeCallback, data, 0, NULL);
}
#elif ZF_ENV_sys_Posix || ZF_ENV_sys_unknown
typedef pthread_t _ZFP_ZFThreadImpl_default_NativeThreadIdType;
static _ZFP_ZFThreadImpl_default_NativeThreadIdType _ZFP_ZFThreadImpl_default_getNativeThreadId(void)
{
    return pthread_self();
}
static void _ZFP_ZFThreadImpl_default_sleep(zftimet miliSecs)
{
    usleep((unsigned int)(miliSecs * 1000));
}
static void _ZFP_ZFThreadImpl_default_threadCallback(_ZFP_ZFThreadImpl_default_ExecuteData *data);
static void *_ZFP_ZFThreadImpl_default_nativeCallback(void *param)
{
    _ZFP_ZFThreadImpl_default_threadCallback(ZFCastStatic(_ZFP_ZFThreadImpl_default_ExecuteData *, param));
    return zfnull;
}
static void _ZFP_ZFThreadImpl_default_startNativeThread(_ZFP_ZFThreadImpl_default_ExecuteData *data)
{
    pthread_t tid = 0;
    pthread_create(&tid, NULL, _ZFP_ZFThreadImpl_default_nativeCallback, data);
}
#endif

// ============================================================
// global data
typedef zfstlmap<_ZFP_ZFThreadImpl_default_NativeThreadIdType, ZFThread *> _ZFP_ZFThreadImpl_default_ThreadMapType;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFThreadImpl_default_DataHolder, ZFLevelZFFrameworkHigh)
{
    mainThread = zfAlloc(ZFThreadMainThread);
    syncObj = zfAlloc(ZFObject);
    threadMap[_ZFP_ZFThreadImpl_default_getNativeThreadId()] = mainThread;
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFThreadImpl_default_DataHolder)
{
    zfRelease(syncObj);
    zfRelease(mainThread);
}
public:
    ZFThread *mainThread;
    ZFObject *syncObj;
    _ZFP_ZFThreadImpl_default_ThreadMapType threadMap;
ZF_GLOBAL_INITIALIZER_END(ZFThreadImpl_default_DataHolder)
#define _ZFP_ZFThreadImpl_default_syncObj (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFThreadImpl_default_DataHolder)->syncObj)
#define _ZFP_ZFThreadImpl_default_mainThread (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFThreadImpl_default_DataHolder)->mainThread)
#define _ZFP_ZFThreadImpl_default_threadMap (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFThreadImpl_default_DataHolder)->threadMap)

void _ZFP_ZFThreadImpl_default_threadCallback(_ZFP_ZFThreadImpl_default_ExecuteData *data)
{
    _ZFP_ZFThreadImpl_default_NativeThreadIdType nativeCurrentThreadId = _ZFP_ZFThreadImpl_default_getNativeThreadId();
    zfsynchronizeLock(_ZFP_ZFThreadImpl_default_syncObj);
    _ZFP_ZFThreadImpl_default_threadMap[nativeCurrentThreadId] = data->ownerZFThread;
    zfsynchronizeUnlock(_ZFP_ZFThreadImpl_default_syncObj);

    data->runnable.execute(ZFListenerData().param0(data->param0).param1(data->param1));

    zfsynchronizeLock(_ZFP_ZFThreadImpl_default_syncObj);
    _ZFP_ZFThreadImpl_default_threadMap.erase(nativeCurrentThreadId);
    zfsynchronizeUnlock(_ZFP_ZFThreadImpl_default_syncObj);

    zfdelete(data);
}

// ============================================================
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFThreadImpl_default, ZFThread, ZFProtocolLevel::e_Default)
public:
    virtual void *nativeThreadRegister(ZF_IN ZFThread *ownerZFThread)
    {
        _ZFP_ZFThreadImpl_default_NativeThreadIdType *token = zfnew(_ZFP_ZFThreadImpl_default_NativeThreadIdType);
        *token = _ZFP_ZFThreadImpl_default_getNativeThreadId();
        zfCoreAssertWithMessage(_ZFP_ZFThreadImpl_default_threadMap.find(*token) == _ZFP_ZFThreadImpl_default_threadMap.end(),
            "thread already registered: %s", ownerZFThread->objectInfo().cString());
        _ZFP_ZFThreadImpl_default_threadMap[*token] = ownerZFThread;
        return ZFCastStatic(void *, token);
    }
    virtual void nativeThreadUnregister(ZF_IN void *token)
    {
        _ZFP_ZFThreadImpl_default_threadMap.erase(_ZFP_ZFThreadImpl_default_getNativeThreadId());
        zfdelete(ZFCastStatic(_ZFP_ZFThreadImpl_default_NativeThreadIdType *, token));
    }
    virtual ZFThread *threadForToken(ZF_IN void *token)
    {
        _ZFP_ZFThreadImpl_default_ThreadMapType::iterator it = _ZFP_ZFThreadImpl_default_threadMap.find(
            *ZFCastStatic(_ZFP_ZFThreadImpl_default_NativeThreadIdType *, token));
        if(it != _ZFP_ZFThreadImpl_default_threadMap.end())
        {
            return it->second;
        }
        return zfnull;
    }
    virtual ZFThread *mainThread(void)
    {
        return _ZFP_ZFThreadImpl_default_mainThread;
    }
    virtual ZFThread *currentThread(void)
    {
        _ZFP_ZFThreadImpl_default_NativeThreadIdType nativeCurrentThread = _ZFP_ZFThreadImpl_default_getNativeThreadId();

        zfsynchronize(_ZFP_ZFThreadImpl_default_syncObj);
        _ZFP_ZFThreadImpl_default_ThreadMapType::const_iterator it = _ZFP_ZFThreadImpl_default_threadMap.find(nativeCurrentThread);
        if(it == _ZFP_ZFThreadImpl_default_threadMap.end())
        {
            zfCoreLogTrim("current thread is null, make sure the thread is started or registered by ZFThread");
            return zfnull;
        }
        return it->second;
    }

    virtual void sleep(ZF_IN zftimet miliSecs)
    {
        _ZFP_ZFThreadImpl_default_sleep(miliSecs);
    }

    virtual void *executeInMainThread(ZF_IN zfidentity taskId,
                                      ZF_IN const ZFListener &runnable,
                                      ZF_IN ZFObject *param0,
                                      ZF_IN ZFObject *param1)
    {
        zfCoreCriticalMessageTrim("[ZFThread] executeInMainThread not available");
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
        _ZFP_ZFThreadImpl_default_ExecuteData *data = zfnew(_ZFP_ZFThreadImpl_default_ExecuteData, ownerZFThread, runnable, param0, param1);
        _ZFP_ZFThreadImpl_default_startNativeThread(data);
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
        zfCoreCriticalMessageTrim("[ZFThread] executeInMainThreadAfterDelay not available");
        return zfnull;
    }
    virtual void executeInMainThreadAfterDelayCancel(ZF_IN zfidentity taskId,
                                                     ZF_IN void *nativeToken)
    {
        zfCoreCriticalMessageTrim("[ZFThread] executeInMainThreadAfterDelay not available");
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFThreadImpl_default)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFThreadImpl_default)

ZF_NAMESPACE_GLOBAL_END

