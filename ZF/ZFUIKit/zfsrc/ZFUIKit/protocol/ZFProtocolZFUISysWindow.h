/**
 * @file ZFProtocolZFUISysWindow.h
 * @brief protocol for ZFUISysWindow
 */

#ifndef _ZFI_ZFProtocolZFUISysWindow_h_
#define _ZFI_ZFProtocolZFUISysWindow_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFUIKit/ZFUISysWindow.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFUISysWindow
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFUISysWindow)
    // ============================================================
    // main window impl
public:
    /**
     * @brief see #ZFUISysWindow::mainWindow
     *
     * for implementations,
     * you should create a new window if not initialized,
     * or return the created one if exist
     */
    virtual ZFUISysWindow *mainWindow(void) zfpurevirtual;
    /**
     * @brief called to cleanup native main window
     *
     * this is ensured to be called during #ZFFrameworkCleanup
     * with level #ZFLevelZFFrameworkLow\n
     * after cleanup, you must ensured it's reentrant to access the main window again
     */
    virtual void mainWindowOnCleanup(void) zfpurevirtual;
    /**
     * @brief called when main window destroyed by #notifyOnDestroy
     */
    virtual void mainWindowOnDestroy(void) zfpurevirtual;

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
    virtual zfautoObject modalWindowShow(ZF_IN ZFUISysWindow *sysWindowOwner) zfpurevirtual;
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
    virtual ZFUIOrientationEnum sysWindowOrientation(ZF_IN ZFUISysWindow *sysWindow) zfpurevirtual;
    /**
     * @brief see #ZFUISysWindow::sysWindowOrientationFlags,
     *  impl should have #ZFUIOrientation::e_Top as init value
     */
    virtual void sysWindowOrientationFlags(ZF_IN ZFUISysWindow *sysWindow,
                                           ZF_IN const ZFUIOrientationFlags &flags) zfpurevirtual;

    // ============================================================
    // callbacks that implementations must notify
public:
    /**
     * @brief implementation must call this method to measure window's frame,
     *   and layout window using the result frame
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
ZFPROTOCOL_INTERFACE_END(ZFUISysWindow)

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFUISysWindow_h_

