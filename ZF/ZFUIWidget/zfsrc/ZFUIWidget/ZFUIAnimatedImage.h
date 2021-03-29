/**
 * @file ZFUIAnimatedImage.h
 * @brief animated image container using multiple #ZFUIImage
 */

#ifndef _ZFI_ZFUIAnimatedImage_h_
#define _ZFI_ZFUIAnimatedImage_h_

#include "ZFUIWidgetDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIAnimatedImage
/**
 * @brief image frame data for #ZFUIAnimatedImage
 */
zfclass ZF_ENV_EXPORT ZFUIAnimatedImageAniFrame : zfextends ZFObject, zfimplements ZFSerializable, zfimplements ZFCopyable
{
    ZFOBJECT_DECLARE(ZFUIAnimatedImageAniFrame, ZFObject)
    ZFIMPLEMENTS_DECLARE(ZFSerializable, ZFCopyable)

public:
    /**
     * @brief image of the frame
     */
    ZFPROPERTY_RETAIN(ZFUIImage *, aniFrameImage)
    /**
     * @brief duration of this frame, -1 by default
     *
     * negative value means not specified, use #ZFUIAnimatedImage::aniFrameDurationDefault\n
     * 0 means disable this frame
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zftimet, aniFrameDuration, -1)
};

// ============================================================
// ZFUIAnimatedImage
zfclassFwd _ZFP_ZFUIAnimatedImagePrivate;
/**
 * @brief animated image container using multiple #ZFUIImage
 *
 * usage:
 * -  use #ZFUIAnimatedImage::aniFrameAdd to add each frame
 * -  add observer to #ZFUIAnimatedImage::EventAniOnUpdate
 * -  use #ZFUIAnimatedImage::aniFrameImageCurrent to access current image
 */
zfclass ZF_ENV_EXPORT ZFUIAnimatedImage : zfextends ZFStyleableObject
{
    ZFOBJECT_DECLARE(ZFUIAnimatedImage, ZFStyleableObject)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIAnimatedImage)

public:
    // ============================================================
    // event
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when ani started
     */
    ZFOBSERVER_EVENT(AniOnStart)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when ani stopped
     */
    ZFOBSERVER_EVENT(AniOnStop)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when last frame finished
     */
    ZFOBSERVER_EVENT(AniOnRepeat)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called each time the frame changed
     */
    ZFOBSERVER_EVENT(AniOnUpdate)

    /**
     * @brief see #ZFObject::observerNotify
     *
     * called to update the frame
     */
    ZFOBSERVER_EVENT(AniFrameOnUpdate)

public:
    /**
     * @brief default duration between each frame, 35 by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zftimet, aniFrameDurationDefault,
                                35)

    /**
     * @brief whether keep previous state when start animating, true by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, aniKeepPreviousState,
                                zftrue)

    /**
     * @brief number of times to repeat, zfindexMax() by default
     *
     * -  0 means no repeat, run once and stop
     * -  zfindexMax() means repeat until stop manually
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfindex, aniRepeatCount,
                                zfindexMax())

    /**
     * @brief ani frames, holds #ZFUIAnimatedImageAniFrame
     */
    ZFPROPERTY_RETAIN_DETAIL(ZFArray *, aniFrames,
                             zflineAlloc(ZFArray),
                             protected, protected)

public:
    /** @brief see #ZFUIAnimatedImageAniFrame */
    ZFMETHOD_INLINE_0(zfindex, aniFrameCount)
    {
        return this->aniFrames()->count();
    }
    /** @brief see #ZFUIAnimatedImageAniFrame */
    ZFMETHOD_INLINE_2(void, aniFrameAdd,
                      ZFMP_IN(ZFUIImage *, aniFrameImage),
                      ZFMP_IN_OPT(zftimet, aniFrameDuration, (zftimet)-1))
    {
        zfCoreAssert(aniFrameImage != zfnull);
        zfblockedAlloc(ZFUIAnimatedImageAniFrame, aniFrame);
        aniFrame->aniFrameImage(aniFrameImage);
        aniFrame->aniFrameDuration(aniFrameDuration);
        this->aniFrames()->add(aniFrame);
    }
    /** @brief see #ZFUIAnimatedImageAniFrame */
    ZFMETHOD_INLINE_1(void, aniFrameRemoveAtIndex,
                      ZFMP_IN(zfindex, index))
    {
        this->aniFrames()->remove(index);
    }
    /** @brief see #ZFUIAnimatedImageAniFrame */
    ZFMETHOD_INLINE_1(ZFUIAnimatedImageAniFrame *, aniFrameAtIndex,
                      ZFMP_IN(zfindex, index))
    {
        return this->aniFrames()->get<ZFUIAnimatedImageAniFrame *>(index);
    }
    /** @brief see #ZFUIAnimatedImageAniFrame */
    ZFMETHOD_INLINE_1(ZFUIImage *, aniFrameImageAtIndex,
                      ZFMP_IN(zfindex, index))
    {
        return this->aniFrames()->get<ZFUIAnimatedImageAniFrame *>(index)->aniFrameImage();
    }
    /** @brief see #ZFUIAnimatedImageAniFrame */
    ZFMETHOD_INLINE_1(zftimet, aniFrameDurationAtIndex,
                      ZFMP_IN(zfindex, index))
    {
        return this->aniFrames()->get<ZFUIAnimatedImageAniFrame *>(index)->aniFrameDuration();
    }
    /** @brief see #ZFUIAnimatedImageAniFrame */
    ZFMETHOD_INLINE_1(zftimet, aniFrameDurationFixedAtIndex,
                      ZFMP_IN(zfindex, index))
    {
        zftimet ret = this->aniFrames()->get<ZFUIAnimatedImageAniFrame *>(index)->aniFrameDuration();
        if(ret < 0)
        {
            ret = this->aniFrameDurationDefault();
        }
        return ret;
    }

public:
    /**
     * @brief start animation
     */
    ZFMETHOD_DECLARE_0(void, aniStart)
    /**
     * @brief stop animation
     */
    ZFMETHOD_DECLARE_0(void, aniStop)
    /**
     * @brief true if running
     */
    ZFMETHOD_DECLARE_0(zfbool, aniRunning)

public:
    /**
     * @brief manually update frame to specified index,
     *   reset to 0 frame if index out of range
     *
     * note calling this method would cause #aniStop being called first,
     * #EventAniFrameOnUpdate and #EventAniOnUpdate would be notified,
     * other event won't be notified
     */
    ZFMETHOD_DECLARE_1(void, aniManual,
                       ZFMP_IN(zfindex, aniFrame))
    /**
     * @brief see #aniManual
     */
    ZFMETHOD_INLINE_0(void, aniManualPrev)
    {
        this->aniManual((this->aniFrameIndexCurrent() > 0)
            ? (this->aniFrameIndexCurrent() - 1)
            : (this->aniFrameCount() - 1));
    }
    /**
     * @brief see #aniManual
     */
    ZFMETHOD_INLINE_0(void, aniManualNext)
    {
        this->aniManual(this->aniFrameIndexCurrent() + 1);
    }

public:
    /**
     * @brief see #aniFrameIndexCurrent
     */
    ZFMETHOD_DECLARE_0(ZFUIImage *, aniFrameImageCurrent)
    /**
     * @brief see #aniFrameIndexCurrent
     */
    ZFMETHOD_DECLARE_0(zftimet, aniFrameDurationCurrent)
    /**
     * @brief see #aniFrameIndexCurrent
     */
    ZFMETHOD_DECLARE_0(zftimet, aniFrameDurationFixedCurrent)
    /**
     * @brief see #aniFrameIndexCurrent
     */
    ZFMETHOD_DECLARE_0(ZFUIAnimatedImageAniFrame *, aniFrameCurrent)
    /**
     * @brief current ani frame index
     *
     * may be zfindexMax() if not started
     */
    ZFMETHOD_DECLARE_0(zfindex, aniFrameIndexCurrent)

public:
    /**
     * @brief repeat count since start, 0 for first time
     */
    ZFMETHOD_DECLARE_0(zfindex, aniRepeatCountCurrent)

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);

protected:
    /** @brief see #EventAniOnStart */
    virtual inline void aniOnStart(void)
    {
        this->observerNotify(ZFUIAnimatedImage::EventAniOnStart());
    }
    /** @brief see #EventAniOnStop */
    virtual inline void aniOnStop(void)
    {
        this->observerNotify(ZFUIAnimatedImage::EventAniOnStop());
    }
    /** @brief see #EventAniOnRepeat */
    virtual inline void aniOnRepeat(void)
    {
        this->observerNotify(ZFUIAnimatedImage::EventAniOnRepeat());
    }
    /** @brief see #EventAniOnUpdate */
    virtual inline void aniOnUpdate(void)
    {
        this->observerNotify(ZFUIAnimatedImage::EventAniOnUpdate());
    }
    /** @brief see #EventAniFrameOnUpdate */
    virtual inline void aniFrameOnUpdate(void)
    {
        this->observerNotify(ZFUIAnimatedImage::EventAniFrameOnUpdate());
    }

private:
    _ZFP_ZFUIAnimatedImagePrivate *d;
    friend zfclassFwd _ZFP_ZFUIAnimatedImagePrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIAnimatedImage_h_

