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
    ZFPROPERTY_OVERRIDE_ON_INIT_INLINE(zfbool, buttonCheckable)
    {
        propertyValue = zftrue;
    }

    ZFPROPERTY_OVERRIDE_ON_INIT_INLINE(ZFUIAlignFlags, buttonContentAlign)
    {
        propertyValue = ZFUIAlign::e_LeftInner;
    }

    ZFPROPERTY_OVERRIDE_ON_INIT_INLINE(ZFUIImageView *, buttonIconStyleNormal)
    {
        propertyValue.to<ZFUIImageView *>()->image(zfRes("ZFUIWidget/ZFUIButtonRatio_IconNormal.png"));
    }
    ZFPROPERTY_OVERRIDE_ON_INIT_INLINE(ZFUIImageView *, buttonIconStyleHighlighted)
    {
        propertyValue.to<ZFUIImageView *>()->image(zfRes("ZFUIWidget/ZFUIButtonRatio_IconHighlighted.png"));
    }
    ZFPROPERTY_OVERRIDE_ON_INIT_INLINE(ZFUIImageView *, buttonIconStyleChecked)
    {
        propertyValue.to<ZFUIImageView *>()->image(zfRes("ZFUIWidget/ZFUIButtonRatio_IconChecked.png"));
    }
    ZFPROPERTY_OVERRIDE_ON_INIT_INLINE(ZFUIImageView *, buttonIconStyleCheckedHighlighted)
    {
        propertyValue.to<ZFUIImageView *>()->image(zfRes("ZFUIWidget/ZFUIButtonRatio_IconCheckedHighlighted.png"));
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIButtonRatio_h_

