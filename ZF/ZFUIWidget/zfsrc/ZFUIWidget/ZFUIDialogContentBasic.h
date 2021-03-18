/**
 * @file ZFUIDialogContentBasic.h
 * @brief basic content for #ZFUIDialogBasic
 */

#ifndef _ZFI_ZFUIDialogContentBasic_h_
#define _ZFI_ZFUIDialogContentBasic_h_

#include "ZFUIDialogBasic.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIDialogContentBasic
zfclassFwd _ZFP_ZFUIDialogContentBasicPrivate;
/**
 * @brief basic content for #ZFUIDialogBasic
 */
zfclass ZF_ENV_EXPORT ZFUIDialogContentBasic : zfextends ZFUIView, zfimplements ZFUIDialogContent
{
    ZFOBJECT_DECLARE(ZFUIDialogContentBasic, ZFUIView)
    ZFIMPLEMENTS_DECLARE(ZFUIDialogContent)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIDialogContentBasic)

public:
    ZFPROPERTY_OVERRIDE_ON_INIT_INLINE(ZFUISize, viewSizeMin)
    {
        propertyValue = ZFUISizeMake(
            ZFUIGlobalStyle::DefaultStyle()->itemSizeDialogWidth(),
            ZFUIGlobalStyle::DefaultStyle()->itemSizeDialog());
    }

    /**
     * @brief dialog title view
     */
    ZFPROPERTY_RETAIN_READONLY(ZFUITextView *, titleTextView, ZFPropertyNoInitValue)
    ZFPROPERTY_OVERRIDE_ON_INIT_INLINE(ZFUITextView *, titleTextView)
    {
        zfblockedAlloc(ZFUITextView, titleTextView);
        propertyValue = titleTextView;
        titleTextView->textSize(ZFUIGlobalStyle::DefaultStyle()->textSizeBig());
        titleTextView->textAppearance(ZFUITextAppearance::e_Bold);
    }

    /**
     * @brief dialog content view
     */
    ZFPROPERTY_RETAIN_READONLY(ZFUITextView *, contentTextView, ZFPropertyNoInitValue)
    ZFPROPERTY_OVERRIDE_ON_INIT_INLINE(ZFUITextView *, contentTextView)
    {
        zfblockedAlloc(ZFUITextView, contentTextView);
        propertyValue = contentTextView;
        contentTextView->textSingleLine(zffalse);
    }

public:
    // ============================================================
    // title
    zfoverride
    virtual ZFUIView *dialogTitleContainer(void);
    zfoverride
    virtual ZFUITextView *dialogTitleView(void);

    // ============================================================
    // content
    zfoverride
    virtual ZFUIView *dialogContentContainer(void);
    zfoverride
    virtual ZFUITextView *dialogContentView(void);

    // ============================================================
    // button
    zfoverride
    virtual ZFUIView *dialogButtonContainer(void);
    zfoverride
    virtual ZFUIButton *dialogButton(ZF_IN ZFUIDialogButtonTypeEnum dialogButtonType,
                                     ZF_IN_OPT zfbool autoCreateIfNotExist = zftrue);
    zfoverride
    virtual const zfchar *dialogButtonText(ZF_IN ZFUIDialogButtonTypeEnum dialogButtonType);
    zfoverride
    virtual void dialogButtonText(ZF_IN ZFUIDialogButtonTypeEnum dialogButtonType,
                                  ZF_IN const zfchar *text);
    zfoverride
    virtual void dialogButtonRemove(ZF_IN ZFUIDialogButtonTypeEnum dialogButtonType);

    // ============================================================
    // button
    zfoverride
    virtual zfindex dialogButtonCount(void);
    zfoverride
    virtual ZFUIButton *dialogButtonAtIndex(ZF_IN zfindex index);
    zfoverride
    virtual zfindex dialogButtonFind(ZF_IN ZFUIButton *dialogButton);
    zfoverride
    virtual void dialogButtonAdd(ZF_IN ZFUIButton *button,
                                 ZF_IN_OPT zfindex atIndex = zfindexMax());
    zfoverride
    virtual void dialogButtonRemove(ZF_IN ZFUIButton *button);
    zfoverride
    virtual void dialogButtonRemoveAtIndex(ZF_IN zfindex index);
    zfoverride
    virtual void dialogButtonRemoveAll(void);

    // ============================================================
protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);

    zfoverride
    virtual void layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                 ZF_IN const ZFUISize &sizeHint,
                                 ZF_IN const ZFUISizeParam &sizeParam);
    zfoverride
    virtual void internalFgViewOnLayout(ZF_IN const ZFUIRect &bounds);
    zfoverride
    virtual zfbool internalViewShouldLayout(ZF_IN ZFUIView *internalView);

private:
    _ZFP_ZFUIDialogContentBasicPrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIDialogContentBasic_h_

