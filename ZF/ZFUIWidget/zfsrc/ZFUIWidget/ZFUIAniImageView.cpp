#include "ZFUIAniImageView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassNotPOD _ZFP_ZFUIAniImageViewPrivate
{
public:
    zfautoObjectT<ZFUIImage *> frameSrc;
    ZFCoreArray<zfautoObjectT<ZFUIImage *> > frameImages;
    ZFUISize frameSizePixel;
    zfindex frameCount;
    ZFCoreArrayPOD<zftimet> frameDurations;

    enum {
        stateFlag_aniStarted = 1 << 0,
        stateFlag_observerHasAddFlag_aniOnStart = 1 << 1,
        stateFlag_observerHasAddFlag_aniOnLoop = 1 << 2,
        stateFlag_observerHasAddFlag_aniOnStop = 1 << 3,
        stateFlag_observerHasAddFlag_aniOnFrame = 1 << 4,
    };
    zfuint stateFlag;
    zfindex aniFrame;
    ZFCoreArrayPOD<zfuint> frameTimers;
    zfindex frameTimerToNext;
    zfindex aniCount;
    ZFListener onTimerListener;

public:
    _ZFP_ZFUIAniImageViewPrivate(void)
    : frameSrc()
    , frameImages()
    , frameSizePixel(ZFUISizeZero())
    , frameCount(0)
    , frameDurations()
    , stateFlag(0)
    , aniFrame(zfindexMax())
    , frameTimers()
    , frameTimerToNext(0)
    , aniCount(0)
    , onTimerListener(ZFCallbackForFunc(onTimer))
    {
    }

public:
    static ZFLISTENER_PROTOTYPE_EXPAND(onTimer)
    {
        ZFUIAniImageView *pimplOwner = userData->objectHolded();
        _ZFP_ZFUIAniImageViewPrivate *d = pimplOwner->d;
        --(d->frameTimerToNext);
        if(d->frameTimerToNext > 0)
        {
            return;
        }

        if(d->aniFrame == d->frameImages.count() - 1 && d->aniCount == 1)
        {
            pimplOwner->aniStop();
            return;
        }

        d->aniFrame = ((d->aniFrame + 1) % d->frameImages.count());
        zfbool isLoop = (d->aniFrame == 0);

        d->frameTimerToNext = d->frameTimers[d->aniFrame];
        if(isLoop && d->aniCount > 0)
        {
            --(d->aniCount);
        }
        pimplOwner->image(d->frameImages[d->aniFrame]);
        pimplOwner->aniOnFrame();
        if(isLoop)
        {
            pimplOwner->aniOnLoop();
        }
    }
};

// ============================================================
ZFOBJECT_REGISTER(ZFUIAniImageView)

ZFOBSERVER_EVENT_REGISTER(ZFUIAniImageView, AniOnStart)
ZFOBSERVER_EVENT_REGISTER(ZFUIAniImageView, AniOnLoop)
ZFOBSERVER_EVENT_REGISTER(ZFUIAniImageView, AniOnStop)
ZFOBSERVER_EVENT_REGISTER(ZFUIAniImageView, AniOnFrame)

ZFMETHOD_DEFINE_4(ZFUIAniImageView, zfbool, aniLoad,
                  ZFMP_IN(ZFUIImage *, src),
                  ZFMP_IN(const ZFUISize &, frameSizePixel),
                  ZFMP_IN(zfindex, frameCount),
                  ZFMP_IN_OPT(ZFCoreArrayPOD<zftimet> const &, frameDurations, ZFCoreArrayPOD<zftimet>()))
{
    if(src == zfnull
        || frameSizePixel.width <= 0 || frameSizePixel.height <= 0
        || frameCount <= 0
    ) {
        return zffalse;
    }
    const ZFUISize &imageSizeFixed = src->imageSizeFixed();
    if(imageSizeFixed.width <= 0 || imageSizeFixed.height <= 0)
    {
        return zffalse;
    }

    d->frameImages.removeAll();
    d->frameSizePixel = frameSizePixel;
    d->frameCount = frameCount;
    d->frameDurations.removeAll();

    d->frameTimers.removeAll();
    d->frameTimerToNext = 0;

    for(zfint y = 0, yEnd = imageSizeFixed.height - frameSizePixel.height; y <= yEnd && d->frameImages.count() < frameCount; y += frameSizePixel.height)
    {
        for(zfint x = 0, xEnd = imageSizeFixed.width - frameSizePixel.width; x <= xEnd && d->frameImages.count() < frameCount; x += frameSizePixel.width)
        {
            zfautoObjectT<ZFUIImage *> frameImage = ZFUIImageLoadInFrame(src, ZFUIRectMake(
                    x,
                    y,
                    frameSizePixel.width,
                    frameSizePixel.height
                ));
            if(frameImage == zfnull)
            {
                d->frameImages.removeAll();
                d->aniFrame = zfindexMax();
                return zffalse;
            }
            d->frameImages.add(frameImage);
        }
    }
    if(d->aniFrame >= d->frameImages.count())
    {
        d->aniFrame = 0;
    }

    frameCount = zfmMin(frameCount, d->frameImages.count());
    zfindex frameCountTmp = zfmMin(frameDurations.count(), frameCount);
    zftimet interval = ZFGlobalTimerIntervalDefault();
    for(zfindex i = 0; i < frameCountTmp; ++i)
    {
        d->frameDurations.add(frameDurations[i]);
        zfuint frameTimer = (zfuint)(frameDurations[i] / interval);
        d->frameTimers.add(frameTimer > 0 ? frameTimer : 1);
    }
    zfuint frameTimerLast = d->frameTimers.isEmpty() ? 1 : d->frameTimers.getLast();
    for(zfindex i = frameCountTmp; i < frameCount; ++i)
    {
        d->frameTimers.add(frameTimerLast);
    }

    this->image(d->frameImages[d->aniFrame]);
    this->aniOnFrame();

    return zftrue;
}

ZFMETHOD_DEFINE_0(ZFUIAniImageView, zfbool, aniStarted)
{
    return ZFBitTest(d->stateFlag, _ZFP_ZFUIAniImageViewPrivate::stateFlag_aniStarted);
}
ZFMETHOD_DEFINE_1(ZFUIAniImageView, void, aniStart,
                  ZFMP_IN_OPT(zfindex, aniCount, 1))
{
    this->aniStop();
    if(d->frameImages.isEmpty())
    {
        return;
    }
    ZFBitSet(d->stateFlag, _ZFP_ZFUIAniImageViewPrivate::stateFlag_aniStarted);
    zfRetain(this);

    ++(d->aniFrame);
    if(d->aniFrame >= d->frameImages.count())
    {
        d->aniFrame = 0;
    }
    d->frameTimerToNext = d->frameTimers[d->aniFrame];
    d->aniCount = aniCount;
    ZFGlobalTimerAttach(d->onTimerListener, this->objectHolder());

    this->image(d->frameImages[d->aniFrame]);
    this->aniOnStart();
    this->aniOnFrame();
}
ZFMETHOD_DEFINE_0(ZFUIAniImageView, void, aniStop)
{
    if(!ZFBitTest(d->stateFlag, _ZFP_ZFUIAniImageViewPrivate::stateFlag_aniStarted))
    {
        return;
    }
    ZFBitUnset(d->stateFlag, _ZFP_ZFUIAniImageViewPrivate::stateFlag_aniStarted);
    ZFGlobalTimerDetach(d->onTimerListener, this->objectHolder());
    this->aniOnStop();
    zfRelease(this);
}

ZFMETHOD_DEFINE_0(ZFUIAniImageView, zfindex const &, aniFrame)
{
    return d->aniFrame;
}
ZFMETHOD_DEFINE_1(ZFUIAniImageView, void, aniFrame,
                  ZFMP_IN(zfindex const &, aniFrame))
{
    this->aniStop();
    if(d->frameImages.isEmpty())
    {
        d->aniFrame = zfindexMax();
        return;
    }
    if(d->aniFrame == aniFrame)
    {
        return;
    }
    if(aniFrame >= d->frameImages.count())
    {
        d->aniFrame = 0;
    }
    else
    {
        d->aniFrame = aniFrame;
    }
    this->image(d->frameImages[d->aniFrame]);
    this->aniOnFrame();
}

ZFMETHOD_DEFINE_0(ZFUIAniImageView, void, aniFrameNext)
{
    if(d->aniFrame < d->frameImages.count() - 1)
    {
        this->aniFrame(d->aniFrame + 1);
    }
    else
    {
        this->aniFrame(0);
    }
}
ZFMETHOD_DEFINE_0(ZFUIAniImageView, void, aniFramePrev)
{
    if(d->aniFrame == 0)
    {
        this->aniFrame(d->frameImages.count() - 1);
    }
    else
    {
        this->aniFrame(d->aniFrame - 1);
    }
}

// ============================================================
void ZFUIAniImageView::aniOnStart(void)
{
    if(ZFBitTest(d->stateFlag, _ZFP_ZFUIAniImageViewPrivate::stateFlag_observerHasAddFlag_aniOnStart))
    {
        this->observerNotify(ZFUIAniImageView::EventAniOnStart());
    }
}
void ZFUIAniImageView::aniOnLoop(void)
{
    if(ZFBitTest(d->stateFlag, _ZFP_ZFUIAniImageViewPrivate::stateFlag_observerHasAddFlag_aniOnLoop))
    {
        this->observerNotify(ZFUIAniImageView::EventAniOnLoop());
    }
}
void ZFUIAniImageView::aniOnStop(void)
{
    if(ZFBitTest(d->stateFlag, _ZFP_ZFUIAniImageViewPrivate::stateFlag_observerHasAddFlag_aniOnStop))
    {
        this->observerNotify(ZFUIAniImageView::EventAniOnStop());
    }
}
void ZFUIAniImageView::aniOnFrame(void)
{
    if(ZFBitTest(d->stateFlag, _ZFP_ZFUIAniImageViewPrivate::stateFlag_observerHasAddFlag_aniOnFrame))
    {
        this->observerNotify(ZFUIAniImageView::EventAniOnFrame());
    }
}

void ZFUIAniImageView::observerOnAdd(ZF_IN zfidentity eventId)
{
    zfsuper::observerOnAdd(eventId);
    if(eventId == ZFUIAniImageView::EventAniOnStart())
    {
        ZFBitSet(d->stateFlag, _ZFP_ZFUIAniImageViewPrivate::stateFlag_observerHasAddFlag_aniOnStart);
    }
    else if(eventId == ZFUIAniImageView::EventAniOnLoop())
    {
        ZFBitSet(d->stateFlag, _ZFP_ZFUIAniImageViewPrivate::stateFlag_observerHasAddFlag_aniOnLoop);
    }
    else if(eventId == ZFUIAniImageView::EventAniOnStop())
    {
        ZFBitSet(d->stateFlag, _ZFP_ZFUIAniImageViewPrivate::stateFlag_observerHasAddFlag_aniOnStop);
    }
    else if(eventId == ZFUIAniImageView::EventAniOnFrame())
    {
        ZFBitSet(d->stateFlag, _ZFP_ZFUIAniImageViewPrivate::stateFlag_observerHasAddFlag_aniOnFrame);
    }
}
void ZFUIAniImageView::observerOnRemove(ZF_IN zfidentity eventId)
{
    zfsuper::observerOnRemove(eventId);
    if(eventId == ZFUIAniImageView::EventAniOnStart())
    {
        ZFBitUnset(d->stateFlag, _ZFP_ZFUIAniImageViewPrivate::stateFlag_observerHasAddFlag_aniOnStart);
    }
    else if(eventId == ZFUIAniImageView::EventAniOnLoop())
    {
        ZFBitUnset(d->stateFlag, _ZFP_ZFUIAniImageViewPrivate::stateFlag_observerHasAddFlag_aniOnLoop);
    }
    else if(eventId == ZFUIAniImageView::EventAniOnStop())
    {
        ZFBitUnset(d->stateFlag, _ZFP_ZFUIAniImageViewPrivate::stateFlag_observerHasAddFlag_aniOnStop);
    }
    else if(eventId == ZFUIAniImageView::EventAniOnFrame())
    {
        ZFBitUnset(d->stateFlag, _ZFP_ZFUIAniImageViewPrivate::stateFlag_observerHasAddFlag_aniOnFrame);
    }
}

// ============================================================
ZFMETHOD_DEFINE_0(ZFUIAniImageView, ZFCoreArray<zfautoObjectT<ZFUIImage *> > const &, frameImages)
{
    return d->frameImages;
}
ZFMETHOD_DEFINE_0(ZFUIAniImageView, ZFUISize const &, frameSizePixel)
{
    return d->frameSizePixel;
}
ZFMETHOD_DEFINE_0(ZFUIAniImageView, zfindex const &, frameCount)
{
    return d->frameCount;
}
ZFMETHOD_DEFINE_0(ZFUIAniImageView, ZFCoreArrayPOD<zftimet> const &, frameDurations)
{
    return d->frameDurations;
}

ZFOBJECT_ON_INIT_DEFINE_4(ZFUIAniImageView,
                          ZFMP_IN(ZFUIImage *, src),
                          ZFMP_IN(const ZFUISize &, frameSizePixel),
                          ZFMP_IN(zfindex, frameCount),
                          ZFMP_IN_OPT(ZFCoreArrayPOD<zftimet> const &, frameDurations, ZFCoreArrayPOD<zftimet>()))
{
    this->objectOnInit();
    this->aniLoad(src, frameSizePixel, frameCount, frameDurations);
}

void ZFUIAniImageView::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIAniImageViewPrivate);
}
void ZFUIAniImageView::objectOnDealloc(void)
{
    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}

void ZFUIAniImageView::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);
    if(this->frameCount() > 0)
    {
        ret += " ";
        ZFUISizeToString(ret, this->frameSizePixel());
        ret += ",";
        zfindexToString(ret, this->frameCount());
    }
}

void ZFUIAniImageView::styleableOnCopyFrom(ZF_IN ZFStyleable *anotherStyleable)
{
    this->aniStop();

    zfsuper::styleableOnCopyFrom(anotherStyleable);
    zfself *another = ZFCastZFObject(zfself *, anotherStyleable);
    if(another == this || another == zfnull) {return;}

    d->frameSrc = another->d->frameSrc;
    d->frameImages.removeAll();
    d->frameImages.addFrom(another->d->frameImages);
    d->frameSizePixel = another->d->frameSizePixel;
    d->frameDurations.removeAll();
    d->frameDurations.addFrom(another->d->frameDurations);

    d->aniFrame = another->d->aniFrame;
    d->frameTimers.removeAll();
    d->frameTimers.addFrom(another->d->frameTimers);
}
ZFSerializablePropertyType ZFUIAniImageView::serializableOnCheckPropertyType(ZF_IN const ZFProperty *property)
{
    if(property == ZFPropertyAccess(ZFUIImageView, image))
    {
        return ZFSerializablePropertyTypeNotSerializable;
    }
    else
    {
        return zfsuper::serializableOnCheckPropertyType(property);
    }
}

zfbool ZFUIAniImageView::serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                                         ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                                         ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {return zffalse;}
    return zftrue;
}
zfbool ZFUIAniImageView::serializableOnSerializeToData(ZF_IN_OUT ZFSerializableData &serializableData,
                                                       ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                       ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, referencedOwnerOrNull, outErrorHint)) {return zffalse;}
    zfself *ref = ZFCastZFObject(zfself *, referencedOwnerOrNull);
    return zftrue;
}

void ZFUIAniImageView::layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                       ZF_IN const ZFUISize &sizeHint,
                                       ZF_IN const ZFUISizeParam &sizeParam)
{
    ZFUILayoutParam::sizeHintApply(ret,
        ZFUISizeApplyScaleReversely(this->frameSizePixel(), ZFUIGlobalStyle::DefaultStyle()->imageScale()),
        sizeHint,
        sizeParam);
}

ZF_NAMESPACE_GLOBAL_END

