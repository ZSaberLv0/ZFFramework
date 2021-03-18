/**
 * @file ZFUIRootView.h
 * @brief root view for holding a ZFUIView's tree
 */

#ifndef _ZFI_ZFUIRootView_h_
#define _ZFI_ZFUIRootView_h_

#include "ZFUIView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd ZFUISysWindow;
zfclassFwd ZFUIWindow;
/**
 * @brief root view for holding a ZFUIView's tree
 *
 * internally used to manage view's,
 * usually you should not use this class
 */
zffinal zfclass ZF_ENV_EXPORT ZFUIRootView : zfextends ZFUIView
{
    ZFOBJECT_DECLARE(ZFUIRootView, ZFUIView)
    ZFCLASS_PRIVATE_ALLOC("can only be created by owner ZFUISysWindow")

public:
    /**
     * @brief set scale for root view, as well as all of its children
     */
    ZFMETHOD_DECLARE_1(void, scaleForApp,
                       ZFMP_IN(zffloat, scale))
    zfoverride
    virtual zffloat scaleForApp(void)
    {
        return zfsuper::scaleForApp();
    }

public:
    /**
     * @brief owner #ZFUISysWindow
     */
    zffinal ZFUISysWindow *rootViewOwnerSysWindow(void)
    {
        return this->_ZFP_ZFUIRootView_rootViewOwnerSysWindow;
    }

    /**
     * @brief all #ZFUIWindow attached to the root view
     */
    ZFMETHOD_DECLARE_0(const ZFCoreArrayPOD<ZFUIWindow *> &, windowList)

protected:
    zfoverride
    virtual void viewOnAddToParent(ZF_IN ZFUIView *parent);
    zfoverride
    virtual void viewOnRemoveFromParent(ZF_IN ZFUIView *parent);

protected:
    /**
     * @brief for a root view, this method would always return sizeHint
     */
    zfoverride
    virtual void layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                 ZF_IN const ZFUISize &sizeHint,
                                 ZF_IN const ZFUISizeParam &sizeParam);
    zfoverride
    virtual void layoutOnLayout(ZF_IN const ZFUIRect &bounds);

public:
    ZFUISysWindow *_ZFP_ZFUIRootView_rootViewOwnerSysWindow;
    ZFCoreArrayPOD<ZFUIWindow *> _ZFP_ZFUIRootView_windowList;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIRootView_h_

