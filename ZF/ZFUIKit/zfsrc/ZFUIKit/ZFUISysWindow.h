/**
 * @file ZFUISysWindow.h
 * @brief root native window
 */

#ifndef _ZFI_ZFUISysWindow_h_
#define _ZFI_ZFUISysWindow_h_

#include "ZFUIRootView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

ZF_NAMESPACE_BEGIN(ZFGlobalEvent)
/**
 * @brief see #ZFObject::observerNotify
 *
 * notified when #ZFUISysWindow::mainWindow has attached,
 * either by access #ZFUISysWindow::mainWindow,
 * or by #ZFUISysWindow::mainWindowRegister
 */
ZFOBSERVER_EVENT_GLOBAL(SysWindowMainWindowOnAttach)
ZF_NAMESPACE_END(ZFGlobalEvent)

// ============================================================
zfclassFwd ZFUISysWindowEmbedImpl;
zfclassFwd _ZFP_ZFUISysWindowPrivate;
/**
 * @brief root native window
 *
 * use #mainWindow to access app's main window,
 * use #modalWindowShow to show a new window,
 * use #nativeWindowEmbed to attach to native impl,
 * never create #ZFUISysWindow's instance manually
 */
zffinal zfclass ZF_ENV_EXPORT ZFUISysWindow : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFUISysWindow, ZFObject)

    // ============================================================
    // observer events
public:
    /**
     * @brief see #ZFObject::observerNotify
     *
     * notified when window created,
     * see #nativeWindowIsCreated for more info
     */
    ZFOBSERVER_EVENT(SysWindowOnCreate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * notified when window about to destroy
     */
    ZFOBSERVER_EVENT(SysWindowOnDestroy)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * notified when window become active
     */
    ZFOBSERVER_EVENT(SysWindowOnResume)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * notified when window enter background
     */
    ZFOBSERVER_EVENT(SysWindowOnPause)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * notified when window rotated,
     * new orientation can be accessed by #sysWindowOrientation
     */
    ZFOBSERVER_EVENT(SysWindowOnRotate)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * notified when #sysWindowMargin changed,
     * param0 is a #v_ZFUIMargin that holds the old window margin
     */
    ZFOBSERVER_EVENT(SysWindowMarginOnUpdate)

public:
    /**
     * @brief used to embed #ZFUISysWindow to existing UI framework
     *
     * #ZFUISysWindow would create internal native window by default,
     * however, you may create your own native window and attach #ZFUISysWindow to it\n
     * to achieve this feature,
     * you must supply custom embed impl by #ZFUISysWindowEmbedImpl,
     * call this method to get an attached #ZFUISysWindow,
     * and manually manage the life cycle of the returned #ZFUISysWindow\n
     * \n
     * the embedImpl would be retained until the created window destroy,
     * so that you only need to take care of the created winodw,
     * to explicitly clear all the contents,
     * simply use `window->nativeWindowEmbedImplDestroy()`\n
     * \n
     * see #ZFUISysWindowEmbedImpl for all the impl that you need to implement
     */
    static zfautoObjectT<ZFUISysWindow *> nativeWindowEmbed(ZF_IN ZFUISysWindowEmbedImpl *embedImpl);

    /**
     * @brief see #nativeWindowEmbed
     */
    virtual ZFUISysWindowEmbedImpl *nativeWindowEmbedImpl(void);

    /**
     * @brief util to destroy the window created by #nativeWindowEmbed
     *
     * equal to #ZFUISysWindowEmbedImpl::notifyOnPause
     * + #ZFUISysWindowEmbedImpl::notifyOnDestroy
     */
    ZFMETHOD_DECLARE_0(void, nativeWindowEmbedImplDestroy)

    /**
     * @brief default impl to attach window to native view
     *
     * note, after attach, you must destroy the window by #nativeWindowEmbedImplDestroy
     * before destroy the nativeParent\n
     * the nativeParent must support add one or more child which fill itself
     */
    ZFMETHOD_DECLARE_STATIC_1(zfautoObjectT<ZFUISysWindow *>, nativeWindowEmbedNativeView,
                              ZFMP_IN(void *, nativeParent))

public:
    /**
     * @brief manually register main window, must be called before accessing #mainWindow
     *
     * this method is useful if you want to embed whole ZFFramework to existing UI framework,
     * to create a custom #ZFUISysWindow, see #nativeWindowEmbed\n
     * @note you must manually manage the life cycle of the registered #ZFUISysWindow
     * @note once registered, it can not be changed back to original internal window,
     *   until #ZFFrameworkCleanup
     * @note it's safe to unregister and register a new window,
     *   but you must ensure you won't access the children of the window after you do so
     */
    ZFMETHOD_DECLARE_STATIC_1(void, mainWindowRegister,
                              ZFMP_IN(ZFUISysWindow *, window))
    /**
     * @brief get application's main window
     *
     * usually, use only one window is recommended\n
     * the main window would be created and attached automatically
     * when first time accessed (directly or implicitly by showing a #ZFUIWindow),
     * and can not be changed after creation\n
     * to embed ZFFramework to native framework,
     * you may use #mainWindowRegister
     */
    ZFMETHOD_DECLARE_STATIC_0(ZFUISysWindow *, mainWindow)
    /**
     * @brief whether #mainWindow has attached
     */
    ZFMETHOD_DECLARE_STATIC_0(zfbool, mainWindowAttached)

    /**
     * @brief default window that #ZFUIWindow would attach to, set null to use #mainWindow
     */
    ZFMETHOD_DECLARE_STATIC_1(void, keyWindow,
                              ZFMP_IN(ZFUISysWindow *, window))
    /**
     * @brief see #keyWindow, return #mainWindow if not set
     */
    ZFMETHOD_DECLARE_STATIC_0(ZFUISysWindow *, keyWindow)

public:
    /**
     * @brief native window margin according to impl
     */
    ZFMETHOD_DECLARE_0(const ZFUIMargin &, sysWindowMargin)
    void _ZFP_ZFUISysWindow_sysWindowMargin(ZF_IN const ZFUIMargin &sysWindowMargin);
protected:
    /**
     * @brief see #EventSysWindowMarginOnUpdate
     */
    virtual void sysWindowMarginOnUpdate(ZF_IN const ZFUIMargin &sysWindowMarginOld);

    ZFCLASS_PRIVATE_ALLOC("can only be created by reflect or ZFUISysWindow::modalWindowShow")
protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnInitFinish(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);
    zfoverride
    virtual void objectOnDealloc(void);

public:
    /**
     * @brief access the internal native window
     *
     * note, this value would be valid only after
     * #EventSysWindowOnCreate\n
     * note, this value can be null if window created by #nativeWindowEmbed
     */
    ZFMETHOD_DECLARE_0(void *, nativeWindow)

public:
    /**
     * @brief true if sys window created
     *
     * @note for some implementation,
     *   creating a ZFUISysWindow may or may not create native window immediately,
     *   you may use this method as well as #EventSysWindowOnCreate to see when did window created,
     *   typical usage:
     *   @code
     *       ZFUISysWindow *newWindow = ZFUISysWindow::mainWindow()->modalWindowShow();
     *       if(!newWindow->nativeWindowIsCreated())
     *       {
     *           ZFListener observer = ... {
     *               initWindow();
     *           };
     *           newWindow->observerAdd(ZFUISysWindow::EventSysWindowOnCreate(), observer);
     *       }
     *       else
     *       {
     *           initWindow();
     *       }
     *   @endcode
     */
    ZFMETHOD_DECLARE_0(zfbool, nativeWindowIsCreated)
    /**
     * @brief true if sys window resumed
     */
    ZFMETHOD_DECLARE_0(zfbool, nativeWindowIsResumed)

public:
    /**
     * @brief get current orientation
     */
    ZFMETHOD_DECLARE_0(ZFUIOrientationEnum, sysWindowOrientation)
    /**
     * @brief set supported orientation, orientation may or may not change immediately
     */
    ZFMETHOD_DECLARE_1(void, sysWindowOrientationFlags,
                       ZFMP_IN(const ZFUIOrientationFlags &, sysWindowOrientationFlags))
    /**
     * @brief get supported orientation
     */
    ZFMETHOD_DECLARE_0(const ZFUIOrientationFlags &, sysWindowOrientationFlags)

public:
    /**
     * @brief show a new modal window,
     *   may or may not show immediately,
     *   may have different behavior on different implementations
     *
     * use only if necessary\n
     * it's recommended that app have only one ZFUISysWindow,
     * use #ZFUIWindow if necessary
     */
    ZFMETHOD_DECLARE_0(ZFUISysWindow *, modalWindowShow)
    /**
     * @brief finish and destroy self as a modal window
     */
    ZFMETHOD_DECLARE_0(void, modalWindowFinish)
    /**
     * @brief get self's current showing modal window
     */
    ZFMETHOD_DECLARE_0(ZFUISysWindow *, modalWindowShowingWindow)
    /**
     * @brief get self's owner if self is a showing modal window
     */
    ZFMETHOD_DECLARE_0(ZFUISysWindow *, modalWindowOwner)

public:
    /**
     * @brief get window's layout param, fill parent by default
     */
    ZFMETHOD_DECLARE_0(ZFUILayoutParam *, sysWindowLayoutParam)

    /**
     * @brief access the root view of the window
     *
     * the root view is ensured #ZFUIRootView type,
     * and you must not add it to another view
     */
    ZFMETHOD_DECLARE_0(ZFUIRootView *, rootView)

public:
    zffinal ZFUIRect _ZFP_ZFUISysWindow_measureWindow(ZF_IN const ZFUIRect &rootRefRect);
    zffinal void _ZFP_ZFUISysWindow_onCreate(ZF_IN void *nativeWindow);
    zffinal void _ZFP_ZFUISysWindow_onDestroy(void);
    zffinal void _ZFP_ZFUISysWindow_onResume(void);
    zffinal void _ZFP_ZFUISysWindow_onPause(void);
    zffinal void _ZFP_ZFUISysWindow_onRotate(void);
    zffinal void _ZFP_ZFUISysWindow_sysWindowLayoutUpdate(void);

private:
    _ZFP_ZFUISysWindowPrivate *d;
};

// ============================================================
/**
 * @brief impl for #ZFUISysWindow::nativeWindowEmbed
 */
zfinterface ZF_ENV_EXPORT ZFUISysWindowEmbedImpl : zfextends ZFInterface
{
    ZFINTERFACE_DECLARE_WITH_CUSTOM_CTOR(ZFUISysWindowEmbedImpl, ZFInterface)

public:
    ZFUISysWindow *_ZFP_ownerZFUISysWindow;
    /**
     * @brief the attached #ZFUISysWindow
     */
    inline ZFUISysWindow *ownerZFUISysWindow(void)
    {
        return this->_ZFP_ownerZFUISysWindow;
    }

public:
    /** @cond ZFPrivateDoc */
    ZFUISysWindowEmbedImpl(void) : _ZFP_ownerZFUISysWindow(zfnull) {}
    /** @endcond */

    // ============================================================
public:
    /**
     * @brief called when owner #ZFUISysWindow is about to destroy
     */
    virtual void nativeWindowOnCleanup(ZF_IN ZFUISysWindow *sysWindow)
    {
    }

    /**
     * @brief called to add root view to native window container
     *
     * if nativeParentView was specified,
     * it would be used to copy some settings to the window's root view
     */
    virtual void nativeWindowRootViewOnAdd(ZF_IN ZFUISysWindow *sysWindow,
                                           ZF_OUT void *&nativeParentView) zfpurevirtual;
    /**
     * @brief called to remove root view to native window container
     */
    virtual void nativeWindowRootViewOnRemove(ZF_IN ZFUISysWindow *sysWindow) zfpurevirtual;

    /**
     * @brief see #ZFUISysWindow::modalWindowShow
     */
    virtual zfautoObjectT<ZFUISysWindow *> modalWindowShow(ZF_IN ZFUISysWindow *sysWindowOwner) zfpurevirtual;
    /**
     * @brief see #ZFUISysWindow::modalWindowFinish
     */
    virtual void modalWindowFinish(ZF_IN ZFUISysWindow *sysWindowOwner,
                                   ZF_IN ZFUISysWindow *sysWindowToFinish) zfpurevirtual;

    /**
     * @brief called to update suggested window layout param,
     *   fill with no margin by default
     */
    virtual void sysWindowLayoutParamOnInit(ZF_IN ZFUISysWindow *sysWindow)
    {
    }
    /**
     * @brief called when window layout param changed
     */
    virtual void sysWindowLayoutParamOnChange(ZF_IN ZFUISysWindow *sysWindow)
    {
    }

    /**
     * @brief see #ZFUISysWindow::sysWindowOrientation
     */
    virtual ZFUIOrientationEnum sysWindowOrientation(ZF_IN ZFUISysWindow *sysWindow)
    {
        return ZFUIOrientation::e_Top;
    }
    /**
     * @brief see #ZFUISysWindow::sysWindowOrientationFlags,
     *  impl should have #ZFUIOrientation::e_Top as init value
     */
    virtual void sysWindowOrientationFlags(ZF_IN ZFUISysWindow *sysWindow,
                                           ZF_IN const ZFUIOrientationFlags &flags)
    {
    }

    // ============================================================
    // callbacks that implementations must notify
public:
    /**
     * @brief implementation must call this method to measure window's frame,
     *   and layout window using the result frame
     *
     * if you are embedding ZFUISysWindow to native view with custom layout logic,
     * you may skip this method
     */
    zffinal ZFUIRect notifyMeasureWindow(ZF_IN ZFUISysWindow *sysWindow,
                                         ZF_IN const ZFUIRect &rootRefRect,
                                         ZF_IN const ZFUIMargin &sysWindowMargin)
    {
        sysWindow->_ZFP_ZFUISysWindow_sysWindowMargin(sysWindowMargin);
        return sysWindow->_ZFP_ZFUISysWindow_measureWindow(rootRefRect);
    }
    /**
     * @brief implementation must call this to notify after window create
     */
    zffinal void notifyOnCreate(ZF_IN ZFUISysWindow *sysWindow,
                                ZF_IN void *nativeWindow)
    {
        sysWindow->_ZFP_ZFUISysWindow_onCreate(nativeWindow);
    }
    /**
     * @brief implementation must call this to notify before window destroy
     */
    zffinal void notifyOnDestroy(ZF_IN ZFUISysWindow *sysWindow)
    {
        sysWindow->_ZFP_ZFUISysWindow_onDestroy();
    }
    /**
     * @brief implementation must call this to notify after window resume,
     *   note that rotate a window should not notify this
     */
    zffinal void notifyOnResume(ZF_IN ZFUISysWindow *sysWindow)
    {
        sysWindow->_ZFP_ZFUISysWindow_onResume();
    }
    /**
     * @brief implementation must call this to notify before window pause,
     *   note that rotate a window should not notify this
     */
    zffinal void notifyOnPause(ZF_IN ZFUISysWindow *sysWindow)
    {
        sysWindow->_ZFP_ZFUISysWindow_onPause();
    }
    /**
     * @brief implementation must call this to notify when window rotate
     */
    zffinal void notifyOnRotate(ZF_IN ZFUISysWindow *sysWindow)
    {
        sysWindow->_ZFP_ZFUISysWindow_onRotate();
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUISysWindow_h_

