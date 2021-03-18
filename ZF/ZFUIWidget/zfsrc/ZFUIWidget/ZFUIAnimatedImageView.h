/**
 * @file ZFUIAnimatedImageView.h
 * @brief image view to hold #ZFUIAnimatedImage
 */

#ifndef _ZFI_ZFUIAnimatedImageView_h_
#define _ZFI_ZFUIAnimatedImageView_h_

#include "ZFUIAnimatedImage.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIAnimatedImageView
zfclassFwd _ZFP_ZFUIAnimatedImageViewPrivate;
/**
 * @brief image view to hold #ZFUIAnimatedImage
 */
zfclass ZF_ENV_EXPORT ZFUIAnimatedImageView : zfextends ZFUIImageView
{
    ZFOBJECT_DECLARE(ZFUIAnimatedImageView, ZFUIImageView)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIAnimatedImageView)

public:
    /**
     * @brief the image
     */
    ZFPROPERTY_RETAIN_READONLY(ZFUIAnimatedImage *, animatedImage, zflineAlloc(ZFUIAnimatedImage))
    /**
     * @brief whether wrap size to each image frame's size, false by default
     *
     * -  true: this view would be measured each time the image frame updated,
     *   and this view's size would be measured to the current image frame's size
     * -  false: this view would not be measured when image frame updated,
     *   and this view's size would be measured to the max size of all image frame
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, animatedImageWrapToImage, zffalse)
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(zfbool, animatedImageWrapToImage)

public:
    /** @brief see #ZFUIAnimatedImage::aniStart */
    ZFMETHOD_INLINE_0(void, aniStart)
    {
        this->animatedImage()->aniStart();
    }
    /** @brief see #ZFUIAnimatedImage::aniStop */
    ZFMETHOD_INLINE_0(void, aniStop)
    {
        this->animatedImage()->aniStop();
    }
    /** @brief see #ZFUIAnimatedImage::aniRunning */
    ZFMETHOD_INLINE_0(zfbool, aniRunning)
    {
        return this->animatedImage()->aniRunning();
    }

protected:
    zfoverride
    virtual void objectOnInitFinish(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);

    zfoverride
    virtual void layoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds);
    zfoverride
    virtual void layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                 ZF_IN const ZFUISize &sizeHint,
                                 ZF_IN const ZFUISizeParam &sizeParam);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIAnimatedImageView_h_

