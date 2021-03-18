/**
 * @file ZFProgressable.h
 * @brief object that can be updated according to progress
 */

#ifndef _ZFI_ZFProgressable_h_
#define _ZFI_ZFProgressable_h_

#include "ZFObjectCore.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFProgressable
/**
 * @brief object that can be updated according to progress
 */
zfinterface ZF_ENV_EXPORT ZFProgressable : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE(ZFProgressable, ZFInterface)

public:
    /** @brief see #ZFProgressable */
    virtual zfbool progressUpdate(ZF_IN ZFProgressable *from,
                                  ZF_IN ZFProgressable *to,
                                  ZF_IN zffloat progress) zfpurevirtual;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProgressable_h_

