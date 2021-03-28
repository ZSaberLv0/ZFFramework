#include "ZFTimer.h"
#include "protocol/ZFProtocolZFTimer.h"

#include "ZFThread.h" // for timer thread register

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// _ZFP_ZFTimerPrivate
zfclassNotPOD _ZFP_ZFTimerPrivate
{
public:
    void *nativeTimer;
    zfbool timerStarted;
    zfindex timerActivatedCount;
    void *timerThreadToken;

public:
    _ZFP_ZFTimerPrivate(void)
    : nativeTimer(zfnull)
    , timerStarted(zffalse)
    , timerActivatedCount(0)
    , timerThreadToken(zfnull)
    {
    }
};

// ============================================================
// ZFTimer
ZFOBJECT_REGISTER(ZFTimer)

ZFOBSERVER_EVENT_REGISTER(ZFTimer, TimerOnStart)
ZFOBSERVER_EVENT_REGISTER(ZFTimer, TimerOnActivate)
ZFOBSERVER_EVENT_REGISTER(ZFTimer, TimerOnStop)

ZFOBJECT_ON_INIT_DEFINE_5(ZFTimer, ZFMP_IN(zftimet, timerInterval),
                          ZFMP_IN_OPT(ZFObject *, timerParam0, zfnull),
                          ZFMP_IN_OPT(ZFObject *, timerParam1, zfnull),
                          ZFMP_IN_OPT(zftimet, timerDelay, zftimetZero()),
                          ZFMP_IN_OPT(zfbool, timerActivateInMainThread, zftrue))
{
    this->objectOnInit();
    zfself::timerInterval(timerInterval);
    zfself::timerDelay(timerDelay);
    zfself::timerParam0(timerParam0);
    zfself::timerParam1(timerParam1);
    zfself::timerActivateInMainThread(timerActivateInMainThread);
}
void ZFTimer::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFTimerPrivate);
    d->nativeTimer = ZFPROTOCOL_ACCESS(ZFTimer)->nativeTimerCreate(this);
}
void ZFTimer::objectOnDealloc(void)
{
    ZFPROTOCOL_ACCESS(ZFTimer)->nativeTimerDestroy(this, d->nativeTimer);
    d->nativeTimer = zfnull;
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}
void ZFTimer::objectOnDeallocPrepare(void)
{
    this->timerStop();
    zfsuper::objectOnDeallocPrepare();
}

ZFMETHOD_DEFINE_0(ZFTimer, void *, nativeTimer)
{
    return d->nativeTimer;
}

ZFPROPERTY_OVERRIDE_ON_VERIFY_DEFINE(ZFTimer, zftimet, timerInterval)
{
    zfCoreAssert(!this->timerStarted());
    zfCoreAssert(this->timerInterval() > 0);
}
ZFPROPERTY_OVERRIDE_ON_VERIFY_DEFINE(ZFTimer, zftimet, timerDelay)
{
    zfCoreAssert(!this->timerStarted());
    zfCoreAssert(this->timerDelay() >= 0);
}
ZFPROPERTY_OVERRIDE_ON_VERIFY_DEFINE(ZFTimer, zfbool, timerActivateInMainThread)
{
    zfCoreAssert(!this->timerStarted());
}
ZFPROPERTY_OVERRIDE_ON_VERIFY_DEFINE(ZFTimer, ZFObject *, timerParam0)
{
    zfCoreAssert(!this->timerStarted());
}
ZFPROPERTY_OVERRIDE_ON_VERIFY_DEFINE(ZFTimer, ZFObject *, timerParam1)
{
    zfCoreAssert(!this->timerStarted());
}

ZFMETHOD_DEFINE_0(ZFTimer, void, timerStart)
{
    if(d->timerStarted)
    {
        return ;
    }
    d->timerStarted = zftrue;

    zfRetain(this);

    d->timerActivatedCount = 0;
    ZFPROTOCOL_ACCESS(ZFTimer)->timerStart(this);
}
ZFMETHOD_DEFINE_0(ZFTimer, void, timerStop)
{
    if(d->timerStarted)
    {
        d->timerStarted = zffalse;
        ZFPROTOCOL_ACCESS(ZFTimer)->timerStop(this);
    }
}

ZFMETHOD_DEFINE_0(ZFTimer, zfbool, timerStarted)
{
    return d->timerStarted;
}

ZFMETHOD_DEFINE_0(ZFTimer, zfindex, timerActivatedCount)
{
    return d->timerActivatedCount;
}

void ZFTimer::_ZFP_ZFTimer_timerOnStart(void)
{
    if(ZFThread::currentThread() == zfnull)
    {
        d->timerThreadToken = ZFThread::nativeThreadRegister();
    }
    this->timerOnStart();
}
void ZFTimer::_ZFP_ZFTimer_timerOnActivate(void)
{
    zfRetain(this);
    {
        ++(d->timerActivatedCount);
        this->timerOnActivate();
    }
    zfRelease(this);
}
void ZFTimer::_ZFP_ZFTimer_timerOnStop(void)
{
    this->timerOnStop();
    if(d->timerThreadToken != zfnull)
    {
        ZFThread::nativeThreadUnregister(d->timerThreadToken);
        d->timerThreadToken = zfnull;
    }
    zfRelease(this);
}

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFGlobalTimerDataHolder, ZFLevelZFFrameworkHigh)
{
    this->globalTimer = zfnull;
    this->globalTimerIntervalDefault = 33;
    this->globalTimerInterval = this->globalTimerIntervalDefault;
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFGlobalTimerDataHolder)
{
    if(this->globalTimer != zfnull)
    {
        this->globalTimer->timerStop();
        zfRetainChange(this->globalTimer, zfnull);
    }
}
zftimet globalTimerIntervalDefault;
zftimet globalTimerInterval;
ZFTimer *globalTimer;
void checkCleanup(void)
{
    if(!this->globalTimer->observerHasAdd(ZFTimer::EventTimerOnActivate()))
    {
        this->globalTimer->timerStop();
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFGlobalTimerDataHolder)

ZFMETHOD_FUNC_DEFINE_5(zfidentity, ZFGlobalTimerAttach,
                       ZFMP_IN(const ZFListener &, timerCallback),
                       ZFMP_IN_OPT(ZFObject *, timerCallbackUserData, zfnull),
                       ZFMP_IN_OPT(ZFObject *, owner, zfnull),
                       ZFMP_IN_OPT(zfbool, autoRemoveAfterActivate, zffalse),
                       ZFMP_IN_OPT(ZFLevel, observerLevel, ZFLevelAppNormal))
{
    if(ZFFrameworkStateCheck(ZFLevelZFFrameworkHigh) != ZFFrameworkStateAvailable || !timerCallback.callbackIsValid())
    {
        return zfidentityInvalid();
    }
    zfCoreMutexLocker();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFGlobalTimerDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFGlobalTimerDataHolder);
    if(d->globalTimer == zfnull)
    {
        d->globalTimer = zfAlloc(ZFTimer);
        d->globalTimer->timerInterval(ZFGlobalTimerInterval());
    }
    if(!d->globalTimer->timerStarted())
    {
        d->globalTimer->timerInterval(ZFGlobalTimerInterval());
        d->globalTimer->timerStart();
    }
    return d->globalTimer->observerAdd(ZFTimer::EventTimerOnActivate(), timerCallback, timerCallbackUserData, owner, autoRemoveAfterActivate, observerLevel);
}
ZFMETHOD_FUNC_DEFINE_2(void, ZFGlobalTimerDetach,
                       ZFMP_IN(const ZFListener &, timerCallback),
                       ZFMP_IN_OPT(ZFObject *, timerCallbackUserData, zfnull))
{
    zfCoreMutexLocker();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFGlobalTimerDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFGlobalTimerDataHolder);
    if(d->globalTimer == zfnull)
    {
        return;
    }
    d->globalTimer->observerRemove(ZFTimer::EventTimerOnActivate(), timerCallback, timerCallbackUserData);
    d->checkCleanup();
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFGlobalTimerDetachByTaskId,
                       ZFMP_IN(zfidentity, taskId))
{
    zfCoreMutexLocker();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFGlobalTimerDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFGlobalTimerDataHolder);
    if(d->globalTimer == zfnull)
    {
        return;
    }
    d->globalTimer->observerRemoveByTaskId(taskId);
    d->checkCleanup();
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFGlobalTimerDetachByOwner,
                       ZFMP_IN(ZFObject *, owner))
{
    zfCoreMutexLocker();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFGlobalTimerDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFGlobalTimerDataHolder);
    if(d->globalTimer == zfnull)
    {
        return;
    }
    d->globalTimer->observerRemoveByOwner(owner);
    d->checkCleanup();
}
ZFMETHOD_FUNC_DEFINE_0(void, ZFGlobalTimerDetachAll)
{
    zfCoreMutexLocker();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFGlobalTimerDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFGlobalTimerDataHolder);
    if(d->globalTimer == zfnull)
    {
        return;
    }
    d->globalTimer->observerRemoveAll(ZFTimer::EventTimerOnActivate());
    d->checkCleanup();
}

ZFMETHOD_FUNC_DEFINE_0(zftimet const &, ZFGlobalTimerIntervalDefault)
{
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFGlobalTimerDataHolder)->globalTimerIntervalDefault;
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFGlobalTimerIntervalDefault,
                       ZFMP_IN(zftimet const &, timerIntervalDefault))
{
    zfCoreAssert(timerIntervalDefault > 0);
    ZF_GLOBAL_INITIALIZER_INSTANCE(ZFGlobalTimerDataHolder)->globalTimerIntervalDefault = timerIntervalDefault;
}
ZFMETHOD_FUNC_DEFINE_0(zftimet const &, ZFGlobalTimerInterval)
{
    return ZF_GLOBAL_INITIALIZER_INSTANCE(ZFGlobalTimerDataHolder)->globalTimerInterval;
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFGlobalTimerInterval,
                       ZFMP_IN(zftimet const &, timerInterval))
{
    zfCoreAssert(timerInterval > 0);
    zfCoreMutexLocker();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFGlobalTimerDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFGlobalTimerDataHolder);
    if(d->globalTimerInterval != timerInterval)
    {
        d->globalTimerInterval = timerInterval;
        if(d->globalTimer != zfnull && d->globalTimer->timerStarted())
        {
            d->globalTimer->timerStop();
            d->globalTimer->timerInterval(d->globalTimerInterval);
            d->globalTimer->timerStart();
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

