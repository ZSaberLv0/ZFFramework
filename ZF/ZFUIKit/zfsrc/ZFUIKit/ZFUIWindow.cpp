#include "ZFUIWindow.h"
#include "ZFUIRootView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFENUM_DEFINE(ZFUIWindowLevel)

ZFSTYLE_DEFAULT_DEFINE(ZFUIWindow)

// ============================================================
// _ZFP_ZFUIWindowPrivate
zfclassNotPOD _ZFP_ZFUIWindowPrivate
{
public:
    ZFUISysWindow *windowOwnerSysWindow;
    ZFUILayoutParam *windowLayoutParam;
    zfbool windowRemoveOverrideFlag;

public:
    _ZFP_ZFUIWindowPrivate(void)
    : windowOwnerSysWindow(zfnull)
    , windowLayoutParam(zfnull)
    , windowRemoveOverrideFlag(zffalse)
    {
    }
};

// ============================================================
// ZFUIWindow
ZFOBJECT_REGISTER(ZFUIWindow)

ZFOBSERVER_EVENT_REGISTER(ZFUIWindow, WindowOwnerSysWindowOnChange)
ZFOBSERVER_EVENT_REGISTER(ZFUIWindow, WindowOnShow)
ZFOBSERVER_EVENT_REGISTER(ZFUIWindow, WindowOnHide)
ZFOBSERVER_EVENT_REGISTER(ZFUIWindow, WindowOwnerSysWindowOnResume)
ZFOBSERVER_EVENT_REGISTER(ZFUIWindow, WindowOwnerSysWindowOnPause)
ZFOBSERVER_EVENT_REGISTER(ZFUIWindow, WindowOwnerSysWindowOnRotate)

ZFMETHOD_DEFINE_1(ZFUIWindow, ZFUIWindow *, windowForView,
                  ZFMP_IN(ZFUIView *, forView))
{
    while(forView != zfnull && !forView->classData()->classIsTypeOf(ZFUIWindow::ClassData()))
    {
        forView = forView->viewParent();
    }
    if(forView != zfnull)
    {
        return ZFCastZFObject(ZFUIWindow *, forView);
    }
    return zfnull;
}

ZFMETHOD_DEFINE_1(ZFUIWindow, ZFUISysWindow *, sysWindowForView,
                  ZFMP_IN(ZFUIView *, view))
{
    ZFUIWindow *window = ZFUIWindow::windowForView(view);
    return ((window != zfnull) ? window->windowOwnerSysWindow() : zfnull);
}

ZFOBJECT_ON_INIT_DEFINE_1(ZFUIWindow, ZFMP_IN(ZFUISysWindow *, windowOwnerSysWindow))
{
    this->objectOnInit();
    if(windowOwnerSysWindow != zfnull)
    {
        this->windowOwnerSysWindow(windowOwnerSysWindow);
    }
}

void ZFUIWindow::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIWindowPrivate);
    d->windowLayoutParam = zfAlloc(ZFUILayoutParam);
    d->windowLayoutParam->sizeParam(ZFUISizeParamFillFill());
}
void ZFUIWindow::objectOnDealloc(void)
{
    zfRelease(d->windowLayoutParam);
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}

// ============================================================
// properties
ZFPROPERTY_ON_VERIFY_DEFINE(ZFUIWindow, ZFUIWindowLevelEnum, windowLevel)
{
    zfCoreAssertWithMessage(!this->windowShowing(), "you must not change window level while it's showing");
}

ZFMETHOD_DEFINE_1(ZFUIWindow, void, windowOwnerSysWindow,
                  ZFMP_IN(ZFUISysWindow *, windowOwnerSysWindow))
{
    if(d->windowOwnerSysWindow != windowOwnerSysWindow)
    {
        zfCoreAssertWithMessage(!this->windowShowing(), "you must not change window's owner while it's showing");
        zfCoreAssertWithMessage(windowOwnerSysWindow != zfnull, "null owner sys window");

        ZFUISysWindow *oldSysWindow = d->windowOwnerSysWindow;
        d->windowOwnerSysWindow = windowOwnerSysWindow;
        if(!(oldSysWindow == windowOwnerSysWindow
            || (oldSysWindow == zfnull && ZFUISysWindow::mainWindowAttached() && windowOwnerSysWindow == ZFUISysWindow::mainWindow())))
        {
            this->windowOwnerSysWindowOnChange(oldSysWindow);
        }
    }
}
ZFMETHOD_DEFINE_0(ZFUIWindow, ZFUISysWindow *, windowOwnerSysWindow)
{
    if(d->windowOwnerSysWindow == zfnull)
    {
        d->windowOwnerSysWindow = ZFUISysWindow::keyWindow();
    }
    return d->windowOwnerSysWindow;
}

ZFMETHOD_DEFINE_0(ZFUIWindow, void, windowShow)
{
    zfindex windowIndex = 0;
    zfindex addToIndex = 0;
    ZFCoreArrayPOD<ZFUIView *> tmpArray = this->windowOwnerSysWindow()->rootView()->childArray();
    for(zfindex i = 0; i < tmpArray.count(); ++i)
    {
        ZFUIWindow *tmpWindow = ZFCastZFObject(ZFUIWindow *, tmpArray.get(i));
        if(tmpWindow != zfnull)
        {
            if(tmpWindow->windowLevel() <= this->windowLevel())
            {
                ++windowIndex;
                addToIndex = i + 1;
            }
            else
            {
                break;
            }
        }
    }
    this->windowOwnerSysWindow()->rootView()->childAdd(this, d->windowLayoutParam, addToIndex);
    this->windowOwnerSysWindow()->rootView()->_ZFP_ZFUIRootView_windowList.add(windowIndex, this);
}
ZFMETHOD_DEFINE_0(ZFUIWindow, void, windowHide)
{
    zfRetain(this);
    ZFUIRootView *rootView = ZFCastZFObject(ZFUIRootView *, this->viewParent());
    if(rootView != zfnull)
    {
        rootView->_ZFP_ZFUIRootView_windowList.removeElement(this);
    }
    d->windowRemoveOverrideFlag = zftrue;
    this->viewRemoveFromParent();
    d->windowRemoveOverrideFlag = zffalse;
    zfRelease(this);
}
ZFMETHOD_DEFINE_0(ZFUIWindow, zfbool, windowShowing)
{
    return (this->viewParent() != zfnull);
}

ZFMETHOD_DEFINE_0(ZFUIWindow, void, windowMoveToTop)
{
    if(!this->windowShowing())
    {
        return ;
    }

    ZFCoreArrayPOD<ZFUIView *> tmpArray = this->windowOwnerSysWindow()->rootView()->childArray();
    ZFUIWindowLevelEnum selfWindowLevel = this->windowLevel();
    zfindex topIndex = zfindexMax();
    zfindex selfIndex = zfindexMax();
    for(zfindex i = tmpArray.count() - 1; i != zfindexMax(); --i)
    {
        ZFUIWindow *tmpWindow = ZFCastZFObject(ZFUIWindow *, tmpArray.get(i));
        if(tmpWindow != zfnull)
        {
            if(topIndex == zfindexMax() && tmpWindow->windowLevel() == selfWindowLevel)
            {
                topIndex = i;
            }
            if(tmpWindow == this)
            {
                selfIndex = i;
                break;
            }
        }
    }
    if(topIndex != selfIndex && topIndex != zfindexMax() && selfIndex != zfindexMax())
    {
        this->windowOwnerSysWindow()->rootView()->childMove(selfIndex, topIndex);
    }
}
ZFMETHOD_DEFINE_0(ZFUIWindow, void, windowMoveToBottom)
{
    if(!this->windowShowing())
    {
        return ;
    }

    ZFCoreArrayPOD<ZFUIView *> tmpArray = this->windowOwnerSysWindow()->rootView()->childArray();
    ZFUIWindowLevelEnum selfWindowLevel = this->windowLevel();
    zfindex bottomIndex = zfindexMax();
    zfindex selfIndex = zfindexMax();
    for(zfindex i = 0; i < tmpArray.count(); ++i)
    {
        ZFUIWindow *tmpWindow = ZFCastZFObject(ZFUIWindow *, tmpArray.get(i));
        if(tmpWindow != zfnull)
        {
            if(bottomIndex == zfindexMax() && tmpWindow->windowLevel() == selfWindowLevel)
            {
                bottomIndex = i;
            }
            if(tmpWindow == this)
            {
                selfIndex = i;
                break;
            }
        }
    }
    if(bottomIndex != selfIndex && bottomIndex != zfindexMax() && selfIndex != zfindexMax())
    {
        this->windowOwnerSysWindow()->rootView()->childMove(selfIndex, bottomIndex);
    }
}

ZFMETHOD_DEFINE_0(ZFUIWindow, ZFUILayoutParam *, windowLayoutParam)
{
    return d->windowLayoutParam;
}

void ZFUIWindow::viewOnAddToParent(ZF_IN ZFUIView *parent)
{
    zfCoreAssertWithMessage(parent->classData()->classIsTypeOf(ZFUIRootView::ClassData()), "you must not add a window to another view");
    zfsuper::viewOnAddToParent(parent);

    this->windowOnShow();
}
void ZFUIWindow::viewOnRemoveFromParent(ZF_IN ZFUIView *parent)
{
    // should not check remove here,
    // since it's all right to remove this window when owner sys window deallocated or when hide window
    zfsuper::viewOnRemoveFromParent(parent);

    if(d->windowRemoveOverrideFlag)
    {
        this->windowOnHide();
    }
    else
    {
        this->windowHide();
    }
}

// ============================================================
// util
ZFMETHOD_FUNC_DEFINE_3(void, ZFUIWindowShow,
                       ZFMP_IN(ZFUIView *, view),
                       ZFMP_IN_OPT(const ZFUISizeParam &, sizeParam, ZFUISizeParamFillFill()),
                       ZFMP_IN_OPT(const ZFUIAlignFlags &, layoutAlign, ZFUIAlign::e_Center))
{
    zfblockedAlloc(ZFUIWindow, window);
    window->childAdd(view, sizeParam, layoutAlign);
    window->windowShow();
}

ZF_NAMESPACE_GLOBAL_END

