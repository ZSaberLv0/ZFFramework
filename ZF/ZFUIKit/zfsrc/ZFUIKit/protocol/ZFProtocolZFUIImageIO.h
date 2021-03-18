/**
 * @file ZFProtocolZFUIImageIO.h
 * @brief protocol for ZFUIImage
 */

#ifndef _ZFI_ZFProtocolZFUIImageIO_h_
#define _ZFI_ZFProtocolZFUIImageIO_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFUIKit/ZFUIImage.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIImage.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFUIImage,
 *   the nativeImage described here should be same as
 *   the one in ZFUIImage's protocol
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFUIImageIO)
public:
    /**
     * @brief see #ZFUIImageScale
     *
     * returned image would be released by nativeImageRelease of ZFUIImage protocol
     * @note size is in pixel unit, you may revert it according imageScale
     */
    virtual void *imageApplyScale(ZF_IN zffloat imageScale,
                                  ZF_IN void *nativeImage,
                                  ZF_IN const ZFUISize &newSize,
                                  ZF_IN const ZFUIMargin &ninePatch) zfpurevirtual;
    /**
     * @brief see #ZFUIImageLoadInFrame
     *
     * returned image would be released by nativeImageRelease of ZFUIImage protocol
     * @note size is in pixel unit, you may revert it according imageScale
     */
    virtual void *imageLoadInFrame(ZF_IN zffloat imageScale,
                                   ZF_IN void *nativeImage,
                                   ZF_IN const ZFUIRect &frameInImage) zfpurevirtual;
    /**
     * @brief see #ZFUIImageLoadFromColor, size ensured valid
     *
     * returned image would be released by nativeImageRelease of ZFUIImage protocol
     * @note size is in pixel unit, you may revert it according imageScale
     */
    virtual void *imageLoadFromColor(ZF_IN zffloat imageScale,
                                     ZF_IN const ZFUIColor &color,
                                     ZF_IN const ZFUISize &size) zfpurevirtual;
ZFPROTOCOL_INTERFACE_END(ZFUIImageIO)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFUIImageIO_h_

