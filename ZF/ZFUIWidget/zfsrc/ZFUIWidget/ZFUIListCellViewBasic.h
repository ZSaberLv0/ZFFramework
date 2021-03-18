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
    ZFPROPERTY_OVERRIDE_ON_INIT_INLINE(ZFUIImageView *, cellIcon)
    {
        zfblockedAlloc(ZFUIImageView, cellIcon);
        propertyValue = cellIcon;
        cellIcon->viewSizeMax(ZFUISizeMake(ZFUIGlobalStyle::DefaultStyle()->itemSizeIcon()));
    }
    ZFPROPERTY_OVERRIDE_ON_ATTACH_INLINE(ZFUIImageView *, cellIcon)
    {
        this->cellIconContainer()->childAdd(this->cellIcon());
        this->cellIcon()->layoutParam()->layoutAlign(ZFUIAlign::e_Center);
    }
    ZFPROPERTY_OVERRIDE_ON_DETACH_INLINE(ZFUIImageView *, cellIcon)
    {
        this->cellIcon()->viewRemoveFromParent();
    }

    /**
     * @brief cell's main label
     */
    ZFPROPERTY_RETAIN_READONLY(ZFUITextView *, cellLabelMain, ZFPropertyNoInitValue)
    ZFPROPERTY_OVERRIDE_ON_INIT_INLINE(ZFUITextView *, cellLabelMain)
    {
        zfblockedAlloc(ZFUITextView, cellLabelMain);
        propertyValue = cellLabelMain;
    }
    ZFPROPERTY_OVERRIDE_ON_ATTACH_INLINE(ZFUITextView *, cellLabelMain)
    {
        this->cellLabelMainContainer()->childAdd(this->cellLabelMain());
        this->cellLabelMain()->layoutParam()->layoutAlign(ZFUIAlign::e_LeftInner);
    }
    ZFPROPERTY_OVERRIDE_ON_DETACH_INLINE(ZFUITextView *, cellLabelMain)
    {
        this->cellLabelMain()->viewRemoveFromParent();
    }

    /**
     * @brief cell's sub label
     */
    ZFPROPERTY_RETAIN_READONLY(ZFUITextView *, cellLabelSub, ZFPropertyNoInitValue)
    ZFPROPERTY_OVERRIDE_ON_INIT_INLINE(ZFUITextView *, cellLabelSub)
    {
        zfblockedAlloc(ZFUITextView, cellLabelSub);
        propertyValue = cellLabelSub;
        cellLabelSub->textColor(ZFUIGlobalStyle::DefaultStyle()->textColorSecondary());
        cellLabelSub->textSize(ZFUIGlobalStyle::DefaultStyle()->textSizeSmall());
    }
    ZFPROPERTY_OVERRIDE_ON_ATTACH_INLINE(ZFUITextView *, cellLabelSub)
    {
        this->cellLabelSubContainer()->childAdd(this->cellLabelSub());
        this->cellLabelSub()->layoutParam()->layoutAlign(ZFUIAlign::e_LeftInner);
    }
    ZFPROPERTY_OVERRIDE_ON_DETACH_INLINE(ZFUITextView *, cellLabelSub)
    {
        this->cellLabelSub()->viewRemoveFromParent();
    }

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

