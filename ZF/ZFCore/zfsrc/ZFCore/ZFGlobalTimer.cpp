#include "ZFGlobalTimer.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFGlobalTimerDataHolder, ZFLevelZFFrameworkHigh)
{
    this->globalTimer = zfnull;
    this->globalTimerIntervalDefault = 33; // 1000/30, 30fps by default
    this->globalTimerInterval = this->globalTimerIntervalDefault;
    this->globalTimerManualStep = zffalse;
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
zfbool globalTimerManualStep;
void checkCleanup(void)
{
    if(!this->globalTimer->observerHasAdd(ZFTimer::EventTimerOnActivate()))
    {
        this->globalTimer->timerStop();
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFGlobalTimerDataHolder)

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFGlobalTimerAutoDetach, ZFLevelZFFrameworkLow)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFGlobalTimerAutoDetach)
{
    ZFGlobalTimerDetachAll();
}
ZF_GLOBAL_INITIALIZER_END(ZFGlobalTimerAutoDetach)

// ============================================================
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
    if(!d->globalTimer->timerStarted() && !d->globalTimerManualStep)
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

// ============================================================
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

// ============================================================
ZFMETHOD_FUNC_DEFINE_0(void, ZFGlobalTimerManualStep)
{
    zfCoreMutexLocker();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFGlobalTimerDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFGlobalTimerDataHolder);
    d->globalTimerManualStep = zftrue;
    if(d->globalTimer != zfnull)
    {
        d->globalTimer->timerStop();
        d->globalTimer->observerNotify(ZFTimer::EventTimerOnActivate(), d->globalTimer->timerParam0(), d->globalTimer->timerParam1());
    }
}
ZFMETHOD_FUNC_DEFINE_0(void, ZFGlobalTimerManualStepCancel)
{
    zfCoreMutexLocker();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFGlobalTimerDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFGlobalTimerDataHolder);
    if(!d->globalTimerManualStep)
    {
        return;
    }
    d->globalTimerManualStep = zffalse;
    if(d->globalTimer != zfnull)
    {
        if(d->globalTimer->observerHasAdd(ZFTimer::EventTimerOnActivate()))
        {
            d->globalTimer->timerInterval(ZFGlobalTimerInterval());
            d->globalTimer->timerStart();
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

