/**
 * @file ZFUIButtonRatio.h
 * @brief check box button
 */

#ifndef _ZFI_ZFUIButtonRatio_h_
#define _ZFI_ZFUIButtonRatio_h_

#include "ZFUIButtonBasic.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIButtonRatio
/**
 * @brief check box button, see #ZFUIButtonBasic
 */
zfclass ZF_ENV_EXPORT ZFUIButtonRatio : zfextends ZFUIButtonBasic
{
    ZFOBJECT_DECLARE(ZFUIButtonRatio, ZFUIButtonBasic)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIButtonRatio)

public:
    ZFPROPERTY_OVERRIDE_ON_INIT_DECLARE(zfbool, buttonCheckable)
    ZFPROPERTY_OVERRIDE_ON_INIT_DECLARE(ZFUIAlignFlags, buttonContentAlign)
    ZFPROPERTY_OVERRIDE_ON_INIT_DECLARE(ZFUIImageView *, buttonIconStyleNormal)
    ZFPROPERTY_OVERRIDE_ON_INIT_DECLARE(ZFUIImageView *, buttonIconStyleHighlighted)
    ZFPROPERTY_OVERRIDE_ON_INIT_DECLARE(ZFUIImageView *, buttonIconStyleChecked)
    ZFPROPERTY_OVERRIDE_ON_INIT_DECLARE(ZFUIImageView *, buttonIconStyleCheckedHighlighted)
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIButtonRatio_h_

