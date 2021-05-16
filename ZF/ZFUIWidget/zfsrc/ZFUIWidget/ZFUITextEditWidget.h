/**
 * @file ZFUITextEditWidget.h
 * @brief basic text edit widget
 */

#ifndef _ZFI_ZFUITextEditWidget_h_
#define _ZFI_ZFUITextEditWidget_h_

#include "ZFUIButtonBasic.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUITextEditWidget
/**
 * @brief basic text edit widget
 */
zfclass ZF_ENV_EXPORT ZFUITextEditWidget : zfextends ZFUITextEdit
{
    ZFOBJECT_DECLARE(ZFUITextEditWidget, ZFUITextEdit)
    ZFSTYLE_DEFAULT_DECLARE(ZFUITextEditWidget)

public:
    // ============================================================
    // properties
    /**
     * @brief the background view
     *
     * note, image's nine patch would be added to #ZFUIView::nativeImplViewMargin
     */
    ZFPROPERTY_RETAIN_READONLY(ZFUIImageView *, textEditBackgroundView, ZFPropertyNoInitValue)
    ZFPROPERTY_ON_INIT_DECLARE(ZFUIImageView *, textEditBackgroundView)

    /**
     * @brief the clear button
     *
     * note, the clear button's size would be added to #ZFUIView::nativeImplViewMargin
     */
    ZFPROPERTY_RETAIN_READONLY(ZFUIButtonBasic *, textEditClearButton, ZFPropertyNoInitValue)
    ZFPROPERTY_ON_INIT_DECLARE(ZFUIButtonBasic *, textEditClearButton)

    /**
     * @brief whether automatically show or hide clear button, false by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, textEditClearButtonAutoEnable, zffalse)
    ZFPROPERTY_ON_ATTACH_DECLARE(zfbool, textEditClearButtonAutoEnable)

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

protected:
    zfoverride
    virtual void layoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds);
    zfoverride
    virtual void nativeImplViewMarginImplUpdate(ZF_IN_OUT ZFUIMargin &nativeImplViewMargin);
    zfoverride
    virtual zfbool internalViewShouldLayout(ZF_IN ZFUIView *internalView);
    zfoverride
    virtual void internalViewOnLayout(ZF_IN const ZFUIRect &bounds);
    zfoverride
    virtual void textOnChange(ZF_IN const zfchar *oldText);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUITextEditWidget_h_

