/**
 * @file ZFProtocolZFUIImageView.h
 * @brief protocol for ZFUIImageView
 */

#ifndef _ZFI_ZFProtocolZFUIImageView_h_
#define _ZFI_ZFProtocolZFUIImageView_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFUIKit/ZFUIImageView.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIImage.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for #ZFUIImageView
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFUIImageView)
public:
    /**
     * @brief create native image view
     */
    virtual void *nativeImageViewCreate(ZF_IN ZFUIImageView *imageView) zfpurevirtual;
    /**
     * @brief destroy native image view
     */
    virtual void nativeImageViewDestroy(ZF_IN ZFUIImageView *imageView,
                                        ZF_IN void *nativeImageView) zfpurevirtual;

    /**
     * @brief set the image
     *
     * implementations must not retain the image object,
     * it's ensured set null when owner image view deallocated\n
     * note that implementation must implement the nine patch logic
     */
    virtual void image(ZF_IN ZFUIImageView *imageView,
                       ZF_IN ZFUIImage *image) zfpurevirtual;
    /**
     * @brief called when image's nine patch setting changed,
     *   ensured called once when change image
     *
     * note that the image's size is applied by #ZFUIImage::imageScale,
     * you should take care of the actual scale logic manually
     * @note size is in pixel unit, you may revert it according imageScale
     */
    virtual void imageNinePatchChanged(ZF_IN ZFUIImageView *imageView,
                                       ZF_IN zffloat imageScale,
                                       ZF_IN const ZFUIMargin &imageNinePatch) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFUIImageView)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFUIImageView_h_

