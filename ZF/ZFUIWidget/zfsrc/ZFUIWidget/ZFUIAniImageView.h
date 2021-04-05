/**
 * @file ZFUIAniImageView.h
 * @brief animated image view
 */

#ifndef _ZFI_ZFUIAniImageView_h_
#define _ZFI_ZFUIAniImageView_h_

#include "ZFUIWidgetDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFUIAniImageDataPrivate;
/** @brief see #ZFUIAniImageView */
zffinal zfclass ZF_ENV_EXPORT ZFUIAniImageData : zfextends ZFStyleableObject
{
    ZFOBJECT_DECLARE(ZFUIAniImageData, ZFStyleableObject)

public:
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when internal data changed by #aniLoad
     */
    ZFOBSERVER_EVENT(AniDataOnUpdate)

public:
    /**
     * @brief util to create files for #ZFUIAniImageView
     *
     * usage:
     * @code
     *   ZFUIAniImageData::Create(
     *       ZFPathInfo(ZFPathType_storageSharedPath, "yourFileName.xml"),
     *       frameImages);
     * @endcode
     *
     * how it works:
     * -# merge large image from frameImages
     * -# #ZFObjectIOSave according to pathInfo,
     *   which would write "yourFileName.png" and "yourFileName.xml"
     */
    ZFMETHOD_DECLARE_STATIC_3(zfbool, Create,
                              ZFMP_IN(const ZFPathInfo &, pathInfo),
                              ZFMP_IN(const ZFCoreArray<zfautoObjectT<ZFUIImage *> > &, frameImages),
                              ZFMP_IN_OPT(const ZFCoreArrayPOD<zftimet> &, frameDurations, ZFCoreArrayPOD<zftimet>()))

public:
    /** @brief see #ZFUIAniImageView */
    ZFMETHOD_DECLARE_4(zfbool, aniLoad,
                       ZFMP_IN(ZFUIImage *, frameSrc),
                       ZFMP_IN(const ZFUISize &, frameSizePixel),
                       ZFMP_IN_OPT(zfindex, frameCount, zfindexMax()),
                       ZFMP_IN_OPT(ZFCoreArrayPOD<zftimet> const &, frameDurations, ZFCoreArrayPOD<zftimet>()))

    /** @brief see #ZFUIAniImageView */
    ZFMETHOD_DECLARE_0(zfautoObjectT<ZFUIImage *> const &, frameSrc)
    /** @brief see #ZFUIAniImageView */
    ZFMETHOD_DECLARE_0(ZFUISize const &, frameSizePixel)
    /** @brief see #ZFUIAniImageView */
    ZFMETHOD_DECLARE_0(zfindex const &, frameCount)
    /** @brief see #ZFUIAniImageView */
    ZFMETHOD_DECLARE_0(ZFCoreArrayPOD<zftimet> const &, frameDurations)

    /** @brief see #ZFUIAniImageView */
    ZFMETHOD_DECLARE_0(ZFCoreArray<zfautoObjectT<ZFUIImage *> > const &, frameImages)
    /** @brief see #ZFUIAniImageView */
    ZFMETHOD_DECLARE_0(ZFCoreArrayPOD<zfuint> const &, frameTimers)

protected:
    /**
     * @brief construct by #aniLoad
     */
    ZFOBJECT_ON_INIT_DECLARE_4(ZFMP_IN(ZFUIImage *, frameSrc),
                               ZFMP_IN(const ZFUISize &, frameSizePixel),
                               ZFMP_IN_OPT(zfindex, frameCount, zfindexMax()),
                               ZFMP_IN_OPT(ZFCoreArrayPOD<zftimet> const &, frameDurations, ZFCoreArrayPOD<zftimet>()))
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);
    zfoverride
    virtual ZFCompareResult objectCompare(ZF_IN ZFObject *anotherObj);

    zfoverride
    virtual void styleableOnCopyFrom(ZF_IN ZFStyleable *anotherStyleable);

    zfoverride
    virtual zfbool serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                                   ZF_OUT_OPT zfstring *outErrorHint = zfnull,
                                                   ZF_OUT_OPT ZFSerializableData *outErrorPos = zfnull);
    zfoverride
    virtual zfbool serializableOnSerializeToData(ZF_IN_OUT ZFSerializableData &serializableData,
                                                 ZF_IN ZFSerializable *referencedOwnerOrNull,
                                                 ZF_OUT_OPT zfstring *outErrorHint = zfnull);

private:
    _ZFP_ZFUIAniImageDataPrivate *d;
};

// ============================================================
// ZFUIAniImageView
zfclassFwd _ZFP_ZFUIAniImageViewPrivate;
/**
 * @brief animated image view
 *
 * typical usage:
 * @code
 *   zfblockedAlloc(ZFUIAniImageView, aniImg);
 *   aniImg->aniLoad(frameSrc, frameSizePixel, frameCount, frameDurations);
 *   aniImg->aniStart();
 *
 *   // or, share same aniData
 *   zfblockedAlloc(ZFUIAniImageView, aniImg2);
 *   aniImg2->aniData(aniImg->aniData());
 *   aniImg2->aniStart();
 * @endcode
 *
 * how it works:
 * -# load a large image
 * -# load each frame by #ZFUIImageLoadInFrame from the large image
 * -# animate by switching each frame
 *
 * serializable data:
 * @code
 *   <ZFUIAniImageView>
 *       <ZFUIAniImageData name="aniData"
 *               frameSizePixel="(40,60)" // size of each frame (in pixel)
 *               frameCount="123" // optional, total frame count
 *               frameDurations="[33,1000]" // optional, list of duration of each frame
 *                                          // use prev item if less than frameCount
 *                                          // use ZFGlobalTimerIntervalDefault if not specified
 *           >
 *           <ZFUIImage category="frameSrc" // the referenced large image
 *               />
 *       </ZFUIAniImageData>
 *   </ZFUIAniImageView>
 * @endcode
 */
zfclass ZF_ENV_EXPORT ZFUIAniImageView : zfextends ZFUIImageView
{
    ZFOBJECT_DECLARE(ZFUIAniImageView, ZFUIImageView)

public:
    // ============================================================
    // events
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when #aniStart
     */
    ZFOBSERVER_EVENT(AniOnStart)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when all frames finished,
     * not called if ready to #EventAniOnStop
     */
    ZFOBSERVER_EVENT(AniOnLoop)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when #aniStop
     */
    ZFOBSERVER_EVENT(AniOnStop)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when frame changed (both for #aniStart or change #aniFrame manually),
     * current frame can be obtained by #aniFrame
     */
    ZFOBSERVER_EVENT(AniOnFrame)

public:
    /**
     * @brief see #ZFUIAniImageData::Create
     */
    ZFMETHOD_DECLARE_STATIC_3(zfbool, Create,
                              ZFMP_IN(const ZFPathInfo &, pathInfo),
                              ZFMP_IN(const ZFCoreArray<zfautoObjectT<ZFUIImage *> > &, frameImages),
                              ZFMP_IN_OPT(const ZFCoreArrayPOD<zftimet> &, frameDurations, ZFCoreArrayPOD<zftimet>()))

    // ============================================================
    // frame control
public:
    /**
     * @brief util to load #aniData
     */
    ZFMETHOD_DECLARE_4(zfbool, aniLoad,
                       ZFMP_IN(ZFUIImage *, frameSrc),
                       ZFMP_IN(const ZFUISize &, frameSizePixel),
                       ZFMP_IN_OPT(zfindex, frameCount, zfindexMax()),
                       ZFMP_IN_OPT(ZFCoreArrayPOD<zftimet> const &, frameDurations, ZFCoreArrayPOD<zftimet>()))

    /**
     * @brief the animation data, see #ZFUIAniImageView
     */
    ZFPROPERTY_RETAIN(ZFUIAniImageData *, aniData)

    /**
     * @brief true if #aniData is not null and valid
     */
    ZFMETHOD_DECLARE_0(zfbool, aniDataValid)

public:
    /** @brief true if #aniStart running */
    ZFMETHOD_DECLARE_0(zfbool, aniStarted)
    /**
     * @brief start animate the image
     *
     * aniCount is the number to play, 0 means infinite until #aniStop\n
     * this method would always start for "previous" frame,
     * if you want to start from first frame,
     * call #aniFrame before calling this method
     */
    ZFMETHOD_DECLARE_1(void, aniStart,
                       ZFMP_IN_OPT(zfindex, aniCount, 1))
    /**
     * @brief stop animate the image
     *
     * this method would always keep current frame,
     * to reset to first or other frame,
     * you may manually call #aniFrame
     */
    ZFMETHOD_DECLARE_0(void, aniStop)

    /**
     * @brief current frame index, return zfindexMax if no available frame
     */
    ZFMETHOD_DECLARE_0(zfindex const &, aniFrame)
    /**
     * @brief manually change the frame
     *
     * note, if this method is called when #aniStart running,
     * #aniStop would be called to stop the animation\n
     * calling this method would notify #EventAniOnFrame,
     * but no other event would be notified
     */
    ZFMETHOD_DECLARE_1(void, aniFrame,
                       ZFMP_IN(zfindex const &, aniFrame))

    /** @brief util to manually move #aniFrame to next */
    ZFMETHOD_DECLARE_0(void, aniFrameNext)
    /** @brief util to manually move #aniFrame to prev */
    ZFMETHOD_DECLARE_0(void, aniFramePrev)

    // ============================================================
    // events
protected:
    /** @brief see #EventAniOnStart */
    virtual void aniOnStart(void);
    /** @brief see #EventAniOnLoop */
    virtual void aniOnLoop(void);
    /** @brief see #EventAniOnStop */
    virtual void aniOnStop(void);
    /** @brief see #EventAniOnFrame */
    virtual void aniOnFrame(void);

    zfoverride
    virtual void observerOnAdd(ZF_IN zfidentity eventId);
    zfoverride
    virtual void observerOnRemove(ZF_IN zfidentity eventId);

protected:
    /**
     * @brief construct by #aniLoad
     */
    ZFOBJECT_ON_INIT_DECLARE_4(ZFMP_IN(ZFUIImage *, frameSrc),
                               ZFMP_IN(const ZFUISize &, frameSizePixel),
                               ZFMP_IN_OPT(zfindex, frameCount, zfindexMax()),
                               ZFMP_IN_OPT(ZFCoreArrayPOD<zftimet> const &, frameDurations, ZFCoreArrayPOD<zftimet>()))

    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);

    zfoverride
    virtual ZFSerializablePropertyType serializableOnCheckPropertyType(ZF_IN const ZFProperty *property);

    zfoverride
    virtual void layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                 ZF_IN const ZFUISize &sizeHint,
                                 ZF_IN const ZFUISizeParam &sizeParam);

private:
    _ZFP_ZFUIAniImageViewPrivate *d;
    friend zfclassFwd _ZFP_ZFUIAniImageViewPrivate;
};

/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIAniImageView_frameSrc "frameSrc"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIAniImageView_frameSizePixel "frameSizePixel"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIAniImageView_frameCount "frameCount"
/** @brief keyword for serialize */
#define ZFSerializableKeyword_ZFUIAniImageView_frameDurations "frameDurations"

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIAniImageView_h_

