/**
 * @file ZFUIDialogContentBasic_Buttons.h
 * @brief buttons for #ZFUIDialogContentBasic
 */

#ifndef _ZFI_ZFUIDialogContentBasic_Buttons_h_
#define _ZFI_ZFUIDialogContentBasic_Buttons_h_

#include "ZFUIDialogBasic.h"
#include "ZFUIButtonBasic.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIDialogButtonType::e_Yes
/** @brief #ZFUIDialogButtonType::e_Yes for #ZFUIDialogContentBasic */
zfclass ZF_ENV_EXPORT ZFUIDialogContentBasicButtonYes : zfextends ZFUIButtonBasic
{
    ZFOBJECT_DECLARE(ZFUIDialogContentBasicButtonYes, ZFUIButtonBasic)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIDialogContentBasicButtonYes)
    ZFPROPERTY_ON_INIT_INLINE(ZFUISize, viewSizeMin)
    {
        propertyValue = ZFUISizeMake(
            ZFUIGlobalStyle::DefaultStyle()->itemSizeButtonWidth(),
            ZFUIGlobalStyle::DefaultStyle()->itemSizeButton());
    }
};

// ============================================================
// ZFUIDialogButtonType::e_No
/** @brief #ZFUIDialogButtonType::e_No for #ZFUIDialogContentBasic */
zfclass ZF_ENV_EXPORT ZFUIDialogContentBasicButtonNo : zfextends ZFUIButtonBasic
{
    ZFOBJECT_DECLARE(ZFUIDialogContentBasicButtonNo, ZFUIButtonBasic)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIDialogContentBasicButtonNo)
    ZFPROPERTY_ON_INIT_INLINE(ZFUISize, viewSizeMin)
    {
        propertyValue = ZFUISizeMake(
            ZFUIGlobalStyle::DefaultStyle()->itemSizeButtonWidth(),
            ZFUIGlobalStyle::DefaultStyle()->itemSizeButton());
    }
};

// ============================================================
// ZFUIDialogButtonType::e_Cancel
/** @brief #ZFUIDialogButtonType::e_Cancel for #ZFUIDialogContentBasic */
zfclass ZF_ENV_EXPORT ZFUIDialogContentBasicButtonCancel : zfextends ZFUIButtonBasic
{
    ZFOBJECT_DECLARE(ZFUIDialogContentBasicButtonCancel, ZFUIButtonBasic)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIDialogContentBasicButtonCancel)
    ZFPROPERTY_ON_INIT_INLINE(ZFUISize, viewSizeMin)
    {
        propertyValue = ZFUISizeMake(
            ZFUIGlobalStyle::DefaultStyle()->itemSizeButtonWidth(),
            ZFUIGlobalStyle::DefaultStyle()->itemSizeButton());
    }
};

// ============================================================
// ZFUIDialogButtonType::e_Destructive
/** @brief #ZFUIDialogButtonType::e_Destructive for #ZFUIDialogContentBasic */
zfclass ZF_ENV_EXPORT ZFUIDialogContentBasicButtonDestructive : zfextends ZFUIButtonBasic
{
    ZFOBJECT_DECLARE(ZFUIDialogContentBasicButtonDestructive, ZFUIButtonBasic)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIDialogContentBasicButtonDestructive)
    ZFPROPERTY_ON_INIT_INLINE(ZFUISize, viewSizeMin)
    {
        propertyValue = ZFUISizeMake(
            ZFUIGlobalStyle::DefaultStyle()->itemSizeButtonWidth(),
            ZFUIGlobalStyle::DefaultStyle()->itemSizeButton());
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIDialogContentBasic_Buttons_h_

