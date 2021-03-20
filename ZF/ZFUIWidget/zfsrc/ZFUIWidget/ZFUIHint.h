/**
 * @file ZFUIHint.h
 * @brief a quick UI hint to show short messages
 */

#ifndef _ZFI_ZFUIHint_h_
#define _ZFI_ZFUIHint_h_

#include "ZFUIWidgetDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief default hint duration, 1500 by default
 */
ZFEXPORT_VAR_DECLARE(zftimet, ZFUIHintDurationDefault)

// ============================================================
zfclassFwd _ZFP_ZFUIHintPrivate;
/**
 * @brief a quick UI hint to show short messages
 *
 * @note can only be created or show or hide in main thread
 */
zfclass ZF_ENV_EXPORT ZFUIHint : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFUIHint, ZFObject)

public:
    // ============================================================
    // events
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called to init the hint object,
     * typically used for one-time init for each hint object,
     * to update app-wide default setting
     */
    ZFOBSERVER_EVENT(HintOnInit)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called to update the hint object before show (each time)
     */
    ZFOBSERVER_EVENT(HintOnUpdate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when the hint showed
     */
    ZFOBSERVER_EVENT(HintOnShow)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when the hint hide
     */
    ZFOBSERVER_EVENT(HintOnHide)

public:
    /**
     * @brief get hint list in specified #ZFUISysWindow
     */
    ZFMETHOD_DECLARE_STATIC_1(ZFCoreArrayPOD<ZFUIHint *>, hintList,
                              ZFMP_IN_OPT(ZFUISysWindow *, inSysWindow, zfnull))

public:
    /**
     * @brief the window to hold the #hintContent
     *
     * by default, the window would:
     * -  have #ZFUIWindowLevel::e_ZFFrameworkFgHighest as #ZFUIWindow::windowLevel
     * -  have #ZFUIView::viewUIEnableTree been disabled
     * -  have #ZFUIGlobalStyle::itemSizeText as #ZFUIView::viewSizeMin
     * -  have #ZFUIWindow::windowLayoutParam as:
     *   -  #ZFUISizeParamWrapWrap as #ZFUILayoutParam::sizeParam
     *   -  #ZFUIAlign::e_Center as #ZFUILayoutParam::layoutAlign
     *   -  #ZFUIGlobalStyle::itemMargin as #ZFUILayoutParam::layoutMargin
     *
     * you should update the layout param of the hint window instead of the hint view,
     * and the hint view would be layouted according the hintWindow's layout param
     */
    ZFMETHOD_DECLARE_0(ZFUIWindow *, hintWindow)

public:
    /**
     * @brief the actual hint view
     */
    ZFPROPERTY_RETAIN(ZFUIView *, hintContent)
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(ZFUIView *, hintContent)
    ZFPROPERTY_OVERRIDE_ON_DETACH_DECLARE(ZFUIView *, hintContent)
    /**
     * @brief the animation to show the hint, null by default
     */
    ZFPROPERTY_RETAIN(ZFAnimation *, hintAniShow)
    /**
     * @brief hint duration in miliseconds, 0 to use #ZFUIHintDurationDefault, 0 by default
     *
     * @note show and hide animation won't be included in #hintDuration
     */
    ZFPROPERTY_ASSIGN(zftimet, hintDuration)
    /** @brief util method to #hintDuration */
    ZFMETHOD_INLINE_0(zftimet, hintDurationFixed)
    {
        return (this->hintDuration() > 0 ? this->hintDuration() : ZFUIHintDurationDefault());
    }
    /**
     * @brief the animation to hide the hint, null by default
     */
    ZFPROPERTY_RETAIN(ZFAnimation *, hintAniHide)

    /**
     * @brief whether automatically fix frame according to #ZFUIOnScreenKeyboardAutoResizeStart,
     *   true by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, hintWindowAutoResize, zftrue)
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(zfbool, hintWindowAutoResize)

public:
    /**
     * @brief show the hint with animation, do nothing if showing
     *
     * if there's another hint showing,
     * this one would be wait until all previous hint hide\n
     * the hint object would be retained when show,
     * and would be released automatically after hide
     */
    ZFMETHOD_DECLARE_0(void, hintShow)
    /**
     * @brief hide the hint with animation,
     *   do nothing if not showing,
     *   cancel show task if delaying
     */
    ZFMETHOD_DECLARE_0(void, hintHide)
    /**
     * @brief whether the hint is showing or delaying, see #hintDelaying
     */
    ZFMETHOD_DECLARE_0(zfbool, hintShowing)
    /**
     * @brief whether the hint is delaying, see #hintShowing
     */
    ZFMETHOD_DECLARE_0(zfbool, hintDelaying)
    /**
     * @brief return the animation if animating, or null if not
     */
    ZFMETHOD_DECLARE_0(ZFAnimation *, hintAnimating)

protected:
    /** @brief see #ZFUIHint::EventHintOnInit */
    virtual inline void hintOnInit(void)
    {
        this->observerNotify(ZFUIHint::EventHintOnInit());
    }
    /** @brief see #ZFUIHint::EventHintOnUpdate */
    virtual inline void hintOnUpdate(void)
    {
        this->observerNotify(ZFUIHint::EventHintOnUpdate());
    }
    /** @brief see #ZFUIHint::EventHintOnShow */
    virtual inline void hintOnShow(void)
    {
        this->observerNotify(ZFUIHint::EventHintOnShow());
    }
    /** @brief see #ZFUIHint::EventHintOnHide */
    virtual inline void hintOnHide(void)
    {
        this->observerNotify(ZFUIHint::EventHintOnHide());
    }

protected:
    zfoverride
    virtual inline void objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
    {
        zfsuper::objectInfoOnAppend(ret);
        if(this->hintContent() != zfnull)
        {
            ret += ", content: ";
            this->hintContent()->objectInfoT(ret);
        }
    }

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnInitFinish(void);
    zfoverride
    virtual void objectOnDealloc(void);

private:
    _ZFP_ZFUIHintPrivate *d;
    friend zfclassFwd _ZFP_ZFUIHintPrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIHint_h_

