#include "ZFThread.h"
#include "protocol/ZFProtocolZFThread.h"
#include "protocol/ZFProtocolZFThreadTaskRequest.h"
#include "ZFAutoReleasePool.h"
#include "ZFSemaphore.h"
#include "ZFMutex.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
#define _ZFP_ZFThreadImpl ZFPROTOCOL_ACCESS(ZFThread)

zfclass _ZFP_I_ZFThreadMutex : zfextends ZFMutex
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFThreadMutex, ZFMutex)
};
ZFMutex *_ZFP_ZFThread_mutex = zfnull;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFThreadMutexHolder, ZFLevelZFFrameworkEssential)
{
    _ZFP_ZFThread_mutex = zfAlloc(_ZFP_I_ZFThreadMutex);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFThreadMutexHolder)
{
    zfRelease(_ZFP_ZFThread_mutex);
    _ZFP_ZFThread_mutex = zfnull;
}
ZF_GLOBAL_INITIALIZER_END(ZFThreadMutexHolder)

// ============================================================
// _ZFP_ZFThreadPrivate
zfclassNotPOD _ZFP_ZFThreadPrivate
{
public:
    ZFAutoReleasePool *autoReleasePool;
    zfbool autoReleasePoolNeedDrain;
    /**
     * no auto-retain, used in ZFThread::threadWait,
     * same as runnable data's semaWait for run task
     */
    ZFSemaphore *semaWaitHolder;
    zfidentity taskId;
    zfbool startFlag;
    zfbool runningFlag;
    zfbool stopRequestedFlag;

public:
    _ZFP_ZFThreadPrivate(void)
    : autoReleasePool(zfnull)
    , autoReleasePoolNeedDrain(zffalse)
    , semaWaitHolder(zfnull)
    , taskId(zfidentityInvalid())
    , startFlag(zffalse)
    , runningFlag(zffalse)
    , stopRequestedFlag(zffalse)
    {
    }
    ~_ZFP_ZFThreadPrivate(void)
    {
        zfRelease(this->autoReleasePool);
    }
};

// ============================================================
// runnable data
typedef enum {
    _ZFP_ZFThreadRunnableTypeExecuteInMainThread,
    _ZFP_ZFThreadRunnableTypeExecuteInNewThread,
    _ZFP_ZFThreadRunnableTypeExecuteInMainThreadAfterDelay,
} _ZFP_ZFThreadRunnableType;
typedef enum {
    _ZFP_ZFThreadRunStatePending,
    _ZFP_ZFThreadRunStateRunning,
    _ZFP_ZFThreadRunStateFinished,
} _ZFP_ZFThreadRunState;
zfclass _ZFP_I_ZFThreadRunnableData : zfextends ZFObject
{
    ZFOBJECT_DECLARE_WITH_CUSTOM_CTOR(_ZFP_I_ZFThreadRunnableData, ZFObject)

public:
    zfidentity taskId;
    _ZFP_ZFThreadRunnableType runnableType;
    void *nativeToken;
    ZFListener _runnable;
    ZFThread *ownerZFThread; // no auto-retain, null if main thread
    _ZFP_ZFThreadPrivate *ownerZFThreadPrivate; // null if main thread
    ZFObject *userData; // auto-retain
    ZFListenerData listenerData; // no auto-retain
    ZFObject *owner; // no auto-retain
    ZFSemaphore *semaWait; // not null, no auto-retain, used int ZFThreadExecuteWait and waitUntilDone, also used to notify task observer
    _ZFP_ZFThreadRunState runState;

public:
    inline void runnable(ZF_IN const ZFListener &runnable)
    {
        zfassert(runnable.callbackIsValid());
        runnable.callbackOwnerObjectRetain();
        this->_runnable.callbackOwnerObjectRelease();
        this->_runnable = runnable;
    }
    inline ZFListener &runnable(void)
    {
        return this->_runnable;
    }

protected:
    zfoverride
    virtual void objectOnDeallocPrepare(void)
    {
        this->_runnable.callbackOwnerObjectRelease();
        zfsuper::objectOnDeallocPrepare();
    }

protected:
    _ZFP_I_ZFThreadRunnableData(void)
    : taskId(zfidentityInvalid())
    , runnableType(_ZFP_ZFThreadRunnableTypeExecuteInMainThread)
    , nativeToken(zfnull)
    , _runnable()
    , ownerZFThread(zfnull)
    , ownerZFThreadPrivate(zfnull)
    , userData(zfnull)
    , listenerData()
    , owner(zfnull)
    , semaWait(zfnull)
    , runState(_ZFP_ZFThreadRunStatePending)
    {
    }
};

// ============================================================
// data holder
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFThreadDataHolder, ZFLevelZFFrameworkEssential)
{
}
public:
    ZFCoreArrayPOD<_ZFP_I_ZFThreadRunnableData *> runnableList;
    ZFIdentityGenerator idGenerator;
ZF_GLOBAL_INITIALIZER_END(ZFThreadDataHolder)
#define _ZFP_ZFThread_runnableList (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFThreadDataHolder)->runnableList)
#define _ZFP_ZFThread_idGenerator (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFThreadDataHolder)->idGenerator)

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFThreadExecute_AutoCancel, ZFLevelZFFrameworkLow)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFThreadExecute_AutoCancel)
{
    zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
    if(lockAvailable)
    {
        zfsynchronizeLock(_ZFP_ZFThread_mutex);
    }
    ZFCoreArrayPOD<zfidentity> allTaskId;
    const ZFCoreArrayPOD<_ZFP_I_ZFThreadRunnableData *> &runnableList = _ZFP_ZFThread_runnableList;
    for(zfindex i = 0; i < runnableList.count(); ++i)
    {
        allTaskId.add(runnableList[i]->taskId);
    }
    if(lockAvailable)
    {
        zfsynchronizeUnlock(_ZFP_ZFThread_mutex);
    }

    for(zfindex i = 0; i < allTaskId.count(); ++i)
    {
        ZFThreadExecuteCancel(allTaskId[i]);
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFThreadExecute_AutoCancel)

// ============================================================
static void _ZFP_ZFThreadRunnableCleanup(ZF_IN _ZFP_I_ZFThreadRunnableData *runnableData);
static zfidentity _ZFP_ZFThreadExecuteInNewThread(ZF_IN const ZFListener &runnable,
                                                  ZF_IN ZFObject *userData,
                                                  ZF_IN const ZFListenerData &listenerData,
                                                  ZF_IN ZFObject *owner,
                                                  ZF_IN ZFThread *ownerZFThread,
                                                  ZF_IN _ZFP_ZFThreadPrivate *ownerZFThreadPrivate);
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFThreadCallback)
{
    zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
    _ZFP_I_ZFThreadRunnableData *runnableData = listenerData.param0<_ZFP_I_ZFThreadRunnableData *>();
    ZFThread *ownerZFThreadFixed = ((runnableData->ownerZFThread == zfnull) ? ZFThread::currentThread() : runnableData->ownerZFThread);

    if(lockAvailable)
    {
        zfsynchronizeLock(_ZFP_ZFThread_mutex);
    }
    if(runnableData->runState == _ZFP_ZFThreadRunStatePending)
    {
        runnableData->runState = _ZFP_ZFThreadRunStateRunning;

        if(lockAvailable)
        {
            zfsynchronizeUnlock(_ZFP_ZFThread_mutex);
        }

        if(runnableData->ownerZFThreadPrivate != zfnull)
        {
            runnableData->ownerZFThreadPrivate->runningFlag = zftrue;
        }

        // start
        if(runnableData->ownerZFThread != zfnull)
        {
            runnableData->ownerZFThread->_ZFP_ZFThread_threadOnStart(
                runnableData->listenerData.param0(), runnableData->listenerData.param1());
        }
        runnableData->semaWait->observerNotifyWithCustomSender(
            ownerZFThreadFixed, ZFThread::EventThreadOnStart(),
            runnableData->listenerData.param0(), runnableData->listenerData.param1());

        // running
        runnableData->runnable().execute(runnableData->listenerData, runnableData->userData);

        // stop
        if(runnableData->ownerZFThread != zfnull)
        {
            runnableData->ownerZFThread->_ZFP_ZFThread_threadOnStop(
                runnableData->listenerData.param0(), runnableData->listenerData.param1());
        }
        runnableData->semaWait->observerNotifyWithCustomSender(
            ownerZFThreadFixed, ZFThread::EventThreadOnStop(),
            runnableData->listenerData.param0(), runnableData->listenerData.param1());

        if(lockAvailable)
        {
            zfsynchronizeLock(_ZFP_ZFThread_mutex);
        }

        runnableData->runState = _ZFP_ZFThreadRunStateFinished;
    }

    if(runnableData->ownerZFThreadPrivate != zfnull)
    {
        runnableData->ownerZFThreadPrivate->stopRequestedFlag = zffalse;
    }
    if(lockAvailable)
    {
        zfsynchronizeUnlock(_ZFP_ZFThread_mutex);
    }

    _ZFP_ZFThreadRunnableCleanup(runnableData);
}
static void _ZFP_ZFThreadRunnableCleanup(ZF_IN _ZFP_I_ZFThreadRunnableData *runnableData)
{
    zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
    if(lockAvailable)
    {
        zfsynchronizeLock(_ZFP_ZFThread_mutex);
    }

    {
        zfbool exist = zffalse;
        ZFCoreArrayPOD<_ZFP_I_ZFThreadRunnableData *> &runnableList = _ZFP_ZFThread_runnableList;
        for(zfindex i = 0; i < runnableList.count(); ++i)
        {
            if(runnableList.get(i) == runnableData)
            {
                runnableList.remove(i);
                exist = zftrue;
                break;
            }
        }
        if(!exist)
        {
            if(lockAvailable)
            {
                zfsynchronizeUnlock(_ZFP_ZFThread_mutex);
            }
            return ;
        }
    }

    zfidentity taskIdSaved = runnableData->taskId;

    runnableData->semaWait->observerRemoveAll(ZFThread::EventThreadOnStart());
    runnableData->semaWait->observerRemoveAll(ZFThread::EventThreadOnStop());
    runnableData->semaWait->observerRemoveAll(ZFThread::EventThreadOnCancel());

    if(runnableData->ownerZFThread != zfnull)
    {
        runnableData->ownerZFThread->autoReleasePoolDrain();
    }
    if(runnableData->ownerZFThreadPrivate != zfnull)
    {
        runnableData->ownerZFThreadPrivate->startFlag = zffalse;
        runnableData->ownerZFThreadPrivate->runningFlag = zffalse;
        runnableData->ownerZFThreadPrivate->semaWaitHolder = zfnull;
    }
    switch(runnableData->runnableType)
    {
        case _ZFP_ZFThreadRunnableTypeExecuteInMainThread:
            _ZFP_ZFThreadImpl->executeInMainThreadCleanup(runnableData->taskId, runnableData->nativeToken);
            break;
        case _ZFP_ZFThreadRunnableTypeExecuteInNewThread:
            _ZFP_ZFThreadImpl->executeInNewThreadCleanup(runnableData->taskId, runnableData->nativeToken);
            break;
        case _ZFP_ZFThreadRunnableTypeExecuteInMainThreadAfterDelay:
            _ZFP_ZFThreadImpl->executeInMainThreadAfterDelayCleanup(runnableData->taskId, runnableData->nativeToken);
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
    zfRelease(runnableData->ownerZFThread);
    zfRelease(runnableData->listenerData.sender());
    zfRelease(runnableData->listenerData.param0());
    zfRelease(runnableData->listenerData.param1());
    zfRelease(runnableData->userData);

    if(lockAvailable)
    {
        zfsynchronizeUnlock(_ZFP_ZFThread_mutex);
    }

    runnableData->semaWait->semaphoreBroadcastLocked();
    zfRelease(runnableData->semaWait);
    runnableData->semaWait = zfnull;
    zfRelease(runnableData);

    if(lockAvailable)
    {
        zfsynchronizeLock(_ZFP_ZFThread_mutex);
    }
    _ZFP_ZFThread_idGenerator.idRelease(taskIdSaved);
    if(lockAvailable)
    {
        zfsynchronizeUnlock(_ZFP_ZFThread_mutex);
    }
}

// ============================================================
// user registered thread
zfclass _ZFP_I_ZFThreadUserRegisteredThread : zfextends ZFThread
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFThreadUserRegisteredThread, ZFThread)

public:
    zfoverride
    virtual void threadStart(ZF_IN_OPT ZFObject *userData = zfnull,
                             ZF_IN_OPT ZFObject *param0 = zfnull,
                             ZF_IN_OPT ZFObject *param1 = zfnull)
    {
        zfCoreLogTrim("you must not start a user registered thread");
    }
    zfoverride
    virtual zfbool threadStarted(void)
    {
        return zftrue;
    }
    zfoverride
    virtual zfbool threadRunning(void)
    {
        return zftrue;
    }
    zfoverride
    virtual void threadStop(void)
    {
        zfCoreLogTrim("you must not stop a user registered thread");
    }
};

// ============================================================
// ZFThread
ZFOBJECT_REGISTER(ZFThread)

ZFOBSERVER_EVENT_REGISTER(ZFThread, ThreadOnStart)
ZFOBSERVER_EVENT_REGISTER(ZFThread, ThreadOnStop)
ZFOBSERVER_EVENT_REGISTER(ZFThread, ThreadOnCancel)

ZFMETHOD_DEFINE_0(ZFThread, void *, nativeThreadRegister)
{
    ZFThread *zfThread = zfAlloc(_ZFP_I_ZFThreadUserRegisteredThread);
    zfThread->_ZFP_ZFThread_d->semaWaitHolder = zfAlloc(ZFSemaphore);
    return _ZFP_ZFThreadImpl->nativeThreadRegister(zfThread);
}
ZFMETHOD_DEFINE_1(ZFThread, void, nativeThreadUnregister,
                  ZFMP_IN(void *, token))
{
    if(token != zfnull)
    {
        ZFThread *zfThread = ZFPROTOCOL_ACCESS(ZFThread)->threadForToken(token);
        zfCoreAssert(zfThread != zfnull);
        zfThread->_ZFP_ZFThread_d->semaWaitHolder->semaphoreBroadcastLocked();
        _ZFP_ZFThreadImpl->nativeThreadUnregister(token);
        zfRelease(zfThread->_ZFP_ZFThread_d->semaWaitHolder);
        zfThread->_ZFP_ZFThread_d->semaWaitHolder = zfnull;
        zfRelease(zfThread);
    }
}

ZFMETHOD_DEFINE_0(ZFThread, ZFThread *, mainThread)
{
    return _ZFP_ZFThreadImpl->mainThread();
}
ZFMETHOD_DEFINE_0(ZFThread, ZFThread *, currentThread)
{
    return _ZFP_ZFThreadImpl->currentThread();
}

ZFMETHOD_DEFINE_1(ZFThread, void, sleep,
                  ZFMP_IN(zftimet, miliSecs))
{
    _ZFP_ZFThreadImpl->sleep(miliSecs);
}

// ============================================================
// zfautoRelease
static ZFObject *_ZFP_ZFThread_drainPoolCallbackMethod(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject *userData)
{
    ZFThread *thread = ZFCastZFObjectUnchecked(ZFThread *, userData);
    thread->autoReleasePoolDrain();
    thread->_ZFP_ZFThreadAutoReleasePoolMarkResolved();
    return zfnull;
}
static ZFListener *_ZFP_ZFThread_drainPoolCallback = zfnull;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFThreadAutoReleasePoolDrainDataHolder, ZFLevelZFFrameworkEssential)
{
    _ZFP_ZFThread_drainPoolCallback = zfnew(ZFListener);
    *_ZFP_ZFThread_drainPoolCallback = ZFCallbackForFunc(_ZFP_ZFThread_drainPoolCallbackMethod);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFThreadAutoReleasePoolDrainDataHolder)
{
    zfdelete(_ZFP_ZFThread_drainPoolCallback);
    _ZFP_ZFThread_drainPoolCallback = zfnull;
}
ZF_GLOBAL_INITIALIZER_END(ZFThreadAutoReleasePoolDrainDataHolder)

// ============================================================
// thread instance method
ZFOBJECT_ON_INIT_DEFINE_1(ZFThread,
                          ZFMP_IN(const ZFListener &, runnable))
{
    this->objectOnInit();
    zfself::threadRunnable(runnable);
}

void ZFThread::objectOnInit(void)
{
    zfsuper::objectOnInit();
    _ZFP_ZFThread_d = zfpoolNew(_ZFP_ZFThreadPrivate);
    _ZFP_ZFThread_d->autoReleasePool = zfAlloc(ZFAutoReleasePool);
}
void ZFThread::objectOnDealloc(void)
{
    zfpoolDelete(_ZFP_ZFThread_d);
    _ZFP_ZFThread_d = zfnull;
    zfsuper::objectOnDealloc();
}
void ZFThread::objectOnDeallocPrepare(void)
{
    ZFThreadTaskCancelWithOwner(this);

    if(!this->isMainThread())
    {
        this->threadWait();
    }
    this->autoReleasePoolDrain();
    zfsuper::objectOnDeallocPrepare();
}

void ZFThread::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);
    if(this->isMainThread())
    {
        ret += " MainThread";
    }
}

ZFMETHOD_DEFINE_3(ZFThread, void, threadStart,
                  ZFMP_IN_OPT(ZFObject *, userData, zfnull),
                  ZFMP_IN_OPT(ZFObject *, param0, zfnull),
                  ZFMP_IN_OPT(ZFObject *, param1, zfnull))
{
    zfidentity taskIdTmp = _ZFP_ZFThreadExecuteInNewThread(
        this->threadRunnable().callbackIsValid()
            ? this->threadRunnable()
            : ZFListener(ZFCallbackForMemberMethod(this, ZFMethodAccess(ZFThread, threadOnRun))),
        userData,
        ZFListenerData().param0(param0).param1(param1),
        zfnull,
        this,
        _ZFP_ZFThread_d);
    if(taskIdTmp != zfidentityInvalid())
    {
        _ZFP_ZFThread_d->taskId = taskIdTmp;
    }
}
ZFMETHOD_DEFINE_0(ZFThread, zfbool, threadStarted)
{
    return _ZFP_ZFThread_d->startFlag;
}
ZFMETHOD_DEFINE_0(ZFThread, zfbool, threadRunning)
{
    return _ZFP_ZFThread_d->runningFlag;
}
ZFMETHOD_DEFINE_0(ZFThread, void, threadStop)
{
    if(_ZFP_ZFThread_d->startFlag)
    {
        _ZFP_ZFThread_d->stopRequestedFlag = zftrue;
        ZFThreadExecuteCancel(_ZFP_ZFThread_d->taskId);
    }
}
ZFMETHOD_DEFINE_0(ZFThread, zfbool, threadStopRequested)
{
    return _ZFP_ZFThread_d->stopRequestedFlag;
}

ZFMETHOD_DEFINE_0(ZFThread, void, threadWait)
{
    if(_ZFP_ZFThread_d->semaWaitHolder != zfnull)
    {
        _ZFP_ZFThread_d->semaWaitHolder->semaphoreWaitLocked();
    }
}
ZFMETHOD_DEFINE_1(ZFThread, zfbool, threadWait,
                  ZFMP_IN(zftimet, miliSecs))
{
    if(_ZFP_ZFThread_d->semaWaitHolder != zfnull)
    {
        return _ZFP_ZFThread_d->semaWaitHolder->semaphoreWaitLocked(miliSecs);
    }
    return zftrue;
}

ZFMETHOD_DEFINE_0(ZFThread, zfbool, isMainThread)
{
    return zffalse;
}

void ZFThread::autoReleasePoolAdd(ZF_IN ZFObject *obj)
{
    _ZFP_ZFThread_d->autoReleasePool->poolAdd(obj);
}

void ZFThread::autoReleasePoolDrain()
{
    _ZFP_ZFThread_d->autoReleasePool->poolDrain();
}

void ZFThread::_ZFP_ZFThreadAutoReleasePoolAdd(ZF_IN ZFObject *obj)
{
    zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
    if(lockAvailable)
    {
        zfsynchronizeLock(_ZFP_ZFThread_mutex);
    }

    this->autoReleasePoolAdd(obj);
    if(!_ZFP_ZFThread_d->autoReleasePoolNeedDrain)
    {
        _ZFP_ZFThread_d->autoReleasePoolNeedDrain = zftrue;
        ZFThreadTaskRequest(*_ZFP_ZFThread_drainPoolCallback, this);
    }

    if(lockAvailable)
    {
        zfsynchronizeUnlock(_ZFP_ZFThread_mutex);
    }
}
void ZFThread::_ZFP_ZFThreadAutoReleasePoolMarkResolved(void)
{
    _ZFP_ZFThread_d->autoReleasePoolNeedDrain = zffalse;
}

ZFMETHOD_DEFINE_2(ZFThread, void, threadOnRun,
                  ZFMP_IN(const ZFListenerData &, listenerData),
                  ZFMP_IN(ZFObject *, userData))
{
    // nothing to do
}

// ============================================================
// thread execute
ZFMETHOD_FUNC_DEFINE_5(zfidentity, ZFThreadExecuteInMainThread,
                       ZFMP_IN(const ZFListener &, runnable),
                       ZFMP_IN_OPT(ZFObject *, userData, zfnull),
                       ZFMP_IN_OPT(const ZFListenerData &, listenerData, ZFListenerData()),
                       ZFMP_IN_OPT(ZFObject *, owner, zfnull),
                       ZFMP_IN_OPT(zfbool, waitUntilDone, zffalse))
{
    if(!runnable.callbackIsValid())
    {
        return zfidentityInvalid();
    }
    ZFThread *curThread = ZFThread::currentThread();
    if(curThread != zfnull && curThread->isMainThread() && waitUntilDone)
    {
        runnable.execute(listenerData, userData);
        return zfidentityInvalid();
    }

    zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
    if(lockAvailable)
    {
        zfsynchronizeLock(_ZFP_ZFThread_mutex);
    }
    _ZFP_I_ZFThreadRunnableData *runnableData = zfAlloc(_ZFP_I_ZFThreadRunnableData);
    zfidentity taskId = _ZFP_ZFThread_idGenerator.idAcquire();
    runnableData->taskId = taskId;
    runnableData->runnableType = _ZFP_ZFThreadRunnableTypeExecuteInMainThread;
    runnableData->runnable(runnable);
    runnableData->listenerData = listenerData;
    {
        zfRetain(listenerData.sender());
        zfRetain(listenerData.param0());
        zfRetain(listenerData.param1());
    }
    runnableData->owner = owner;
    runnableData->userData = zfRetain(userData);
    runnableData->semaWait = zfAlloc(ZFSemaphore);
    _ZFP_ZFThread_runnableList.add(runnableData);
    if(lockAvailable)
    {
        zfsynchronizeUnlock(_ZFP_ZFThread_mutex);
    }

    zfRetain(runnableData);
    zfRetain(runnableData->semaWait);
    runnableData->nativeToken = _ZFP_ZFThreadImpl->executeInMainThread(
        taskId,
        ZFCallbackForFunc(_ZFP_ZFThreadCallback),
        runnableData,
        zfnull);

    if(waitUntilDone)
    {
        runnableData->semaWait->semaphoreWaitLocked();
    }
    zfRelease(runnableData->semaWait);
    zfRelease(runnableData);

    return taskId;
}
ZFMETHOD_FUNC_INLINE_DEFINE_4(zfidentity, ZFThreadExecuteInMainThreadWaitUntilDone,
                              ZFMP_IN(const ZFListener &, runnable),
                              ZFMP_IN_OPT(ZFObject *, userData, zfnull),
                              ZFMP_IN_OPT(const ZFListenerData &, listenerData, ZFListenerData()),
                              ZFMP_IN_OPT(ZFObject *, owner, zfnull))

static zfidentity _ZFP_ZFThreadExecuteInNewThread(ZF_IN const ZFListener &runnable,
                                                  ZF_IN ZFObject *userData,
                                                  ZF_IN const ZFListenerData &listenerData,
                                                  ZF_IN ZFObject *owner,
                                                  ZF_IN ZFThread *ownerZFThread,
                                                  ZF_IN _ZFP_ZFThreadPrivate *ownerZFThreadPrivate)
{
    zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
    if(lockAvailable)
    {
        zfsynchronizeLock(_ZFP_ZFThread_mutex);
    }
    if(ownerZFThreadPrivate->startFlag)
    {
        if(lockAvailable)
        {
            zfsynchronizeUnlock(_ZFP_ZFThread_mutex);
        }
        return zfidentityInvalid();
    }
    ownerZFThreadPrivate->startFlag = zftrue;

    _ZFP_I_ZFThreadRunnableData *runnableData = zfAlloc(_ZFP_I_ZFThreadRunnableData);
    zfidentity taskId = _ZFP_ZFThread_idGenerator.idAcquire();
    runnableData->taskId = taskId;
    runnableData->runnableType = _ZFP_ZFThreadRunnableTypeExecuteInNewThread;
    runnableData->runnable(runnable);
    runnableData->ownerZFThread = zfRetain(ownerZFThread);
    runnableData->ownerZFThreadPrivate = ownerZFThreadPrivate;
    runnableData->listenerData = listenerData;
    {
        zfRetain(listenerData.sender());
        zfRetain(listenerData.param0());
        zfRetain(listenerData.param1());
    }
    runnableData->owner = owner;
    runnableData->userData = zfRetain(userData);
    runnableData->semaWait = zfAlloc(ZFSemaphore);
    if(runnableData->ownerZFThreadPrivate != zfnull)
    {
        runnableData->ownerZFThreadPrivate->semaWaitHolder = runnableData->semaWait;
    }
    _ZFP_ZFThread_runnableList.add(runnableData);
    if(lockAvailable)
    {
        zfsynchronizeUnlock(_ZFP_ZFThread_mutex);
    }
    runnableData->nativeToken = _ZFP_ZFThreadImpl->executeInNewThread(
        taskId,
        ownerZFThread,
        ZFCallbackForFunc(_ZFP_ZFThreadCallback),
        runnableData,
        zfnull);
    return taskId;
}
ZFMETHOD_FUNC_DEFINE_4(zfidentity, ZFThreadExecuteInNewThread,
                       ZFMP_IN(const ZFListener &, runnable),
                       ZFMP_IN_OPT(ZFObject *, userData, zfnull),
                       ZFMP_IN_OPT(const ZFListenerData &, listenerData, ZFListenerData()),
                       ZFMP_IN_OPT(ZFObject *, owner, zfnull))
{
    if(!runnable.callbackIsValid())
    {
        return zfidentityInvalid();
    }

    ZFThread *tmpThread = zfAlloc(ZFThread);
    tmpThread->threadRunnable(runnable);
    zfidentity taskId = _ZFP_ZFThreadExecuteInNewThread(
        runnable,
        userData,
        listenerData,
        owner,
        tmpThread,
        tmpThread->_ZFP_ZFThread_d);
    zfRelease(tmpThread);
    return taskId;
}

ZFMETHOD_FUNC_DEFINE_5(zfidentity, ZFThreadExecuteInMainThreadAfterDelay,
                       ZFMP_IN(zftimet, delay),
                       ZFMP_IN(const ZFListener &, runnable),
                       ZFMP_IN_OPT(ZFObject *, userData, zfnull),
                       ZFMP_IN_OPT(const ZFListenerData &, listenerData, ZFListenerData()),
                       ZFMP_IN_OPT(ZFObject *, owner, zfnull))
{
    if(delay <= 0)
    {
        return ZFThreadExecuteInMainThread(runnable, userData, listenerData, owner);
    }
    if(!runnable.callbackIsValid())
    {
        return zfidentityInvalid();
    }

    zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
    if(lockAvailable)
    {
        zfsynchronizeLock(_ZFP_ZFThread_mutex);
    }
    _ZFP_I_ZFThreadRunnableData *runnableData = zfAlloc(_ZFP_I_ZFThreadRunnableData);
    zfidentity taskId = _ZFP_ZFThread_idGenerator.idAcquire();
    runnableData->taskId = taskId;
    runnableData->runnableType = _ZFP_ZFThreadRunnableTypeExecuteInMainThreadAfterDelay;
    runnableData->runnable(runnable);
    runnableData->listenerData = listenerData;
    {
        zfRetain(listenerData.sender());
        zfRetain(listenerData.param0());
        zfRetain(listenerData.param1());
    }
    runnableData->owner = owner;
    runnableData->userData = zfRetain(userData);
    runnableData->semaWait = zfAlloc(ZFSemaphore);
    _ZFP_ZFThread_runnableList.add(runnableData);
    if(lockAvailable)
    {
        zfsynchronizeUnlock(_ZFP_ZFThread_mutex);
    }

    zfRetain(runnableData);
    zfRetain(runnableData->semaWait);
    runnableData->nativeToken = _ZFP_ZFThreadImpl->executeInMainThreadAfterDelay(
        taskId,
        delay,
        ZFCallbackForFunc(_ZFP_ZFThreadCallback),
        runnableData,
        zfnull);
    zfRelease(runnableData->semaWait);
    zfRelease(runnableData);
    return taskId;
}

static void _ZFP_ZFThreadDoCancelTask(ZF_IN _ZFP_I_ZFThreadRunnableData *runnableData)
{
    if(runnableData->runState != _ZFP_ZFThreadRunStatePending)
    {
        return ;
    }
    runnableData->runState = _ZFP_ZFThreadRunStateFinished;
    switch(runnableData->runnableType)
    {
        case _ZFP_ZFThreadRunnableTypeExecuteInMainThread:
            _ZFP_ZFThreadImpl->executeInMainThreadCancel(runnableData->taskId, runnableData->nativeToken);
            break;
        case _ZFP_ZFThreadRunnableTypeExecuteInNewThread:
            _ZFP_ZFThreadImpl->executeInNewThreadCancel(runnableData->taskId, runnableData->nativeToken);
            break;
        case _ZFP_ZFThreadRunnableTypeExecuteInMainThreadAfterDelay:
            _ZFP_ZFThreadImpl->executeInMainThreadAfterDelayCancel(runnableData->taskId, runnableData->nativeToken);
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            return ;
    }
    if(runnableData->ownerZFThreadPrivate != zfnull)
    {
        if(runnableData->ownerZFThreadPrivate->startFlag)
        {
            runnableData->ownerZFThreadPrivate->stopRequestedFlag = zftrue;
        }
    }
    if(runnableData->ownerZFThread != zfnull)
    {
        runnableData->ownerZFThread->_ZFP_ZFThread_threadOnCancel(
            runnableData->listenerData.param0(), runnableData->listenerData.param1());
    }
    runnableData->semaWait->observerNotifyWithCustomSender(
        (runnableData->ownerZFThread == zfnull) ? ZFThread::currentThread() : runnableData->ownerZFThread,
        ZFThread::EventThreadOnCancel(),
        runnableData->listenerData.param0(), runnableData->listenerData.param1());

    _ZFP_ZFThreadRunnableCleanup(runnableData);
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFThreadExecuteCancel,
                       ZFMP_IN(zfidentity, taskId))
{
    if(taskId != zfidentityInvalid())
    {
        zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
        if(lockAvailable)
        {
            zfsynchronizeLock(_ZFP_ZFThread_mutex);
        }
        for(zfindex i = 0; i < _ZFP_ZFThread_runnableList.count(); ++i)
        {
            _ZFP_I_ZFThreadRunnableData *runnableData = _ZFP_ZFThread_runnableList.get(i);
            if(runnableData->taskId == taskId)
            {
                _ZFP_ZFThreadDoCancelTask(runnableData);
                break;
            }
        }
        if(lockAvailable)
        {
            zfsynchronizeUnlock(_ZFP_ZFThread_mutex);
        }
    }
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFThreadExecuteCancel,
                       ZFMP_IN(const ZFListener &, runnable))
{
    if(runnable.callbackIsValid())
    {
        zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
        if(lockAvailable)
        {
            zfsynchronizeLock(_ZFP_ZFThread_mutex);
        }
        for(zfindex i = 0; i < _ZFP_ZFThread_runnableList.count(); ++i)
        {
            _ZFP_I_ZFThreadRunnableData *runnableData = _ZFP_ZFThread_runnableList.get(i);
            if(runnableData->ownerZFThread->threadRunnable().objectCompare(runnable) == ZFCompareTheSame)
            {
                _ZFP_ZFThreadDoCancelTask(runnableData);
            }
        }
        if(lockAvailable)
        {
            zfsynchronizeUnlock(_ZFP_ZFThread_mutex);
        }
    }
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFThreadExecuteCancelByOwner,
                       ZFMP_IN(ZFObject *, owner))
{
    if(owner != zfnull)
    {
        zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
        if(lockAvailable)
        {
            zfsynchronizeLock(_ZFP_ZFThread_mutex);
        }
        for(zfindex i = 0; i < _ZFP_ZFThread_runnableList.count(); ++i)
        {
            _ZFP_I_ZFThreadRunnableData *runnableData = _ZFP_ZFThread_runnableList.get(i);
            if(runnableData->owner == owner)
            {
                _ZFP_ZFThreadDoCancelTask(runnableData);
            }
        }
        if(lockAvailable)
        {
            zfsynchronizeUnlock(_ZFP_ZFThread_mutex);
        }
    }
}

ZFMETHOD_FUNC_DEFINE_1(void, ZFThreadExecuteWait,
                       ZFMP_IN(zfidentity, taskId))
{
    if(taskId != zfidentityInvalid())
    {
        zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
        if(lockAvailable)
        {
            zfsynchronizeLock(_ZFP_ZFThread_mutex);
        }
        for(zfindex i = 0; i < _ZFP_ZFThread_runnableList.count(); ++i)
        {
            _ZFP_I_ZFThreadRunnableData *runnableData = _ZFP_ZFThread_runnableList.get(i);
            if(runnableData->taskId == taskId)
            {
                if(runnableData->ownerZFThread != zfnull && !runnableData->ownerZFThread->isMainThread())
                {
                    if(lockAvailable)
                    {
                        zfsynchronizeUnlock(_ZFP_ZFThread_mutex);
                    }
                    runnableData->semaWait->semaphoreWaitLocked();
                    return ;
                }
                else
                {
                    if(lockAvailable)
                    {
                        zfsynchronizeUnlock(_ZFP_ZFThread_mutex);
                    }
                    return ;
                }
            }
        }
        if(lockAvailable)
        {
            zfsynchronizeUnlock(_ZFP_ZFThread_mutex);
        }
    }
}
ZFMETHOD_FUNC_DEFINE_2(zfbool, ZFThreadExecuteWait,
                       ZFMP_IN(zfidentity, taskId),
                       ZFMP_IN(zftimet, miliSecs))
{
    if(taskId != zfidentityInvalid())
    {
        zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
        if(lockAvailable)
        {
            zfsynchronizeLock(_ZFP_ZFThread_mutex);
        }
        for(zfindex i = 0; i < _ZFP_ZFThread_runnableList.count(); ++i)
        {
            _ZFP_I_ZFThreadRunnableData *runnableData = _ZFP_ZFThread_runnableList.get(i);
            if(runnableData->taskId == taskId)
            {
                if(runnableData->ownerZFThread != zfnull && !runnableData->ownerZFThread->isMainThread())
                {
                    if(lockAvailable)
                    {
                        zfsynchronizeUnlock(_ZFP_ZFThread_mutex);
                    }
                    return runnableData->semaWait->semaphoreWaitLocked(miliSecs);
                }
                else
                {
                    if(lockAvailable)
                    {
                        zfsynchronizeUnlock(_ZFP_ZFThread_mutex);
                    }
                    return zffalse;
                }
            }
        }
        if(lockAvailable)
        {
            zfsynchronizeUnlock(_ZFP_ZFThread_mutex);
        }
    }
    return zffalse;
}

ZFMETHOD_FUNC_DEFINE_3(void, ZFThreadExecuteObserverAdd,
                       ZFMP_IN(zfidentity, taskId),
                       ZFMP_IN(zfidentity, eventId),
                       ZFMP_IN(const ZFListener &, callback))
{
    if(eventId != ZFThread::EventThreadOnStart()
        && eventId != ZFThread::EventThreadOnStop()
        && eventId != ZFThread::EventThreadOnCancel())
    {
        zfCoreCriticalMessage("thread task can only add ZFThread's observer event, event: %s",
            ZFIdMapNameForId(eventId));
        return ;
    }
    if(taskId != zfidentityInvalid())
    {
        zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
        if(lockAvailable)
        {
            zfsynchronizeLock(_ZFP_ZFThread_mutex);
        }
        for(zfindex i = 0; i < _ZFP_ZFThread_runnableList.count(); ++i)
        {
            _ZFP_I_ZFThreadRunnableData *runnableData = _ZFP_ZFThread_runnableList.get(i);
            if(runnableData->taskId == taskId)
            {
                runnableData->semaWait->observerAdd(eventId, callback);
                break;
            }
        }
        if(lockAvailable)
        {
            zfsynchronizeUnlock(_ZFP_ZFThread_mutex);
        }
    }
}
ZFMETHOD_FUNC_DEFINE_3(void, ZFThreadExecuteObserverRemove,
                       ZFMP_IN(zfidentity, taskId),
                       ZFMP_IN(zfidentity, eventId),
                       ZFMP_IN(const ZFListener &, callback))
{
    if(eventId != ZFThread::EventThreadOnStart()
        && eventId != ZFThread::EventThreadOnStop()
        && eventId != ZFThread::EventThreadOnCancel())
    {
        zfCoreCriticalMessage("thread task can only add ZFThread's observer event, event: %s",
            ZFIdMapNameForId(eventId));
        return ;
    }
    if(taskId != zfidentityInvalid())
    {
        zfbool lockAvailable = (_ZFP_ZFThread_mutex != zfnull);
        if(lockAvailable)
        {
            zfsynchronizeLock(_ZFP_ZFThread_mutex);
        }
        for(zfindex i = 0; i < _ZFP_ZFThread_runnableList.count(); ++i)
        {
            _ZFP_I_ZFThreadRunnableData *runnableData = _ZFP_ZFThread_runnableList.get(i);
            if(runnableData->taskId == taskId)
            {
                runnableData->semaWait->observerRemove(eventId, callback);
                break;
            }
        }
        if(lockAvailable)
        {
            zfsynchronizeUnlock(_ZFP_ZFThread_mutex);
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

