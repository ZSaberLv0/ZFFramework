/**
 * @file ZFUIButtonGroupBasic.h
 * @brief abstract button group management
 */

#ifndef _ZFI_ZFUIButtonGroupBasic_h_
#define _ZFI_ZFUIButtonGroupBasic_h_

#include "ZFUIButtonGroup.h"
#include "ZFUILinearLayout.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIButtonGroupBasic
/**
 * @brief basic button group which layout buttons as #ZFUILinearLayout
 */
zfclass ZF_ENV_EXPORT ZFUIButtonGroupBasic : zfextends ZFUILinearLayout, zfimplements ZFUIButtonGroup
{
    ZFOBJECT_DECLARE(ZFUIButtonGroupBasic, ZFUILinearLayout)
    ZFIMPLEMENTS_DECLARE(ZFUIButtonGroup)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIButtonGroupBasic)

protected:
    zfoverride
    virtual inline void buttonOnAdd(ZF_IN ZFUIButton *button,
                                    ZF_IN zfindex buttonIndex)
    {
        this->childAdd(button, zfnull, buttonIndex);
        zfsuperI(ZFUIButtonGroup)::buttonOnAdd(button, buttonIndex);
    }
    zfoverride
    virtual inline void buttonOnRemove(ZF_IN ZFUIButton *button,
                                       ZF_IN zfindex buttonIndex)
    {
        this->childRemoveAtIndex(buttonIndex);
        zfsuperI(ZFUIButtonGroup)::buttonOnRemove(button, buttonIndex);
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIButtonGroupBasic_h_

