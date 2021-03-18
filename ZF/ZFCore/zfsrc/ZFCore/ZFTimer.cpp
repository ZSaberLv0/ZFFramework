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
ZFTYPEID_ACCESS_ONLY_DEFINE(ZFTimerExecuteParam, ZFTimerExecuteParam)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFTimerExecuteParam, zftimet, timerInterval)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFTimerExecuteParam, zfbool, timerActivateInMainThread)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFTimerExecuteParam, ZFObject *, timerParam0)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFTimerExecuteParam, ZFObject *, timerParam1)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFTimerExecuteParam, ZFObject *, userData)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFTimerExecuteParam, ZFListener, timerCallback)
ZFMETHOD_USER_REGISTER_FOR_WRAPPER_SETTER_GETTER(v_ZFTimerExecuteParam, zfindex, timerActivateCountMax)

ZFMETHOD_FUNC_DEFINE_1(zfautoObject, ZFTimerExecute,
                       ZFMP_IN(const ZFTimerExecuteParam &, param))
{
    if(param.timerInterval() <= 0 || !param.timerCallback().callbackIsValid())
    {
        return zfnull;
    }
    zfblockedAlloc(ZFTimer, timer);
    timer->timerInterval(param.timerInterval());
    timer->timerDelay(param.timerDelay());
    timer->timerActivateInMainThread(param.timerActivateInMainThread());
    timer->timerParam0(param.timerParam0());
    timer->timerParam1(param.timerParam1());
    timer->observerAdd(ZFTimer::EventTimerOnActivate(), param.timerCallback(), param.userData());
    if(param.timerActivateCountMax() > 0)
    {
        ZFLISTENER_LOCAL(timerOnActivate, {
            zfindex timerActivatedCountMax = userData->to<v_zfindex *>()->zfv;
            ZFTimer *timer = listenerData.sender<ZFTimer *>();
            if(timer->timerActivatedCount() > timerActivatedCountMax)
            {
                timer->timerStop();
            }
        })
        timer->observerAdd(ZFTimer::EventTimerOnActivate(),
            timerOnActivate,
            zflineAlloc(v_zfindex, param.timerActivateCountMax()));
    }
    timer->timerStart();
    return timer;
}

ZF_NAMESPACE_GLOBAL_END

