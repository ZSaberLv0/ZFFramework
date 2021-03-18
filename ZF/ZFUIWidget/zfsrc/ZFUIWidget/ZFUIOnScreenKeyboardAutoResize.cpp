#include "ZFUIOnScreenKeyboardAutoResize.h"

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData;
static void _ZFP_ZFUIOnScreenKeyboardAutoResize_doStart(ZF_IN ZFUIWindow *window);
static void _ZFP_ZFUIOnScreenKeyboardAutoResize_doStop(ZF_IN ZFUIWindow *window, ZF_IN _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *taskData);
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFUIOnScreenKeyboardAutoResize_onScreenKeyboardStateChange);
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFUIOnScreenKeyboardAutoResize_windowOnUpdateLayout);
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFUIOnScreenKeyboardAutoResize_windowOnSysWindowChange);
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFUIOnScreenKeyboardAutoResize_windowLayoutMarginChange);

zfclass _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData : zfextends ZFObject
{
    ZFOBJECT_DECLARE(_ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData, ZFObject)

protected:
    zfoverride
    virtual void objectOnInit(void)
    {
        zfsuper::objectOnInit();
        this->ownerWindow = zfnull;
        this->startCount = 1;
        this->layoutMarginSaved = ZFUIMarginZero();
        this->layoutMarginHasStored = zffalse;
    }
    zfoverride
    virtual void objectOnDeallocPrepare(void)
    {
        _ZFP_ZFUIOnScreenKeyboardAutoResize_doStop(this->ownerWindow, this);
        zfsuper::objectOnDeallocPrepare();
    }

public:
    ZFUIWindow *ownerWindow;
    zfindex startCount;
    ZFUIMargin layoutMarginSaved;
    zfbool layoutMarginHasStored;
};

ZFMETHOD_FUNC_DEFINE_1(void, ZFUIOnScreenKeyboardAutoResizeStart,
                       ZFMP_IN(ZFUIWindow *, window))
{
    if(window == zfnull)
    {
        return ;
    }

    zfCoreMutexLocker();

    _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *taskData = ZFAny(window->objectTag(
        _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData::ClassData()->classNameFull()));
    if(taskData != zfnull)
    {
        taskData->startCount += 1;
        return ;
    }
    zfblockedAlloc(_ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData, taskDataTmp);
    taskData = taskDataTmp;
    taskData->ownerWindow = window;
    window->objectTag(_ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData::ClassData()->classNameFull(), taskDataTmp);

    _ZFP_ZFUIOnScreenKeyboardAutoResize_doStart(window);
}
ZFMETHOD_FUNC_DEFINE_1(void, ZFUIOnScreenKeyboardAutoResizeStop,
                       ZFMP_IN(ZFUIWindow *, window))
{
    if(window == zfnull)
    {
        return ;
    }

    zfCoreMutexLocker();

    _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *taskData = ZFAny(window->objectTag(
        _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData::ClassData()->classNameFull()));
    if(taskData == zfnull)
    {
        return ;
    }
    if(taskData->startCount > 1)
    {
        taskData->startCount -= 1;
        return ;
    }
    window->objectTagRemove(_ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData::ClassData()->classNameFull());
}

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIOnScreenKeyboardAutoResizeDataHolder, ZFLevelZFFrameworkEssential)
{
    this->onScreenKeyboardStateChangeListener = ZFCallbackForFunc(_ZFP_ZFUIOnScreenKeyboardAutoResize_onScreenKeyboardStateChange);
    this->windowOnUpdateLayoutListener = ZFCallbackForFunc(_ZFP_ZFUIOnScreenKeyboardAutoResize_windowOnUpdateLayout);
    this->windowOnSysWindowChangeListener = ZFCallbackForFunc(_ZFP_ZFUIOnScreenKeyboardAutoResize_windowOnSysWindowChange);
    this->windowLayoutMarginChangeListener = ZFCallbackForFunc(_ZFP_ZFUIOnScreenKeyboardAutoResize_windowLayoutMarginChange);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIOnScreenKeyboardAutoResizeDataHolder)
{
    zfCoreAssertWithMessageTrim(this->windowList.isEmpty(),
        "ZFUIOnScreenKeyboardAutoResizeStart/ZFUIOnScreenKeyboardAutoResizeStop mismatch, have you forgot to stop?");
}
public:
    ZFCoreArrayPOD<ZFUIWindow *> windowList;
    ZFListener onScreenKeyboardStateChangeListener;
    ZFListener windowOnUpdateLayoutListener;
    ZFListener windowOnSysWindowChangeListener;
    ZFListener windowLayoutMarginChangeListener;
ZF_GLOBAL_INITIALIZER_END(ZFUIOnScreenKeyboardAutoResizeDataHolder)

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIOnScreenKeyboardAutoResizeAutoStop, ZFLevelZFFrameworkEssential)
{
    (void)ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIOnScreenKeyboardAutoResizeDataHolder);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIOnScreenKeyboardAutoResizeAutoStop)
{
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIOnScreenKeyboardAutoResizeDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIOnScreenKeyboardAutoResizeDataHolder);
    while(!d->windowList.isEmpty())
    {
        d->windowList.getLast()->objectTagRemove(_ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData::ClassData()->classNameFull());
    }
}
ZF_GLOBAL_INITIALIZER_END(ZFUIOnScreenKeyboardAutoResizeAutoStop)

static void _ZFP_ZFUIOnScreenKeyboardAutoResize_apply(ZF_IN ZFUIWindow *window, ZF_IN ZFUIOnScreenKeyboardState *state)
{
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIOnScreenKeyboardAutoResizeDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIOnScreenKeyboardAutoResizeDataHolder);
    _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *taskData = ZFAny(window->objectTag(
        _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData::ClassData()->classNameFull()));
    if(taskData == zfnull)
    {
        return ;
    }
    const ZFUIMargin &layoutMarginOld = (taskData->layoutMarginHasStored ? taskData->layoutMarginSaved : window->windowLayoutParam()->layoutMargin());
    if(state->keyboardShowing() && window->windowShowing())
    {
        ZFUIMargin margin = layoutMarginOld;

        ZFUIRect windowFrame = ZFUIRectZero();
        ZFUIViewPositionOnScreen(windowFrame, window->windowOwnerSysWindow()->rootView());
        ZFUIRectApplyMargin(windowFrame, windowFrame, margin);
        ZFUIRectApplyMargin(windowFrame, windowFrame, window->windowOwnerSysWindow()->sysWindowMargin());

        ZFUIRect clientFrame = ZFUIRectZero();
        state->keyboardFixClientFrameT(clientFrame);

        if(ZFUIRectGetLeft(clientFrame) > ZFUIRectGetLeft(windowFrame))
        {
            margin.left += ZFUIRectGetLeft(clientFrame) - ZFUIRectGetLeft(windowFrame);
        }
        if(ZFUIRectGetTop(clientFrame) > ZFUIRectGetTop(windowFrame))
        {
            margin.top += ZFUIRectGetTop(clientFrame) - ZFUIRectGetTop(windowFrame);
        }
        if(ZFUIRectGetRight(clientFrame) < ZFUIRectGetRight(windowFrame))
        {
            margin.right += ZFUIRectGetRight(windowFrame) - ZFUIRectGetRight(clientFrame);
        }
        if(ZFUIRectGetBottom(clientFrame) < ZFUIRectGetBottom(windowFrame))
        {
            margin.bottom += ZFUIRectGetBottom(windowFrame) - ZFUIRectGetBottom(clientFrame);
        }

        if(!taskData->layoutMarginHasStored)
        {
            taskData->layoutMarginHasStored = zftrue;
            taskData->layoutMarginSaved = layoutMarginOld;
        }

        window->windowLayoutParam()->observerRemove(
            ZFObject::EventObjectPropertyValueOnUpdate(),
            d->windowLayoutMarginChangeListener);
        window->windowLayoutParam()->layoutMargin(margin);
        window->windowLayoutParam()->observerAdd(
            ZFObject::EventObjectPropertyValueOnUpdate(),
            d->windowLayoutMarginChangeListener,
            window->objectHolder());
    }
    else
    {
        window->windowLayoutParam()->observerRemove(
            ZFObject::EventObjectPropertyValueOnUpdate(),
            d->windowLayoutMarginChangeListener);
        taskData->layoutMarginHasStored = zffalse;
        window->windowLayoutParam()->layoutMargin(layoutMarginOld);
    }
}

static void _ZFP_ZFUIOnScreenKeyboardAutoResize_doStart(ZF_IN ZFUIWindow *window)
{
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIOnScreenKeyboardAutoResizeDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIOnScreenKeyboardAutoResizeDataHolder);
    if(d->windowList.isEmpty())
    {
        ZFObjectGlobalEventObserver().observerAdd(ZFUIOnScreenKeyboardState::EventKeyboardStateOnChange(), d->onScreenKeyboardStateChangeListener);
    }
    d->windowList.add(window);

    window->windowOwnerSysWindow()->rootView()->observerAdd(
        ZFUIView::EventViewLayoutOnLayoutPrepare(),
        d->windowOnUpdateLayoutListener,
        window->objectHolder());
    window->observerAdd(
        ZFUIWindow::EventWindowOwnerSysWindowOnChange(),
        d->windowOnSysWindowChangeListener);
    _ZFP_ZFUIOnScreenKeyboardAutoResize_apply(window, ZFUIOnScreenKeyboardState::instanceForView(window));
}
static void _ZFP_ZFUIOnScreenKeyboardAutoResize_doStop(ZF_IN ZFUIWindow *window, ZF_IN _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *taskData)
{
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIOnScreenKeyboardAutoResizeDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIOnScreenKeyboardAutoResizeDataHolder);

    window->windowOwnerSysWindow()->rootView()->observerRemove(
        ZFUIView::EventViewLayoutOnLayoutPrepare(),
        d->windowOnUpdateLayoutListener);
    window->observerRemove(
        ZFUIWindow::EventWindowOwnerSysWindowOnChange(),
        d->windowOnSysWindowChangeListener);
    window->windowLayoutParam()->observerRemove(
        ZFObject::EventObjectPropertyValueOnUpdate(),
        d->windowLayoutMarginChangeListener);
    window->layoutRequest();

    if(taskData->layoutMarginHasStored)
    {
        taskData->layoutMarginHasStored = zffalse;
        window->windowLayoutParam()->layoutMargin(taskData->layoutMarginSaved);
    }

    d->windowList.removeElement(window);
    if(d->windowList.isEmpty())
    {
        ZFObjectGlobalEventObserver().observerRemove(ZFUIOnScreenKeyboardState::EventKeyboardStateOnChange(), d->onScreenKeyboardStateChangeListener);
    }
}
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFUIOnScreenKeyboardAutoResize_onScreenKeyboardStateChange)
{
    ZFUIOnScreenKeyboardState *state = listenerData.sender<ZFUIOnScreenKeyboardState *>();
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIOnScreenKeyboardAutoResizeDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIOnScreenKeyboardAutoResizeDataHolder);
    for(zfindex i = 0; i < d->windowList.count(); ++i)
    {
        ZFUIWindow *window = d->windowList[i];
        if(window->windowOwnerSysWindow() != state->keyboardOwnerSysWindow())
        {
            continue;
        }

        _ZFP_ZFUIOnScreenKeyboardAutoResize_apply(window, state);
    }
}
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFUIOnScreenKeyboardAutoResize_windowOnUpdateLayout)
{
    ZFUIRootView *rootView = listenerData.sender<ZFUIRootView *>();
    ZFUIWindow *window = userData->objectHolded();
    if(rootView->viewFrame() != rootView->viewFramePrev())
    {
        _ZFP_ZFUIOnScreenKeyboardAutoResize_apply(window, ZFUIOnScreenKeyboardState::instanceForView(window));
    }
}
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFUIOnScreenKeyboardAutoResize_windowOnSysWindowChange)
{
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIOnScreenKeyboardAutoResizeDataHolder) *d = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIOnScreenKeyboardAutoResizeDataHolder);

    ZFUIWindow *window = listenerData.sender<ZFUIWindow *>();
    ZFUIRootView *rootViewOld = listenerData.param0<ZFUISysWindow *>()->rootView();
    rootViewOld->observerRemove(
        ZFUIView::EventViewLayoutOnLayoutPrepare(),
        d->windowOnUpdateLayoutListener);
    window->windowOwnerSysWindow()->rootView()->observerAdd(
        ZFUIView::EventViewLayoutOnLayoutPrepare(),
        d->windowOnUpdateLayoutListener,
        window->objectHolder());
}
static ZFLISTENER_PROTOTYPE_EXPAND(_ZFP_ZFUIOnScreenKeyboardAutoResize_windowLayoutMarginChange)
{
    const ZFProperty *property = listenerData.param0<v_ZFProperty *>()->zfv;
    if(property != ZFPropertyAccess(ZFUIViewLayoutParam, layoutMargin))
    {
        return ;
    }

    ZFUIViewLayoutParam *layoutParam = listenerData.sender<ZFUIViewLayoutParam *>();
    ZFUIWindow *window = userData->objectHolded();
    _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData *taskData = ZFAny(window->objectTag(
        _ZFP_I_ZFUIOnScreenKeyboardAutoResizeTaskData::ClassData()->classNameFull()));
    if(taskData == zfnull)
    {
        return ;
    }
    if(taskData->layoutMarginHasStored)
    {
        taskData->layoutMarginSaved = layoutParam->layoutMargin();
    }
}

ZF_NAMESPACE_GLOBAL_END

