/**
 * @file ZFUIButtonBasic.h
 * @brief basic button
 */

#ifndef _ZFI_ZFUIButtonBasic_h_
#define _ZFI_ZFUIButtonBasic_h_

#include "ZFUIButton.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIButtonBasic
zfclassFwd _ZFP_ZFUIButtonBasicPrivate;
/**
 * @brief basic button, see #ZFUIButton
 *
 * a basic button contains:
 * -  label: to show button's text
 * -  icon: to show button's icon
 * -  background: to show button's background
 *
 * to change the button's property,
 * you must use ZFUIButtonBasic's styleable property,
 * such as #ZFUIButtonBasic::buttonLabelStyleNormal\n
 * \n
 * specially, those property's normal state's value would be used
 * if other state doesn't supply value:
 * -  label's #ZFUITextView::text
 * -  icon's #ZFUIImageView::image
 * -  background's #ZFUIImageView::image
 *
 * @note button's components (label, icon, background) would be created
 *   only if accessed or text/image has been set,
 *   or whenever you called #prepareButtonLabel/#prepareButtonIcon/#prepareButtonBackground
 */
zfclass ZF_ENV_EXPORT ZFUIButtonBasic : zfextends ZFUIButton
{
    ZFOBJECT_DECLARE(ZFUIButtonBasic, ZFUIButton)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIButtonBasic)

public:
    // ============================================================
    // content layout
    /**
     * @brief button icon's orientation (relative to label), #ZFUIOrientation::e_Left by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIOrientationEnum, buttonContentIconPosition, ZFUIOrientation::e_Left)
    /**
     * @brief button content(icon, label)'s layout align, #ZFUIAlign::e_Center by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIAlignFlags, buttonContentAlign, ZFUIAlign::e_Center)
    /**
     * @brief button content(icon, label)'s layout margin, #ZFUIGlobalStyle::itemMargin by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIMargin, buttonContentMargin, ZFUIMarginMake(ZFUIGlobalStyle::DefaultStyle()->itemMargin()))
    /**
     * @brief button icon and label's space, #ZFUIGlobalStyle::itemSpace by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zffloat, buttonContentSpace, ZFUIGlobalStyle::DefaultStyle()->itemSpace())

    /**
     * @brief button background's layout margin, #ZFUIMarginZero by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(ZFUIMargin, buttonBackgroundMargin, ZFUIMarginZero())

    // ============================================================
    // label
    /** @brief label style in normal state */
    ZFPROPERTY_RETAIN_READONLY(ZFUITextView *, buttonLabelStyleNormal,
                               zflineAlloc(ZFUITextView))
    ZFPROPERTY_ON_INIT_DECLARE(ZFUITextView *, buttonLabelStyleNormal)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUITextView *, buttonLabelStyleNormal)
    ZFPROPERTY_ON_DETACH_DECLARE(ZFUITextView *, buttonLabelStyleNormal)

    /** @brief label style in highlighted state */
    ZFPROPERTY_RETAIN_READONLY(ZFUITextView *, buttonLabelStyleHighlighted,
                               zflineAlloc(ZFUITextView))
    ZFPROPERTY_ON_INIT_DECLARE(ZFUITextView *, buttonLabelStyleHighlighted)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUITextView *, buttonLabelStyleHighlighted)
    ZFPROPERTY_ON_DETACH_DECLARE(ZFUITextView *, buttonLabelStyleHighlighted)

    /** @brief label style in checked state */
    ZFPROPERTY_RETAIN_READONLY(ZFUITextView *, buttonLabelStyleChecked,
                               zflineAlloc(ZFUITextView))
    ZFPROPERTY_ON_INIT_DECLARE(ZFUITextView *, buttonLabelStyleChecked)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUITextView *, buttonLabelStyleChecked)
    ZFPROPERTY_ON_DETACH_DECLARE(ZFUITextView *, buttonLabelStyleChecked)

    /** @brief label style in checked highlighted state */
    ZFPROPERTY_RETAIN_READONLY(ZFUITextView *, buttonLabelStyleCheckedHighlighted,
                               zflineAlloc(ZFUITextView))
    ZFPROPERTY_ON_INIT_DECLARE(ZFUITextView *, buttonLabelStyleCheckedHighlighted)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUITextView *, buttonLabelStyleCheckedHighlighted)
    ZFPROPERTY_ON_DETACH_DECLARE(ZFUITextView *, buttonLabelStyleCheckedHighlighted)

    /** @brief label style in disabled state */
    ZFPROPERTY_RETAIN_READONLY(ZFUITextView *, buttonLabelStyleDisabled,
                               zflineAlloc(ZFUITextView))
    ZFPROPERTY_ON_INIT_DECLARE(ZFUITextView *, buttonLabelStyleDisabled)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUITextView *, buttonLabelStyleDisabled)
    ZFPROPERTY_ON_DETACH_DECLARE(ZFUITextView *, buttonLabelStyleDisabled)

    // ============================================================
    // icon
    /** @brief icon style in normal state */
    ZFPROPERTY_RETAIN_READONLY(ZFUIImageView *, buttonIconStyleNormal,
                               zflineAlloc(ZFUIImageView))
    ZFPROPERTY_ON_INIT_DECLARE(ZFUIImageView *, buttonIconStyleNormal)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIImageView *, buttonIconStyleNormal)
    ZFPROPERTY_ON_DETACH_DECLARE(ZFUIImageView *, buttonIconStyleNormal)

    /** @brief icon style in highlighted state */
    ZFPROPERTY_RETAIN_READONLY(ZFUIImageView *, buttonIconStyleHighlighted,
                               zflineAlloc(ZFUIImageView))
    ZFPROPERTY_ON_INIT_DECLARE(ZFUIImageView *, buttonIconStyleHighlighted)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIImageView *, buttonIconStyleHighlighted)
    ZFPROPERTY_ON_DETACH_DECLARE(ZFUIImageView *, buttonIconStyleHighlighted)

    /** @brief icon style in checked state */
    ZFPROPERTY_RETAIN_READONLY(ZFUIImageView *, buttonIconStyleChecked,
                               zflineAlloc(ZFUIImageView))
    ZFPROPERTY_ON_INIT_DECLARE(ZFUIImageView *, buttonIconStyleChecked)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIImageView *, buttonIconStyleChecked)
    ZFPROPERTY_ON_DETACH_DECLARE(ZFUIImageView *, buttonIconStyleChecked)

    /** @brief icon style in checked highlighted state */
    ZFPROPERTY_RETAIN_READONLY(ZFUIImageView *, buttonIconStyleCheckedHighlighted,
                               zflineAlloc(ZFUIImageView))
    ZFPROPERTY_ON_INIT_DECLARE(ZFUIImageView *, buttonIconStyleCheckedHighlighted)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIImageView *, buttonIconStyleCheckedHighlighted)
    ZFPROPERTY_ON_DETACH_DECLARE(ZFUIImageView *, buttonIconStyleCheckedHighlighted)

    /** @brief icon style in disabled state */
    ZFPROPERTY_RETAIN_READONLY(ZFUIImageView *, buttonIconStyleDisabled,
                               zflineAlloc(ZFUIImageView))
    ZFPROPERTY_ON_INIT_DECLARE(ZFUIImageView *, buttonIconStyleDisabled)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIImageView *, buttonIconStyleDisabled)
    ZFPROPERTY_ON_DETACH_DECLARE(ZFUIImageView *, buttonIconStyleDisabled)

    // ============================================================
    // background
    /** @brief background style in normal state */
    ZFPROPERTY_RETAIN_READONLY(ZFUIImageView *, buttonBackgroundStyleNormal,
                               zflineAlloc(ZFUIImageView))
    ZFPROPERTY_ON_INIT_DECLARE(ZFUIImageView *, buttonBackgroundStyleNormal)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIImageView *, buttonBackgroundStyleNormal)
    ZFPROPERTY_ON_DETACH_DECLARE(ZFUIImageView *, buttonBackgroundStyleNormal)

    /** @brief background style in highlighted state */
    ZFPROPERTY_RETAIN_READONLY(ZFUIImageView *, buttonBackgroundStyleHighlighted,
                               zflineAlloc(ZFUIImageView))
    ZFPROPERTY_ON_INIT_DECLARE(ZFUIImageView *, buttonBackgroundStyleHighlighted)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIImageView *, buttonBackgroundStyleHighlighted)
    ZFPROPERTY_ON_DETACH_DECLARE(ZFUIImageView *, buttonBackgroundStyleHighlighted)

    /** @brief background style in checked state */
    ZFPROPERTY_RETAIN_READONLY(ZFUIImageView *, buttonBackgroundStyleChecked,
                               zflineAlloc(ZFUIImageView))
    ZFPROPERTY_ON_INIT_DECLARE(ZFUIImageView *, buttonBackgroundStyleChecked)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIImageView *, buttonBackgroundStyleChecked)
    ZFPROPERTY_ON_DETACH_DECLARE(ZFUIImageView *, buttonBackgroundStyleChecked)

    /** @brief background style in checked highlighted state */
    ZFPROPERTY_RETAIN_READONLY(ZFUIImageView *, buttonBackgroundStyleCheckedHighlighted,
                               zflineAlloc(ZFUIImageView))
    ZFPROPERTY_ON_INIT_DECLARE(ZFUIImageView *, buttonBackgroundStyleCheckedHighlighted)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIImageView *, buttonBackgroundStyleCheckedHighlighted)
    ZFPROPERTY_ON_DETACH_DECLARE(ZFUIImageView *, buttonBackgroundStyleCheckedHighlighted)

    /** @brief background style in disabled state */
    ZFPROPERTY_RETAIN_READONLY(ZFUIImageView *, buttonBackgroundStyleDisabled,
                               zflineAlloc(ZFUIImageView))
    ZFPROPERTY_ON_INIT_DECLARE(ZFUIImageView *, buttonBackgroundStyleDisabled)
    ZFPROPERTY_ON_ATTACH_DECLARE(ZFUIImageView *, buttonBackgroundStyleDisabled)
    ZFPROPERTY_ON_DETACH_DECLARE(ZFUIImageView *, buttonBackgroundStyleDisabled)

    // ============================================================
    // util method
public:
    /**
     * @brief util method to get button label's style
     */
    ZFMETHOD_DECLARE_1(ZFUITextView *, buttonLabelStyle,
                       ZFMP_IN_OPT(ZFUIButtonStateEnum, forState, ZFUIButtonState::e_Normal))
    /**
     * @brief util method to get button icon's style
     */
    ZFMETHOD_DECLARE_1(ZFUIImageView *, buttonIconStyle,
                       ZFMP_IN_OPT(ZFUIButtonStateEnum, forState, ZFUIButtonState::e_Normal))
    /**
     * @brief util method to get button icon's style
     */
    ZFMETHOD_DECLARE_1(ZFUIImageView *, buttonBackgroundStyle,
                       ZFMP_IN_OPT(ZFUIButtonStateEnum, forState, ZFUIButtonState::e_Normal))

    /**
     * @brief util method to get button label's text
     */
    ZFMETHOD_DECLARE_1(const zfchar *, buttonLabelText,
                       ZFMP_IN_OPT(ZFUIButtonStateEnum, forState, ZFUIButtonState::e_Normal))
    /**
     * @brief util method to set button label's text
     */
    ZFMETHOD_DECLARE_2(void, buttonLabelText,
                       ZFMP_IN(const zfchar *, text),
                       ZFMP_IN_OPT(ZFUIButtonStateEnum, forState, ZFUIButtonState::e_Normal))

    /**
     * @brief util method to get button icon's image
     */
    ZFMETHOD_DECLARE_1(ZFUIImage *, buttonIconImage,
                       ZFMP_IN_OPT(ZFUIButtonStateEnum, forState, ZFUIButtonState::e_Normal))
    /**
     * @brief util method to set button icon's image
     */
    ZFMETHOD_DECLARE_2(void, buttonIconImage,
                       ZFMP_IN(ZFUIImage *, image),
                       ZFMP_IN_OPT(ZFUIButtonStateEnum, forState, ZFUIButtonState::e_Normal))

    /**
     * @brief util method to get button background's image
     */
    ZFMETHOD_DECLARE_1(ZFUIImage *, buttonBackgroundImage,
                       ZFMP_IN_OPT(ZFUIButtonStateEnum, forState, ZFUIButtonState::e_Normal))
    /**
     * @brief util method to set button background's image
     */
    ZFMETHOD_DECLARE_2(void, buttonBackgroundImage,
                       ZFMP_IN(ZFUIImage *, image),
                       ZFMP_IN_OPT(ZFUIButtonStateEnum, forState, ZFUIButtonState::e_Normal))

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);

    zfoverride
    virtual void layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                 ZF_IN const ZFUISize &sizeHint,
                                 ZF_IN const ZFUISizeParam &sizeParam);
    zfoverride
    virtual void internalViewOnLayout(ZF_IN const ZFUIRect &bounds);
    zfoverride
    virtual zfbool internalViewShouldLayout(ZF_IN ZFUIView *internalView);

    zfoverride
    virtual void buttonStateOnUpdate(void);

protected:
    /**
     * @brief must be type of #ZFUITextView, by default it's #ZFUITextView
     */
    virtual const ZFClass *buttonLabelClass(void);
    /**
     * @brief must be type of #ZFUIImageView, by default it's #ZFUIImageView
     */
    virtual const ZFClass *buttonIconClass(void);
    /**
     * @brief must be type of #ZFUIImageView, by default it's #ZFUIImageView
     */
    virtual const ZFClass *buttonBackgroundClass(void);

public:
    /**
     * @brief button's label view, style of this view must be changed by #ZFUIButtonBasic::buttonLabelStyleNormal, etc
     *
     * @note property may not updated immediately,
     *   if really want to update manually,
     *   use #ZFUIButton::buttonStateUpdate
     */
    ZFMETHOD_DECLARE_0(ZFUITextView *, buttonLabel)
    /**
     * @brief button's icon view, style of this view must be changed by #ZFUIButtonBasic::buttonIconStyleNormal, etc
     *
     * @note property may not updated immediately,
     *   if really want to update manually,
     *   use #ZFUIButton::buttonStateUpdate
     */
    ZFMETHOD_DECLARE_0(ZFUIImageView *, buttonIcon)
    /**
     * @brief button's background view, style of this view must be changed by #ZFUIButtonBasic::buttonBackgroundStyleNormal, etc
     *
     * @note property may not updated immediately,
     *   if really want to update manually,
     *   use #ZFUIButton::buttonStateUpdate
     */
    ZFMETHOD_DECLARE_0(ZFUIImageView *, buttonBackground)

public:
    /**
     * @brief prepare button label
     *
     * by default, button component (label/icon/background) won't be created until it would be accessed,
     * either by access them directly by #buttonLabel,
     * or change #buttonLabelStyle's property\n
     * you may manually call this method to create the button component, if necessary
     */
    ZFMETHOD_DECLARE_0(void, prepareButtonLabel)
    /**
     * @brief see #prepareButtonLabel
     */
    ZFMETHOD_DECLARE_0(void, prepareButtonIcon)
    /**
     * @brief see #prepareButtonLabel
     */
    ZFMETHOD_DECLARE_0(void, prepareButtonBackground)

private:
    _ZFP_ZFUIButtonBasicPrivate *d;
    friend zfclassFwd _ZFP_ZFUIButtonBasicPrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIButtonBasic_h_

