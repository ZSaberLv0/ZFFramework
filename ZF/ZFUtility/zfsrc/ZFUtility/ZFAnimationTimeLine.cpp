#include "ZFAnimationTimeLine.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// _ZFP_ZFAnimationTimeLinePrivate
zfclass _ZFP_ZFAnimationTimeLinePrivate : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_ZFAnimationTimeLinePrivate, ZFObject)

public:
    ZFAnimationTimeLine *pimplOwner;
    ZFTimer *timer;
    zftimet timeStart;

    ZFLISTENER_INLINE(timerOnEvent)
    {
        zffloat progress = ((zffloat)(ZFTime::timestamp() - this->timeStart)) / this->pimplOwner->aniDurationFixed();
        if(progress < 0)
        {
            progress = 0;
        }
        else if(progress > 1)
        {
            progress = 1;
        }
        this->update(progress);
    }
public:
    void update(ZF_IN zffloat progress)
    {
        if(this->pimplOwner->aniCurve() != zfnull)
        {
            progress = this->pimplOwner->aniCurve()->progressUpdate(progress);
        }
        this->pimplOwner->aniTimeLineOnUpdate(progress);
        if(ZFTime::timestamp() - this->timeStart >= this->pimplOwner->aniDurationFixed())
        {
            this->timer->timerStop();
            this->pimplOwner->aniImplNotifyStop();
        }
    }

protected:
    zfoverride
    virtual void objectOnInit(void)
    {
        zfsuper::objectOnInit();
        this->timer = zfAlloc(ZFTimer);
        this->timer->observerAdd(ZFTimer::EventTimerOnActivate(),
            ZFCallbackForMemberMethod(this, ZFMethodAccess(zfself, timerOnEvent)));
    }
    zfoverride
    virtual void objectOnDealloc(void)
    {
        zfRelease(this->timer);
        zfsuper::objectOnDealloc();
    }
};

// ============================================================
ZFOBJECT_REGISTER(ZFAnimationTimeLine)

ZFOBSERVER_EVENT_REGISTER(ZFAnimationTimeLine, AniTimeLineOnUpdate)

// ============================================================
// object
void ZFAnimationTimeLine::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfAlloc(_ZFP_ZFAnimationTimeLinePrivate);
    d->pimplOwner = this;
}
void ZFAnimationTimeLine::objectOnDealloc(void)
{
    zfRelease(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

zfidentity ZFAnimationTimeLine::objectHash(void)
{
    return zfidentityHash(zfsuper::objectHash()
        , this->aniTimeLineInterval()
        );
}
ZFCompareResult ZFAnimationTimeLine::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(zfsuper::objectCompare(anotherObj) != ZFCompareTheSame)
    {
        return ZFCompareUncomparable;
    }
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(this->aniTimeLineInterval() == another->aniTimeLineInterval())
    {
        return ZFCompareTheSame;
    }
    return ZFCompareUncomparable;
}

// ============================================================
// start stop
void ZFAnimationTimeLine::aniImplStart(void)
{
    zfsuper::aniImplStart();
    d->timeStart = ZFTime::timestamp();
    if(this->aniDurationFixed() < this->aniTimeLineInterval())
    {
        d->timer->timerInterval(this->aniDurationFixed());
    }
    else
    {
        d->timer->timerInterval(this->aniTimeLineInterval());
    }

    d->timer->timerActivateInMainThread(this->aniTimeLineNotifyInMainThread());
    d->timer->timerStart();
    d->update(0);
}
void ZFAnimationTimeLine::aniImplStop(void)
{
    d->timer->timerStop();
    zfsuper::aniImplStop();
}

void ZFAnimationTimeLine::aniTimeLineOnUpdate(ZF_IN zffloat progress)
{
    this->observerNotify(
        ZFAnimationTimeLine::EventAniTimeLineOnUpdate(),
        zflineAlloc(v_zffloat, progress));
}

ZF_NAMESPACE_GLOBAL_END

