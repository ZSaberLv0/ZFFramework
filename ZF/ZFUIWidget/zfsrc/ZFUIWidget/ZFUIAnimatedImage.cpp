#include "ZFUIAnimatedImage.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFOBJECT_REGISTER(ZFUIAnimatedImageAniFrame)

// ============================================================
ZFSTYLE_DEFAULT_DEFINE(ZFUIAnimatedImage)

// ============================================================
// _ZFP_ZFUIAnimatedImagePrivate
zfclassNotPOD _ZFP_ZFUIAnimatedImagePrivate
{
public:
    ZFUIAnimatedImage *pimplOwner;
    zfbool aniRunning;
    zfindex aniFrameIndexLast;
    zfindex aniFrameIndexCurrent;
    zfindex aniRepeatCountCurrent;
    zfidentity aniFrameTaskId;
    ZFListener aniFramesOnChangeListener;
    ZFListener aniFrameDurationOnFinishListener;

public:
    _ZFP_ZFUIAnimatedImagePrivate(void)
    : pimplOwner(zfnull)
    , aniRunning(zffalse)
    , aniFrameIndexLast(zfindexMax())
    , aniFrameIndexCurrent(zfindexMax())
    , aniRepeatCountCurrent(0)
    , aniFrameTaskId(zfidentityInvalid())
    , aniFramesOnChangeListener(ZFCallbackForFunc(_ZFP_ZFUIAnimatedImagePrivate::aniFramesOnChange))
    , aniFrameDurationOnFinishListener(ZFCallbackForFunc(_ZFP_ZFUIAnimatedImagePrivate::aniFrameDurationOnFinish))
    {
    }

public:
    zfindex aniIndexNext(ZF_IN zfindex index)
    {
        return ((index + 1) % this->pimplOwner->aniFrames()->count());
    }
    void aniRunNext(void)
    {
        zfblockedRelease(zfRetain(this->pimplOwner));

        zfbool loopFinish = zffalse;
        if(this->aniFrameIndexLast != zfindexMax()
            && this->aniFrameIndexCurrent == this->pimplOwner->aniFrames()->count() - 1)
        {
            loopFinish = zftrue;
        }

        do
        {
            this->aniFrameIndexLast = this->aniFrameIndexCurrent;
            this->aniFrameIndexCurrent = this->aniIndexNext(this->aniFrameIndexCurrent);
            this->pimplOwner->aniFrameOnUpdate();
            if(!this->aniRunning)
            {
                return ;
            }
        } while(this->pimplOwner->aniFrameDurationCurrent() == 0);

        if(loopFinish)
        {
            if(this->aniRepeatCountCurrent < this->pimplOwner->aniRepeatCount())
            {
                ++this->aniRepeatCountCurrent;
                this->pimplOwner->aniOnRepeat();
                if(!this->aniRunning)
                {
                    return ;
                }
            }
            else
            {
                this->pimplOwner->aniStop();
                return ;
            }
        }

        this->pimplOwner->aniOnUpdate();
        if(!this->aniRunning)
        {
            return ;
        }
        this->aniFrameTaskId = ZFThreadExecuteInMainThreadAfterDelay(
            this->pimplOwner->aniFrameDurationFixedCurrent(),
            this->aniFrameDurationOnFinishListener,
            this->pimplOwner->objectHolder());
    }

public:
    static ZFLISTENER_PROTOTYPE_EXPAND(aniFramesOnChange)
    {
        ZFUIAnimatedImage *pimplOwner = userData->objectHolded();
        if(pimplOwner->aniRunning())
        {
            zfCoreCriticalMessageTrim("[ZFUIAnimatedImage] you must not modify aniFrame while animating");
        }
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(aniFrameDurationOnFinish)
    {
        ZFUIAnimatedImage *pimplOwner = userData->objectHolded();
        pimplOwner->d->aniRunNext();
    }
};

// ============================================================
// ZFUIAnimatedImage
ZFOBJECT_REGISTER(ZFUIAnimatedImage)

ZFOBSERVER_EVENT_REGISTER(ZFUIAnimatedImage, AniOnStart)
ZFOBSERVER_EVENT_REGISTER(ZFUIAnimatedImage, AniOnStop)
ZFOBSERVER_EVENT_REGISTER(ZFUIAnimatedImage, AniOnRepeat)
ZFOBSERVER_EVENT_REGISTER(ZFUIAnimatedImage, AniOnUpdate)
ZFOBSERVER_EVENT_REGISTER(ZFUIAnimatedImage, AniFrameOnUpdate)

ZFMETHOD_DEFINE_0(ZFUIAnimatedImage, void, aniStart)
{
    if(d->aniRunning || this->aniFrameCount() == 0)
    {
        return ;
    }
    d->aniRunning = zftrue;
    this->aniFrames()->observerAdd(ZFContainer::EventContentOnChange(), d->aniFramesOnChangeListener, this->objectHolder());
    zfRetain(this);

    d->aniRepeatCountCurrent = 0;
    if(!this->aniKeepPreviousState())
    {
        d->aniFrameIndexCurrent = zfindexMax();
    }

    d->aniRunNext();
}
ZFMETHOD_DEFINE_0(ZFUIAnimatedImage, void, aniStop)
{
    if(!d->aniRunning)
    {
        return ;
    }
    d->aniRunning = zffalse;
    this->aniFrames()->observerRemove(ZFContainer::EventContentOnChange(), d->aniFramesOnChangeListener);
    d->aniFrameIndexLast = zfindexMax();

    zfidentity aniFrameTaskIdTmp = d->aniFrameTaskId;
    d->aniFrameTaskId = zfidentityInvalid();
    ZFThreadExecuteCancel(aniFrameTaskIdTmp);

    zfRelease(this);
}
ZFMETHOD_DEFINE_0(ZFUIAnimatedImage, zfbool, aniRunning)
{
    return d->aniRunning;
}

ZFMETHOD_DEFINE_1(ZFUIAnimatedImage, void, aniManual,
                  ZFMP_IN(zfindex, aniFrame))
{
    this->aniStop();

    d->aniFrameIndexLast = d->aniFrameIndexCurrent;
    if(aniFrame < this->aniFrames()->count())
    {
        d->aniFrameIndexCurrent = aniFrame;
    }
    else
    {
        d->aniFrameIndexCurrent = 0;
    }
    if(d->aniFrameIndexCurrent >= this->aniFrames()->count())
    {
        d->aniFrameIndexCurrent = zfindexMax();
    }
    this->aniFrameOnUpdate();
    this->aniOnUpdate();
}

ZFMETHOD_DEFINE_0(ZFUIAnimatedImage, ZFUIImage *, aniFrameImageCurrent)
{
    if(d->aniFrameIndexCurrent < this->aniFrames()->count())
    {
        return this->aniFrameImageAtIndex(d->aniFrameIndexCurrent);
    }
    else
    {
        return zfnull;
    }
}
ZFMETHOD_DEFINE_0(ZFUIAnimatedImage, zftimet, aniFrameDurationCurrent)
{
    if(d->aniFrameIndexCurrent < this->aniFrames()->count())
    {
        return this->aniFrameDurationAtIndex(d->aniFrameIndexCurrent);
    }
    else
    {
        return zftimetZero();
    }
}
ZFMETHOD_DEFINE_0(ZFUIAnimatedImage, zftimet, aniFrameDurationFixedCurrent)
{
    if(d->aniFrameIndexCurrent < this->aniFrames()->count())
    {
        return this->aniFrameDurationFixedAtIndex(d->aniFrameIndexCurrent);
    }
    else
    {
        return zftimetZero();
    }
}
ZFMETHOD_DEFINE_0(ZFUIAnimatedImage, ZFUIAnimatedImageAniFrame *, aniFrameCurrent)
{
    if(d->aniFrameIndexCurrent < this->aniFrames()->count())
    {
        return this->aniFrameAtIndex(d->aniFrameIndexCurrent);
    }
    else
    {
        return zfnull;
    }
}
ZFMETHOD_DEFINE_0(ZFUIAnimatedImage, zfindex, aniFrameIndexCurrent)
{
    if(d->aniFrameIndexCurrent < this->aniFrames()->count())
    {
        return d->aniFrameIndexCurrent;
    }
    else
    {
        return zfindexMax();
    }
}

ZFMETHOD_DEFINE_0(ZFUIAnimatedImage, zfindex, aniRepeatCountCurrent)
{
    return d->aniRepeatCountCurrent;
}

void ZFUIAnimatedImage::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIAnimatedImagePrivate);
    d->pimplOwner = this;
}
void ZFUIAnimatedImage::objectOnDealloc(void)
{
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

void ZFUIAnimatedImage::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);

    zfstringAppend(ret, " %zi of %zi", this->aniFrameIndexCurrent(), this->aniFrameCount());
    if(this->aniFrameCount() > 0)
    {
        for(zfindex i = 0; i < this->aniFrameCount(); ++i)
        {
            if(i == 0)
            {
                ret += " frames:";
            }
            zfstringAppend(ret, " %s", zfsFromInt(this->aniFrameDurationFixedAtIndex(i)).cString());
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

