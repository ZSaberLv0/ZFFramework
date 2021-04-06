#include "ZFUIAniImageView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
zfclassNotPOD _ZFP_ZFUIAniImageDataPrivate
{
public:
    zfautoObjectT<ZFUIImage *> frameSrc;
    ZFUISize frameSizePixel;
    zfindex frameCount;
    ZFCoreArrayPOD<zftimet> frameDurations;

    ZFCoreArray<zfautoObjectT<ZFUIImage *> > frameImages;
    ZFCoreArrayPOD<zfuint> frameTimers;
};

// ============================================================
ZFOBJECT_REGISTER(ZFUIAniImageData)

ZFOBSERVER_EVENT_REGISTER(ZFUIAniImageData, AniDataOnUpdate)

ZFMETHOD_DEFINE_4(ZFUIAniImageData, zfbool, aniLoad,
                  ZFMP_IN(ZFUIImage *, frameSrc),
                  ZFMP_IN(const ZFUISize &, frameSizePixel),
                  ZFMP_IN_OPT(zfindex, frameCount, zfindexMax()),
                  ZFMP_IN_OPT(ZFCoreArrayPOD<zftimet> const &, frameDurations, ZFCoreArrayPOD<zftimet>()))
{
    if(frameSrc == zfnull
        || frameSizePixel.width <= 0 || frameSizePixel.height <= 0
        || frameCount <= 0
    ) {
        return zffalse;
    }
    const ZFUISize &imageSizeFixed = frameSrc->imageSizeFixed();
    if(imageSizeFixed.width <= 0 || imageSizeFixed.height <= 0)
    {
        return zffalse;
    }

    d->frameSrc = zfnull;
    d->frameSizePixel = frameSizePixel;
    d->frameCount = frameCount;
    d->frameDurations.removeAll();

    d->frameImages.removeAll();
    d->frameTimers.removeAll();

    for(zfint y = 0, yEnd = imageSizeFixed.height - frameSizePixel.height; y <= yEnd && d->frameImages.count() < frameCount; y += frameSizePixel.height)
    {
        for(zfint x = 0, xEnd = imageSizeFixed.width - frameSizePixel.width; x <= xEnd && d->frameImages.count() < frameCount; x += frameSizePixel.width)
        {
            zfautoObjectT<ZFUIImage *> frameImage = ZFUIImageLoadInFrame(frameSrc, ZFUIRectMake(
                    x,
                    y,
                    frameSizePixel.width,
                    frameSizePixel.height
                ));
            if(frameImage == zfnull)
            {
                d->frameImages.removeAll();
                this->observerNotify(ZFUIAniImageData::EventAniDataOnUpdate());
                return zffalse;
            }
            d->frameImages.add(frameImage);
        }
    }
    if(d->frameImages.isEmpty())
    {
        this->observerNotify(ZFUIAniImageData::EventAniDataOnUpdate());
        return zffalse;
    }

    d->frameSrc = frameSrc;
    d->frameDurations.copyFrom(frameDurations);

    frameCount = zfmMin(frameCount, d->frameImages.count());
    zfindex frameCountTmp = zfmMin(frameDurations.count(), frameCount);
    zftimet interval = ZFGlobalTimerIntervalDefault();
    for(zfindex i = 0; i < frameCountTmp; ++i)
    {
        zfuint frameTimer = (zfuint)(frameDurations[i] / interval);
        d->frameTimers.add(frameTimer > 0 ? frameTimer : 1);
    }
    zfuint frameTimerLast = d->frameTimers.isEmpty() ? 1 : d->frameTimers.getLast();
    for(zfindex i = frameCountTmp; i < frameCount; ++i)
    {
        d->frameTimers.add(frameTimerLast);
    }

    this->observerNotify(ZFUIAniImageData::EventAniDataOnUpdate());
    return zftrue;
}

ZFMETHOD_DEFINE_0(ZFUIAniImageData, zfautoObjectT<ZFUIImage *> const &, frameSrc)
{
    return d->frameSrc;
}
ZFMETHOD_DEFINE_0(ZFUIAniImageData, ZFUISize const &, frameSizePixel)
{
    return d->frameSizePixel;
}
ZFMETHOD_DEFINE_0(ZFUIAniImageData, zfindex const &, frameCount)
{
    return d->frameCount;
}
ZFMETHOD_DEFINE_0(ZFUIAniImageData, ZFCoreArrayPOD<zftimet> const &, frameDurations)
{
    return d->frameDurations;
}

ZFMETHOD_DEFINE_0(ZFUIAniImageData, ZFCoreArray<zfautoObjectT<ZFUIImage *> > const &, frameImages)
{
    return d->frameImages;
}
ZFMETHOD_DEFINE_0(ZFUIAniImageData, ZFCoreArrayPOD<zfuint> const &, frameTimers)
{
    return d->frameTimers;
}

ZFOBJECT_ON_INIT_DEFINE_4(ZFUIAniImageData,
                          ZFMP_IN(ZFUIImage *, frameSrc),
                          ZFMP_IN(const ZFUISize &, frameSizePixel),
                          ZFMP_IN_OPT(zfindex, frameCount, zfindexMax()),
                          ZFMP_IN_OPT(ZFCoreArrayPOD<zftimet> const &, frameDurations, ZFCoreArrayPOD<zftimet>()))
{
    this->objectOnInit();
    this->aniLoad(frameSrc, frameSizePixel, frameCount, frameDurations);
}

void ZFUIAniImageData::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIAniImageDataPrivate);
}
void ZFUIAniImageData::objectOnDealloc(void)
{
    zfpoolDelete(d);
    zfsuper::objectOnDealloc();
}

void ZFUIAniImageData::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);
    if(!this->frameImages().isEmpty())
    {
        ret += " ";
        ZFUISizeToString(ret, this->frameSizePixel());
        ret += ",";
        zfindexToString(ret, this->frameCount());
    }
}
ZFCompareResult ZFUIAniImageData::objectCompare(ZF_IN ZFObject *anotherObj)
{
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another != zfnull
        && ZFObjectCompare(this->frameSrc(), another->frameSrc()) == ZFCompareTheSame
        && this->frameSizePixel() == another->frameSizePixel()
        && this->frameCount() == another->frameCount()
        && this->frameDurations().objectCompare(another->frameDurations()) == ZFCompareTheSame
    ) {
        return ZFCompareTheSame;
    }
    else
    {
        return ZFCompareUncomparable;
    }
}

void ZFUIAniImageData::styleableOnCopyFrom(ZF_IN ZFStyleable *anotherStyleable)
{
    zfsuper::styleableOnCopyFrom(anotherStyleable);
    zfself *another = ZFCastZFObject(zfself *, anotherStyleable);
    if(another == this || another == zfnull) {return;}

    d->frameSrc = another->d->frameSrc;
    d->frameSizePixel = another->d->frameSizePixel;
    d->frameCount = another->d->frameCount;
    d->frameDurations.copyFrom(another->d->frameDurations);

    d->frameImages.copyFrom(another->d->frameImages);
    d->frameTimers.copyFrom(another->d->frameTimers);

    this->observerNotify(ZFUIAniImageData::EventAniDataOnUpdate());
}

zfbool ZFUIAniImageData::serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                                         ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                                         ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {return zffalse;}
    const ZFSerializableData *frameSrcData = ZFSerializableUtil::requireElementByCategory(serializableData, ZFSerializableKeyword_ZFUIAniImageView_frameSrc, outErrorHint, outErrorPos);
    if(frameSrcData == zfnull)
    {
        return zffalse;
    }

    zfautoObject frameSrcImage;
    if(!ZFObjectFromData(frameSrcImage, *frameSrcData, outErrorHint, outErrorPos))
    {
        return zffalse;
    }
    if(frameSrcImage == zfnull || !frameSrcImage->classData()->classIsTypeOf(ZFUIImage::ClassData()))
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, *frameSrcData,
            "invalid frameSrc: %s", ZFObjectInfo(frameSrcImage).cString());
        return zffalse;
    }

    ZFUISize frameSizePixel = ZFUISizeZero();
    const zfchar *frameSizePixelString = ZFSerializableUtil::requireAttribute(serializableData, ZFSerializableKeyword_ZFUIAniImageView_frameSizePixel, outErrorHint, outErrorPos);
    if(frameSizePixelString == zfnull || !ZFUISizeFromString(frameSizePixel, frameSizePixelString))
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
            "invalid %s: %s",
            ZFSerializableKeyword_ZFUIAniImageView_frameSizePixel,
            frameSizePixelString);
        return zffalse;
    }

    zfindex frameCount = zfindexMax();
    const zfchar *frameCountString = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFUIAniImageView_frameCount);
    if(frameCountString != zfnull && !zfindexFromString(frameCount, frameCountString))
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
            "invalid %s: %s",
            ZFSerializableKeyword_ZFUIAniImageView_frameCount,
            frameCountString);
        return zffalse;
    }

    ZFCoreArrayPOD<zftimet> frameDurations;
    const zfchar *frameDurationsString = ZFSerializableUtil::checkAttribute(serializableData, ZFSerializableKeyword_ZFUIAniImageView_frameDurations);
    if(frameDurationsString != zfnull && !ZFCoreArrayFromString(frameDurations, zftimetFromString, frameDurationsString))
    {
        ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, serializableData,
            "invalid %s: %s",
            ZFSerializableKeyword_ZFUIAniImageView_frameDurations,
            frameDurationsString);
        return zffalse;
    }

    return this->aniLoad(frameSrcImage, frameSizePixel, frameCount, frameDurations);
}
zfbool ZFUIAniImageData::serializableOnSerializeToData(ZF_IN_OUT ZFSerializableData &serializableData,
                                                       ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                       ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, referencedOwnerOrNull, outErrorHint)) {return zffalse;}
    if(referencedOwnerOrNull != zfnull && this->objectCompare(referencedOwnerOrNull->toObject()) == ZFCompareTheSame)
    {
        return zftrue;
    }
    if(this->frameImages().isEmpty())
    {
        return zftrue;
    }

    ZFSerializableData frameSrcData;
    if(!this->frameSrc()->serializeToData(frameSrcData, outErrorHint))
    {
        return zffalse;
    }
    frameSrcData.category(ZFSerializableKeyword_ZFUIAniImageView_frameSrc);
    serializableData.elementAdd(frameSrcData);

    zfstring frameSizePixelString;
    if(!ZFUISizeToString(frameSizePixelString, this->frameSizePixel()))
    {
        return zffalse;
    }
    serializableData.attributeForName(ZFSerializableKeyword_ZFUIAniImageView_frameSizePixel, frameSizePixelString);

    if(this->frameCount() != zfindexMax())
    {
        zfstring frameCountString;
        if(!zfindexToString(frameCountString, this->frameCount()))
        {
            return zffalse;
        }
        serializableData.attributeForName(ZFSerializableKeyword_ZFUIAniImageView_frameCount, frameCountString);
    }

    if(!this->frameDurations().isEmpty())
    {
        zfstring frameDurationsString;
        if(!ZFCoreArrayToString(frameDurationsString, zftimetToString, this->frameDurations()))
        {
            return zffalse;
        }
        serializableData.attributeForName(ZFSerializableKeyword_ZFUIAniImageView_frameDurations, frameDurationsString);
    }

    return zftrue;
}

// ============================================================
zfclassNotPOD _ZFP_ZFUIAniImageViewPrivate
{
public:
    enum {
        stateFlag_aniStarted = 1 << 0,
        stateFlag_observerHasAddFlag_aniOnStart = 1 << 1,
        stateFlag_observerHasAddFlag_aniOnLoop = 1 << 2,
        stateFlag_observerHasAddFlag_aniOnStop = 1 << 3,
        stateFlag_observerHasAddFlag_aniOnFrame = 1 << 4,
    };
    zfuint stateFlag;
    zfindex aniFrame;
    zfindex frameTimerToNext;
    zfindex aniCount;
    ZFListener onTimerListener;

public:
    _ZFP_ZFUIAniImageViewPrivate(void)
    : stateFlag(0)
    , aniFrame(zfindexMax())
    , frameTimerToNext(0)
    , aniCount(0)
    , onTimerListener(ZFCallbackForFunc(onTimer))
    {
    }

public:
    static void onTimer(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject *userData)
    {
        ZFUIAniImageView *pimplOwner = userData->objectHolded();
        _ZFP_ZFUIAniImageViewPrivate *d = pimplOwner->d;
        --(d->frameTimerToNext);
        if(d->frameTimerToNext > 0)
        {
            return;
        }
        ZFUIAniImageData *aniData = pimplOwner->aniData();

        if(d->aniFrame == aniData->frameImages().count() - 1 && d->aniCount == 1)
        {
            pimplOwner->aniStop();
            return;
        }

        d->aniFrame = ((d->aniFrame + 1) % aniData->frameImages().count());
        zfbool isLoop = (d->aniFrame == 0);

        d->frameTimerToNext = aniData->frameTimers()[d->aniFrame];
        if(isLoop && d->aniCount > 0)
        {
            --(d->aniCount);
        }
        pimplOwner->image(aniData->frameImages()[d->aniFrame]);
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
                  ZFMP_IN(ZFUIImage *, frameSrc),
                  ZFMP_IN(const ZFUISize &, frameSizePixel),
                  ZFMP_IN_OPT(zfindex, frameCount, zfindexMax()),
                  ZFMP_IN_OPT(ZFCoreArrayPOD<zftimet> const &, frameDurations, ZFCoreArrayPOD<zftimet>()))
{
    if(this->aniData() == zfnull)
    {
        this->aniData(zflineAlloc(ZFUIAniImageData));
    }
    if(!this->aniData()->aniLoad(frameSrc, frameSizePixel, frameCount, frameDurations))
    {
        d->aniFrame = zfindexMax();
        this->image(zfnull);
        return zffalse;
    }
    if(d->aniFrame >= this->aniData()->frameImages().count())
    {
        d->aniFrame = 0;
    }
    this->image(this->aniData()->frameImages()[d->aniFrame]);
    this->aniOnFrame();
    return zftrue;
}

ZFMETHOD_DEFINE_0(ZFUIAniImageView, zfbool, aniDataValid)
{
    return this->aniData() != zfnull && !this->aniData()->frameImages().isEmpty();
}

ZFMETHOD_DEFINE_0(ZFUIAniImageView, zfbool, aniStarted)
{
    return ZFBitTest(d->stateFlag, _ZFP_ZFUIAniImageViewPrivate::stateFlag_aniStarted);
}
ZFMETHOD_DEFINE_1(ZFUIAniImageView, void, aniStart,
                  ZFMP_IN_OPT(zfindex, aniCount, 1))
{
    this->aniStop();
    if(!this->aniDataValid())
    {
        return;
    }
    ZFBitSet(d->stateFlag, _ZFP_ZFUIAniImageViewPrivate::stateFlag_aniStarted);
    zfRetain(this);

    ++(d->aniFrame);
    if(d->aniFrame >= this->aniData()->frameImages().count())
    {
        d->aniFrame = 0;
    }
    d->frameTimerToNext = this->aniData()->frameTimers()[d->aniFrame];
    d->aniCount = aniCount;
    ZFGlobalTimerAttach(d->onTimerListener, this->objectHolder());

    this->image(this->aniData()->frameImages()[d->aniFrame]);
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
    if(!this->aniDataValid())
    {
        d->aniFrame = zfindexMax();
        return;
    }
    if(d->aniFrame == aniFrame)
    {
        return;
    }
    if(aniFrame >= this->aniData()->frameImages().count())
    {
        d->aniFrame = 0;
    }
    else
    {
        d->aniFrame = aniFrame;
    }
    this->image(this->aniData()->frameImages()[d->aniFrame]);
    this->aniOnFrame();
}

ZFMETHOD_DEFINE_0(ZFUIAniImageView, void, aniFrameNext)
{
    if(this->aniDataValid())
    {
        if(d->aniFrame < this->aniData()->frameImages().count() - 1)
        {
            this->aniFrame(d->aniFrame + 1);
        }
        else
        {
            this->aniFrame(0);
        }
    }
}
ZFMETHOD_DEFINE_0(ZFUIAniImageView, void, aniFramePrev)
{
    if(this->aniDataValid())
    {
        if(d->aniFrame == 0)
        {
            this->aniFrame(this->aniData()->frameImages().count() - 1);
        }
        else
        {
            this->aniFrame(d->aniFrame - 1);
        }
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
ZFOBJECT_ON_INIT_DEFINE_4(ZFUIAniImageView,
                          ZFMP_IN(ZFUIImage *, frameSrc),
                          ZFMP_IN(const ZFUISize &, frameSizePixel),
                          ZFMP_IN_OPT(zfindex, frameCount, zfindexMax()),
                          ZFMP_IN_OPT(ZFCoreArrayPOD<zftimet> const &, frameDurations, ZFCoreArrayPOD<zftimet>()))
{
    this->objectOnInit();
    this->aniLoad(frameSrc, frameSizePixel, frameCount, frameDurations);
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
    if(this->aniDataValid())
    {
        ret += " ";
        ZFUISizeToString(ret, this->aniData()->frameSizePixel());
        ret += ",";
        zfindexToString(ret, this->aniData()->frameCount());
    }
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

void ZFUIAniImageView::layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                       ZF_IN const ZFUISize &sizeHint,
                                       ZF_IN const ZFUISizeParam &sizeParam)
{
    if(this->aniDataValid())
    {
        ZFUILayoutParam::sizeHintApply(ret,
            ZFUISizeApplyScaleReversely(this->aniData()->frameSizePixel(), ZFUIGlobalStyle::DefaultStyle()->imageScale()),
            sizeHint,
            sizeParam);
    }
}

// ============================================================
static zfbool _ZFP_ZFUIAniImageCreate(ZF_IN const ZFClass *desiredClass,
                                      ZF_IN const ZFPathInfo &pathInfo,
                                      ZF_IN const ZFCoreArray<zfautoObjectT<ZFUIImage *> > &frameImages,
                                      ZF_IN_OPT const ZFCoreArrayPOD<zftimet> &frameDurations = ZFCoreArrayPOD<zftimet>());
ZFMETHOD_DEFINE_3(ZFUIAniImageData, zfbool, Create,
                  ZFMP_IN(const ZFPathInfo &, pathInfo),
                  ZFMP_IN(const ZFCoreArray<zfautoObjectT<ZFUIImage *> > &, frameImages),
                  ZFMP_IN_OPT(const ZFCoreArrayPOD<zftimet> &, frameDurations, ZFCoreArrayPOD<zftimet>()))
{
    return _ZFP_ZFUIAniImageCreate(zfself::ClassData(), pathInfo, frameImages, frameDurations);
}
ZFMETHOD_DEFINE_3(ZFUIAniImageView, zfbool, Create,
                  ZFMP_IN(const ZFPathInfo &, pathInfo),
                  ZFMP_IN(const ZFCoreArray<zfautoObjectT<ZFUIImage *> > &, frameImages),
                  ZFMP_IN_OPT(const ZFCoreArrayPOD<zftimet> &, frameDurations, ZFCoreArrayPOD<zftimet>()))
{
    return _ZFP_ZFUIAniImageCreate(zfself::ClassData(), pathInfo, frameImages, frameDurations);
}
static zfbool _ZFP_ZFUIAniImageCreate(ZF_IN const ZFClass *desiredClass,
                                      ZF_IN const ZFPathInfo &pathInfo,
                                      ZF_IN const ZFCoreArray<zfautoObjectT<ZFUIImage *> > &frameImages,
                                      ZF_IN_OPT const ZFCoreArrayPOD<zftimet> &frameDurations /* = ZFCoreArrayPOD<zftimet>() */)
{
    if(frameImages.isEmpty())
    {
        return zffalse;
    }
    ZFOutput objOutput = ZFOutputForPathInfo(pathInfo);
    if(!objOutput.callbackIsValid())
    {
        return zffalse;
    }
    ZFPathInfo imgPathInfo = pathInfo;
    zfindex dotPos = zfstringFindReversely(imgPathInfo.pathData, '.');
    if(dotPos == zfindexMax())
    {
        return zffalse;
    }
    ++dotPos;
    imgPathInfo.pathData.replace(dotPos, imgPathInfo.pathData.length() - dotPos, "png");
    ZFOutput imgOutput = ZFOutputForPathInfo(imgPathInfo);
    if(!imgOutput.callbackIsValid())
    {
        return zffalse;
    }
    zfstring imgFileName;
    if(!ZFFilePathInfoToFileName(imgPathInfo, imgFileName))
    {
        return zffalse;
    }

    const ZFUISize &frameSizePixel = frameImages[0]->imageSizeFixed();
    zfindex frameCount = frameImages.count();

    zfuint xCount = 1;
    zfuint yCount = (zfuint)frameCount;
    zfuint minSize = xCount * yCount;
    zfuint minDiff = zfmAbs((zfint)(frameSizePixel.width * xCount) - (zfint)(frameSizePixel.height * yCount));
    for(zfuint x = 2; x <= frameCount; ++x)
    {
        zfuint y = ((frameCount % x) == 0) ? (zfuint)(frameCount / x) : (zfuint)(frameCount / x + 1);
        zfuint size = x * y;
        zfuint diff = zfmAbs((zfint)(frameSizePixel.width * x) - (zfint)(frameSizePixel.height * y));
        if(size <= minSize && diff <= minDiff)
        {
            xCount = x;
            yCount = y;
            minSize = size;
            minDiff = diff;
        }
    }

    void *context = ZFUIDraw::beginForImage(ZFUISizeMake(frameSizePixel.width * xCount, frameSizePixel.height * yCount));
    for(zfuint y = 0, iFrame = 0; y < yCount; ++y)
    {
        for(zfuint x = 0; x < xCount; ++x, ++iFrame)
        {
            ZFUIDraw::drawImage(context, frameImages[iFrame], ZFUIRectZero(), ZFUIRectMake(
                frameSizePixel.width * x,
                frameSizePixel.height * y,
                frameSizePixel.width,
                frameSizePixel.height));
        }
    }
    zfautoObjectT<ZFUIImage *> frameSrc = ZFUIDraw::endForImage(context);
    if(frameSrc == zfnull || !ZFUIImageSaveToFile(imgOutput, frameSrc))
    {
        return zffalse;
    }

    imgOutput.callbackClear(); // clear to make the file readable
    ZFInput imgInput = ZFInputForLocalFile(pathInfo, imgFileName);
    if(!imgInput.callbackIsValid())
    {
        return zffalse;
    }
    ZFSerializableData imgInputData;
    if(!ZFCallbackToData(imgInputData, imgInput))
    {
        return zffalse;
    }
    frameSrc->imageSerializableType(ZFUIImageSerializeType_input);
    frameSrc->imageSerializableData(&imgInputData);

    zfautoObject result = desiredClass->newInstance();
    if(desiredClass->classIsTypeOf(ZFUIAniImageData::ClassData()))
    {
        if(!result.to<ZFUIAniImageData *>()->aniLoad(frameSrc, frameSizePixel, frameCount, frameDurations))
        {
            return zffalse;
        }
    }
    else
    {
        if(!result.to<ZFUIAniImageView *>()->aniLoad(frameSrc, frameSizePixel, frameCount, frameDurations))
        {
            return zffalse;
        }
    }

    if(!ZFObjectIOSave(objOutput, result))
    {
        return zffalse;
    }

    return zftrue;
}

ZF_NAMESPACE_GLOBAL_END

