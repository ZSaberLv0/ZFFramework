/**
 * @file ZFUITextView.h
 * @brief view to display plain text
 */

#ifndef _ZFI_ZFUITextView_h_
#define _ZFI_ZFUITextView_h_

#include "ZFUIView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUITextView
/**
 * @brief view to display plain text
 *
 * for editable or selectable text view, you should use #ZFUITextView,
 * for rich text, you should supply you own native view,
 * or use web view with html strings
 */
zfclass ZF_ENV_EXPORT ZFUITextView : zfextends ZFUIView
{
    ZFOBJECT_DECLARE(ZFUITextView, ZFUIView)
    ZFSTYLE_DEFAULT_DECLARE(ZFUITextView)

public:
    // ============================================================
    // properties
    ZFPROPERTY_OVERRIDE_ON_INIT_INLINE(zfbool, viewUIEnable)
    {
        propertyValue = zffalse;
    }
    ZFPROPERTY_OVERRIDE_ON_INIT_INLINE(zfbool, viewUIEnableTree)
    {
        propertyValue = zffalse;
    }

public:
    /**
     * @brief text, may be null if not set
     */
    ZFPROPERTY_ASSIGN(zfstring, text)
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(zfstring, text)

    /**
     * @brief text appearance, #ZFUIGlobalStyle::textAppearance by default
     *
     * note, for some implementations and font settings, italic or bold may or may not be supported
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUITextAppearanceEnum, textAppearance,
                                ZFUIGlobalStyle::DefaultStyle()->textAppearance())
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(ZFUITextAppearanceEnum, textAppearance)

    /**
     * @brief text alignment, #ZFUIGlobalStyle::textAlign by default
     *
     * @note only LeftInner, Center, RightInner would be supported for sure
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIAlignFlags, textAlign,
                                ZFUIGlobalStyle::DefaultStyle()->textAlign())
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(ZFUIAlignFlags, textAlign)

    /**
     * @brief text color, #ZFUIGlobalStyle::textColorDefault by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIColor, textColor,
                                ZFUIGlobalStyle::DefaultStyle()->textColorDefault())
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(ZFUIColor, textColor)

    /**
     * @brief text shadow color, #ZFUIColorZero by default, use transparent to disable text shadow
     *
     * note that implementation may have no text shadow support
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIColor, textShadowColor,
                                ZFUIColorZero())
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(ZFUIColor, textShadowColor)

    /**
     * @brief text shadow offset, (1, 1) by default
     *
     * note that implementation may have no text shadow support
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUISize, textShadowOffset,
                                (ZFUISizeMake(1, 1)))
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(ZFUISize, textShadowOffset)

    /**
     * @brief text size in pixel, #ZFUIGlobalStyle::textSizeNormal by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, textSize,
                                ZFUIGlobalStyle::DefaultStyle()->textSizeNormal())
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(zfint, textSize)

    /**
     * @brief if not 0, auto decrease text size
     *   when the view isn't big enough to hold current text,
     *   #ZFUIGlobalStyle::textSizeTiny by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, textSizeAutoChangeMinSize,
                                ZFUIGlobalStyle::DefaultStyle()->textSizeTiny())
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(zfint, textSizeAutoChangeMinSize)
    /**
     * @brief if not 0 and larger than #textSizeAutoChangeMinSize,
     *   auto increase text size when the view is bigger than current text need,
     *   0 by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfint, textSizeAutoChangeMaxSize,
                                0)
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(zfint, textSizeAutoChangeMaxSize)

    /**
     * @brief single line or not, true by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, textSingleLine,
                                zftrue)
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(zfbool, textSingleLine)

    /**
     * @brief text truncate mode, #ZFUITextTruncateMode::e_Disable by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUITextTruncateModeEnum, textTruncateMode,
                                ZFUIGlobalStyle::DefaultStyle()->textTruncateMode())
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(ZFUITextTruncateModeEnum, textTruncateMode)

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnInitFinish(void);

    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);

public:
    /**
     * @brief measure text view
     *
     * this is separate from UIView's measure step,
     * usually called by app to measure text's size,
     * and usually have no need to call\n
     * sizeHint shows max size the text view may have, or 0 if no limit
     */
    ZFMETHOD_DECLARE_2(void, measureTextView,
                       ZFMP_OUT(ZFUISize &, ret),
                       ZFMP_IN_OPT(const ZFUISize &, sizeHint, ZFUISizeZero()))

    /**
     * @brief get current text size
     *
     * if the view hasn't been layouted, the size may be invalid
     */
    ZFMETHOD_DECLARE_0(zfint, textSizeCurrent)

    // ============================================================
    // override
protected:
    /**
     * @brief we would update text settings when scale changed
     */
    zfoverride
    virtual void scaleOnChange(void);
    /**
     * @brief we would measure according text size
     */
    zfoverride
    virtual void layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                 ZF_IN const ZFUISize &sizeHint,
                                 ZF_IN const ZFUISizeParam &sizeParam);
    zfoverride
    virtual void internalImplViewOnLayout(ZF_IN const ZFUIRect &bounds);
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUITextView_h_

