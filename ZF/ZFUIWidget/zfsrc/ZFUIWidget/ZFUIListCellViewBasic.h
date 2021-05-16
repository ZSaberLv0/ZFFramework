/**
 * @file ZFUIListCellViewBasic.h
 * @brief basic list cell view with icon and label
 */

#ifndef _ZFI_ZFUIListCellViewBasic_h_
#define _ZFI_ZFUIListCellViewBasic_h_

#include "ZFUIListCellView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIListCellViewBasic
zfclassFwd _ZFP_ZFUIListCellViewBasicPrivate;
/**
 * @brief basic list cell view with icon and label
 */
zfclass ZF_ENV_EXPORT ZFUIListCellViewBasic : zfextends ZFUIListCellView
{
    ZFOBJECT_DECLARE(ZFUIListCellViewBasic, ZFUIListCellView)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIListCellViewBasic)

public:
    // ============================================================
    // properties
    /**
     * @brief cell's icon
     */
    ZFPROPERTY_RETAIN_READONLY(ZFUIImageView *, cellIcon, ZFPropertyNoInitValue)
    ZFPROPERTY_ON_INIT_DECLARE(ZFUIImageView *, cellIcon)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIImageView *, cellIcon)
    ZFPROPERTY_ON_DETACH_DECLARE(ZFUIImageView *, cellIcon)

    /**
     * @brief cell's main label
     */
    ZFPROPERTY_RETAIN_READONLY(ZFUITextView *, cellLabelMain, ZFPropertyNoInitValue)
    ZFPROPERTY_ON_INIT_DECLARE(ZFUITextView *, cellLabelMain)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUITextView *, cellLabelMain)
    ZFPROPERTY_ON_DETACH_DECLARE(ZFUITextView *, cellLabelMain)

    /**
     * @brief cell's sub label
     */
    ZFPROPERTY_RETAIN_READONLY(ZFUITextView *, cellLabelSub, ZFPropertyNoInitValue)
    ZFPROPERTY_ON_INIT_DECLARE(ZFUITextView *, cellLabelSub)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUITextView *, cellLabelSub)
    ZFPROPERTY_ON_DETACH_DECLARE(ZFUITextView *, cellLabelSub)

public:
    /**
     * @brief container for icon, you may add your own custom view
     */
    ZFMETHOD_DECLARE_0(ZFUIView *, cellIconContainer)
    /**
     * @brief container for center item, you may add your own custom view
     */
    ZFMETHOD_DECLARE_0(ZFUIView *, cellCenterContainer)
    /**
     * @brief container for main label, you may add your own custom view
     */
    ZFMETHOD_DECLARE_0(ZFUIView *, cellLabelMainContainer)
    /**
     * @brief container for sub label, you may add your own custom view
     */
    ZFMETHOD_DECLARE_0(ZFUIView *, cellLabelSubContainer)
    /**
     * @brief container for accessory, you may add your own custom view
     */
    ZFMETHOD_DECLARE_0(ZFUIView *, cellAccessoryContainer)

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

private:
    _ZFP_ZFUIListCellViewBasicPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIListCellViewBasic_h_

