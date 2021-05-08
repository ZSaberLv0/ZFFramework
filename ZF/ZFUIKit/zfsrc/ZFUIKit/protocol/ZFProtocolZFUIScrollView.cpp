#include "ZFProtocolZFUIScrollView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#define _ZFP_ZFProtocolZFUIScrollView_DEBUG 0

ZFPROTOCOL_INTERFACE_REGISTER(ZFUIScrollView)

// ============================================================
zftimet _ZFP_ZFProtocolZFUIScrollView_scrollAnimationStart(ZF_IN ZFPROTOCOL_INTERFACE_CLASS(ZFUIScrollView) *impl,
                                                           ZF_IN ZFUIScrollView *scrollView,
                                                           ZF_IN zftimet recommendTimerInterval)
{
    zfblockedAlloc(ZFTimer, scrollTimer);
    scrollTimer->timerInterval(recommendTimerInterval);
    ZFLISTENER_LOCAL(scrollTimerEvent, {
        ZFPROTOCOL_INTERFACE_CLASS(ZFUIScrollView) *impl = (ZFPROTOCOL_INTERFACE_CLASS(ZFUIScrollView) *)(userData->objectTag<ZFTypeHolder *>("_ZFP_ZFProtocolZFUIScrollView_impl")->holdedData);
        ZFUIScrollView *scrollView = userData->objectTag("_ZFP_ZFProtocolZFUIScrollView_scrollView")->objectHolded();
        impl->notifyScrollViewScrollAnimation(scrollView, ZFTime::timestamp());
    })
    zfblockedAlloc(ZFObject, userData);
    userData->objectTag("_ZFP_ZFProtocolZFUIScrollView_impl", zflineAlloc(ZFTypeHolder, impl, ZFTypeHolderTypePointerRef));
    userData->objectTag("_ZFP_ZFProtocolZFUIScrollView_scrollView", scrollView->objectHolder());
    scrollTimer->observerAdd(ZFTimer::EventTimerOnActivate(), scrollTimerEvent, userData);
    scrollView->objectTag("_ZFP_ZFProtocolZFUIScrollView_scrollTimer", scrollTimer);
    scrollTimer->timerStart();
    return ZFTime::timestamp();
}
void _ZFP_ZFProtocolZFUIScrollView_scrollAnimationStop(ZF_IN ZFPROTOCOL_INTERFACE_CLASS(ZFUIScrollView) *impl,
                                                       ZF_IN ZFUIScrollView *scrollView)
{
    ZFTimer *scrollTimer = scrollView->objectTag<ZFTimer *>("_ZFP_ZFProtocolZFUIScrollView_scrollTimer");
    zfRetain(scrollTimer);
    scrollView->objectTagRemove("_ZFP_ZFProtocolZFUIScrollView_scrollTimer");
    scrollTimer->timerStop();
    zfRelease(scrollTimer);
}

// ============================================================
// default impl for track delay
void ZFUIScrollViewImplHelperProtocol::trackDelayStart(ZF_IN zftimet timeoutMiliSeconds,
                                                       ZF_IN ZFUIScrollViewImplHelper *owner)
{
    if(owner->_trackDelayDefaultImplTimer == zfnull)
    {
        owner->_trackDelayDefaultImplTimer = zfAlloc(ZFTimer);
        owner->_trackDelayDefaultImplTimer->timerInterval(timeoutMiliSeconds);
        ZFLISTENER_LOCAL(timerActivated, {
            listenerData.sender<ZFTimer *>()->timerStop();
            userData->to<ZFTypeHolder *>()->holdedDataPointer<ZFUIScrollViewImplHelper *>()->trackDelayNotifyTimeout();
        })
        owner->_trackDelayDefaultImplTimer->observerAdd(
            ZFTimer::EventTimerOnActivate(),
            timerActivated,
            zflineAlloc(ZFTypeHolder, owner, ZFTypeHolderTypePointerRef));
    }
    owner->_trackDelayDefaultImplTimer->timerInterval(timeoutMiliSeconds);
    owner->_trackDelayDefaultImplTimer->timerStart();
}
void ZFUIScrollViewImplHelperProtocol::trackDelayStop(ZF_IN ZFUIScrollViewImplHelper *owner)
{
    if(owner->_trackDelayDefaultImplTimer != zfnull)
    {
        owner->_trackDelayDefaultImplTimer->timerStop();
    }
}

// ============================================================
typedef enum {
    _ZFP_ZFUIScrollViewImplHelperDragStateTracking,
    _ZFP_ZFUIScrollViewImplHelperDragStateDragging,
    _ZFP_ZFUIScrollViewImplHelperDragStateIgnored,
} _ZFP_ZFUIScrollViewImplHelperDragState;
zfclassNotPOD _ZFP_ZFUIScrollViewImplHelperPrivate
{
public:
    ZFUIScrollViewImplHelper *pimplOwner;
    _ZFP_ZFUIScrollViewImplHelperDragState dragState;
    zfbool mouseDownFlag;
    void *mouseDownSaved;
    ZFUIScrollViewImplHelper *trackingParent;
    ZFUIScrollViewImplHelper *trackingChild;
    void *processingChild;

public:
    _ZFP_ZFUIScrollViewImplHelperPrivate(void)
    : pimplOwner(zfnull)
    , dragState(_ZFP_ZFUIScrollViewImplHelperDragStateTracking)
    , mouseDownFlag(zffalse)
    , mouseDownSaved(zfnull)
    , trackingParent(zfnull)
    , trackingChild(zfnull)
    , processingChild(zfnull)
    {
    }

public:
    zfbool scrollable(void)
    {
        ZFUIScrollView *v = pimplOwner->scrollView;
        return (v->viewUIEnable() && v->viewUIEnableTree() && v->scrollEnable() && (v->scrollContentFrame().width > v->viewFrame().width || v->scrollContentFrame().height > v->viewFrame().height));
    }
    zfbool scrollableX(void)
    {
        ZFUIScrollView *v = pimplOwner->scrollView;
        return (v->viewUIEnable() && v->viewUIEnableTree() && v->scrollEnable() && (v->scrollContentFrame().width > v->viewFrame().width));
    }
    zfbool scrollableY(void)
    {
        ZFUIScrollView *v = pimplOwner->scrollView;
        return (v->viewUIEnable() && v->viewUIEnableTree() && v->scrollEnable() && (v->scrollContentFrame().height > v->viewFrame().height));
    }
    zfbool scrollableLeft(void)
    {
        ZFUIScrollView *v = pimplOwner->scrollView;
        return (v->viewUIEnable() && v->viewUIEnableTree() && v->scrollEnable() && (v->scrollContentFrame().x < 0));
    }
    zfbool scrollableTop(void)
    {
        ZFUIScrollView *v = pimplOwner->scrollView;
        return (v->viewUIEnable() && v->viewUIEnableTree() && v->scrollEnable() && (v->scrollContentFrame().y < 0));
    }
    zfbool scrollableRight(void)
    {
        ZFUIScrollView *v = pimplOwner->scrollView;
        return (v->viewUIEnable() && v->viewUIEnableTree() && v->scrollEnable() && (ZFUIRectGetRight(v->scrollContentFrame()) > v->viewFrame().width));
    }
    zfbool scrollableBottom(void)
    {
        ZFUIScrollView *v = pimplOwner->scrollView;
        return (v->viewUIEnable() && v->viewUIEnableTree() && v->scrollEnable() && (ZFUIRectGetBottom(v->scrollContentFrame()) > v->viewFrame().height));
    }

    zfbool scrollableOrBounceable(void)
    {
        ZFUIScrollView *v = pimplOwner->scrollView;
        return (v->viewUIEnable() && v->viewUIEnableTree() && v->scrollEnable() && (v->scrollBounceHorizontalAlways() || v->scrollBounceVerticalAlways() || v->scrollContentFrame().width > v->viewFrame().width || v->scrollContentFrame().height > v->viewFrame().height));
    }
    zfbool scrollableOrBounceableX(void)
    {
        ZFUIScrollView *v = pimplOwner->scrollView;
        return (v->viewUIEnable() && v->viewUIEnableTree() && v->scrollEnable() && (v->scrollBounceHorizontalAlways() || v->scrollContentFrame().width > v->viewFrame().width));
    }
    zfbool scrollableOrBounceableY(void)
    {
        ZFUIScrollView *v = pimplOwner->scrollView;
        return (v->viewUIEnable() && v->viewUIEnableTree() && v->scrollEnable() && (v->scrollBounceVerticalAlways() || v->scrollContentFrame().height > v->viewFrame().height));
    }
    zfbool scrollableOrBounceableLeft(void)
    {
        ZFUIScrollView *v = pimplOwner->scrollView;
        return (v->viewUIEnable() && v->viewUIEnableTree() && v->scrollEnable() && (v->scrollBounceHorizontalAlways() || (v->scrollBounceHorizontal() && v->scrollContentFrame().width > v->viewFrame().width) || v->scrollContentFrame().x < 0));
    }
    zfbool scrollableOrBounceableTop(void)
    {
        ZFUIScrollView *v = pimplOwner->scrollView;
        return (v->viewUIEnable() && v->viewUIEnableTree() && v->scrollEnable() && (v->scrollBounceVerticalAlways() || (v->scrollBounceVertical() && v->scrollContentFrame().height > v->viewFrame().height) || v->scrollContentFrame().y < 0));
    }
    zfbool scrollableOrBounceableRight(void)
    {
        ZFUIScrollView *v = pimplOwner->scrollView;
        return (v->viewUIEnable() && v->viewUIEnableTree() && v->scrollEnable() && (v->scrollBounceHorizontalAlways() || (v->scrollBounceHorizontal() && v->scrollContentFrame().width > v->viewFrame().width) || ZFUIRectGetRight(v->scrollContentFrame()) > v->viewFrame().width));
    }
    zfbool scrollableOrBounceableBottom(void)
    {
        ZFUIScrollView *v = pimplOwner->scrollView;
        return (v->viewUIEnable() && v->viewUIEnableTree() && v->scrollEnable() && (v->scrollBounceVerticalAlways() || (v->scrollBounceVertical() && v->scrollContentFrame().height > v->viewFrame().height) || ZFUIRectGetBottom(v->scrollContentFrame()) > v->viewFrame().height));
    }

public:
    void mouseDownResend(void)
    {
        if(this->mouseDownSaved && this->processingChild)
        {
            ZFUIPoint mouseDownPos = this->mouseEventPos(this->mouseDownSaved);
            this->mouseDownSaved = this->pimplOwner->implProtocol->translateFromParentToChild(
                this->processingChild,
                this->mouseDownSaved,
                mouseDownPos.x,
                mouseDownPos.y);
            #if _ZFP_ZFProtocolZFUIScrollView_DEBUG
            zfLogTrimT() << zfLogCurTimeString() << "[ScrollImpl]" << ZF_CALLER_LINE << this->pimplOwner->scrollView << "mouse down resend"
                << this->mouseEventPos(this->mouseDownSaved);
            #endif
            this->pimplOwner->implProtocol->mouseEventForward(this->processingChild, this->mouseDownSaved);
        }
        this->mouseDownCleanup();
    }
    void mouseDownCleanup(void)
    {
        if(this->mouseDownSaved)
        {
            this->pimplOwner->implProtocol->mouseEventCleanup(this->mouseDownSaved);
            this->mouseDownSaved = zfnull;
        }
        #if _ZFP_ZFProtocolZFUIScrollView_DEBUG
        zfLogTrimT() << zfLogCurTimeString() << "[ScrollImpl]" << ZF_CALLER_LINE << this->pimplOwner->scrollView << "track timer stop";
        #endif
        this->pimplOwner->implProtocol->trackDelayStop(this->pimplOwner);
    }

public:
    void mouseEventPos(ZF_OUT ZFUIPoint &mousePos, ZF_IN void *nativeMouseEvent)
    {
        this->pimplOwner->implProtocol->mouseEventPos(mousePos, nativeMouseEvent);
    }
    ZFUIPoint mouseEventPos(ZF_IN void *nativeMouseEvent)
    {
        ZFUIPoint mousePos = ZFUIPointZero();
        this->mouseEventPos(mousePos, nativeMouseEvent);
        return mousePos;
    }

public:
    void trackMouseDown(ZF_IN void *nativeMouseEvent);
    void trackMouseMove(ZF_IN void *nativeMouseEvent);
    void trackMouseUp(ZF_IN void *nativeMouseEvent);
    void trackMouseCancel(ZF_IN void *nativeMouseEvent);

    void resolveMouseDown(ZF_IN void *nativeMouseEvent);
    void resolveMouseMove(ZF_IN void *nativeMouseEvent);
    void resolveMouseUp(ZF_IN void *nativeMouseEvent);
    void resolveMouseCancel(ZF_IN void *nativeMouseEvent);
};

ZFUIScrollViewImplHelper::ZFUIScrollViewImplHelper(void)
: scrollView(zfnull)
, implProtocol(zfnull)
, nativeScrollTolerance(4)
, d(zfnull)
, _trackDelayDefaultImplTimer(zfnull)
{
    d = zfpoolNew(_ZFP_ZFUIScrollViewImplHelperPrivate);
    d->pimplOwner = this;
}
ZFUIScrollViewImplHelper::~ZFUIScrollViewImplHelper(void)
{
    if(_trackDelayDefaultImplTimer != zfnull)
    {
        _trackDelayDefaultImplTimer->timerStop();
        zfRelease(_trackDelayDefaultImplTimer);
        _trackDelayDefaultImplTimer = zfnull;
    }
    zfpoolDelete(d);
}

void ZFUIScrollViewImplHelper::interceptMouse(ZF_IN void *nativeMouseEvent,
                                              ZF_IN ZFUIMouseActionEnum mouseAction)
{
    #if _ZFP_ZFProtocolZFUIScrollView_DEBUG
    zfLogTrimT() << zfLogCurTimeString() << "[ScrollImpl]" << ZF_CALLER_LINE << d->pimplOwner->scrollView << "intercept" << mouseAction
        << d->mouseEventPos(nativeMouseEvent);
    #endif
    switch(mouseAction)
    {
        case ZFUIMouseAction::e_MouseDown:
            d->mouseDownFlag = zftrue;
            d->trackMouseDown(nativeMouseEvent);
            break;
        case ZFUIMouseAction::e_MouseMove:
            if(!d->mouseDownFlag)
            {
                #if _ZFP_ZFProtocolZFUIScrollView_DEBUG
                zfLogTrimT() << zfLogCurTimeString() << "[ScrollImpl]" << ZF_CALLER_LINE << d->pimplOwner->scrollView << "forward" << mouseAction
                    << d->mouseEventPos(nativeMouseEvent);
                #endif
                d->pimplOwner->implProtocol->mouseEventForward(d->pimplOwner->scrollView->nativeImplView(), nativeMouseEvent);
            }
            else
            {
                d->trackMouseMove(nativeMouseEvent);
            }
            break;
        case ZFUIMouseAction::e_MouseUp:
            if(!d->mouseDownFlag)
            {
                #if _ZFP_ZFProtocolZFUIScrollView_DEBUG
                zfLogTrimT() << zfLogCurTimeString() << "[ScrollImpl]" << ZF_CALLER_LINE << d->pimplOwner->scrollView << "forward" << mouseAction
                    << d->mouseEventPos(nativeMouseEvent);
                #endif
                d->pimplOwner->implProtocol->mouseEventForward(d->pimplOwner->scrollView->nativeImplView(), nativeMouseEvent);
            }
            else
            {
                d->mouseDownFlag = zffalse;
                d->trackMouseUp(nativeMouseEvent);
            }
            break;
        case ZFUIMouseAction::e_MouseCancel:
            if(!d->mouseDownFlag)
            {
                #if _ZFP_ZFProtocolZFUIScrollView_DEBUG
                zfLogTrimT() << zfLogCurTimeString() << "[ScrollImpl]" << ZF_CALLER_LINE << d->pimplOwner->scrollView << "forward" << mouseAction
                    << d->mouseEventPos(nativeMouseEvent);
                #endif
                d->pimplOwner->implProtocol->mouseEventForward(d->pimplOwner->scrollView->nativeImplView(), nativeMouseEvent);
            }
            else
            {
                d->mouseDownFlag = zffalse;
                d->trackMouseCancel(nativeMouseEvent);
            }
            break;
        case ZFUIMouseAction::e_MouseHoverEnter:
        case ZFUIMouseAction::e_MouseHover:
        case ZFUIMouseAction::e_MouseHoverExit:
            break;
        default:
            zfCoreCriticalShouldNotGoHere();
            break;
    }
}
void ZFUIScrollViewImplHelper::trackDelayNotifyTimeout(void)
{
    // mouse without drag and timeout,
    // restore mouse down event and ignore all following events
    d->dragState = _ZFP_ZFUIScrollViewImplHelperDragStateIgnored;

    // restore mouse down
    d->mouseDownResend();
}

// ============================================================
// track logic
void _ZFP_ZFUIScrollViewImplHelperPrivate::trackMouseDown(ZF_IN void *nativeMouseEvent)
{
    #if _ZFP_ZFProtocolZFUIScrollView_DEBUG
    zfLogTrimT() << zfLogCurTimeString() << "[ScrollImpl]" << ZF_CALLER_LINE << this->pimplOwner->scrollView << "track" << ZFUIMouseAction::e_MouseDown
        << this->mouseEventPos(nativeMouseEvent);
    #endif
    ZFUIPoint mousePos = this->mouseEventPos(nativeMouseEvent);
    zffloat &x = mousePos.x;
    zffloat &y = mousePos.y;

    this->dragState = _ZFP_ZFUIScrollViewImplHelperDragStateTracking;
    this->trackingChild = this->pimplOwner->implProtocol->findTouchedChildScrollView(x, y);
    if(this->trackingChild != zfnull && this->trackingChild->d->scrollableOrBounceable())
    {
        this->trackingChild->d->trackingParent = this->pimplOwner;

        void *tmp = this->pimplOwner->implProtocol->mouseEventClone(nativeMouseEvent);
        tmp = this->pimplOwner->implProtocol->translateFromParentToChild(
            this->trackingChild->scrollView->nativeImplView(),
            tmp,
            x, y);
        #if _ZFP_ZFProtocolZFUIScrollView_DEBUG
        zfLogTrimT() << zfLogCurTimeString() << "[ScrollImpl]" << ZF_CALLER_LINE << this->pimplOwner->scrollView << "forward" << ZFUIMouseAction::e_MouseDown
            << this->mouseEventPos(tmp);
        #endif
        this->pimplOwner->implProtocol->mouseEventForward(this->trackingChild->scrollView->nativeImplView(), tmp);
        this->pimplOwner->implProtocol->mouseEventCleanup(tmp);
        return ;
    }

    if(this->trackingChild != zfnull)
    {
        this->pimplOwner->implProtocol->findTouchedChildScrollViewCleanup(this->trackingChild);
        this->trackingChild = zfnull;
    }
    this->resolveMouseDown(nativeMouseEvent);
}
void _ZFP_ZFUIScrollViewImplHelperPrivate::trackMouseMove(ZF_IN void *nativeMouseEvent)
{
    #if _ZFP_ZFProtocolZFUIScrollView_DEBUG
    zfLogTrimT() << zfLogCurTimeString() << "[ScrollImpl]" << ZF_CALLER_LINE << this->pimplOwner->scrollView << "track" << ZFUIMouseAction::e_MouseMove
        << this->mouseEventPos(nativeMouseEvent);
    #endif
    ZFUIPoint mousePos = this->mouseEventPos(nativeMouseEvent);
    zffloat &x = mousePos.x;
    zffloat &y = mousePos.y;
    if(this->trackingChild != zfnull)
    {
        void *tmp = this->pimplOwner->implProtocol->mouseEventClone(nativeMouseEvent);
        tmp = this->pimplOwner->implProtocol->translateFromParentToChild(
            this->trackingChild->scrollView->nativeImplView(),
            tmp,
            x, y);
        #if _ZFP_ZFProtocolZFUIScrollView_DEBUG
        zfLogTrimT() << zfLogCurTimeString() << "[ScrollImpl]" << ZF_CALLER_LINE << this->pimplOwner->scrollView << "forward" << ZFUIMouseAction::e_MouseMove
            << this->mouseEventPos(tmp);
        #endif
        this->pimplOwner->implProtocol->mouseEventForward(this->trackingChild->scrollView->nativeImplView(), tmp);
        this->pimplOwner->implProtocol->mouseEventCleanup(tmp);
        return ;
    }

    if(this->trackingParent == zfnull || this->dragState != _ZFP_ZFUIScrollViewImplHelperDragStateTracking)
    {
        this->resolveMouseMove(nativeMouseEvent);
        return ;
    }

    ZFUIPoint mouseDownPos = this->mouseEventPos(this->mouseDownSaved);
    zffloat xOffset = x - mouseDownPos.x;
    zffloat yOffset = y - mouseDownPos.y;
    if(zfmAbs(xOffset) <= this->pimplOwner->nativeScrollTolerance && zfmAbs(yOffset) <= this->pimplOwner->nativeScrollTolerance)
    {
        return ;
    }
    zfbool stopTracking = zffalse;
    if(zfmAbs(yOffset) >= zfmAbs(xOffset))
    { // drag vertical
        if(yOffset > 0)
        {
            if(this->scrollableTop())
            {
                this->trackingParent = zfnull;
            }
            else if(this->trackingParent->d->scrollableTop())
            {
                stopTracking = zftrue;
            }
            else if(this->scrollableOrBounceableTop())
            {
                this->trackingParent = zfnull;
            }
            else if(this->trackingParent->d->scrollableOrBounceableTop())
            {
                stopTracking = zftrue;
            }
        }
        else if(yOffset < 0)
        {
            if(this->scrollableBottom())
            {
                this->trackingParent = zfnull;
            }
            else if(this->trackingParent->d->scrollableBottom())
            {
                stopTracking = zftrue;
            }
            else if(this->scrollableOrBounceableBottom())
            {
                this->trackingParent = zfnull;
            }
            else if(this->trackingParent->d->scrollableOrBounceableBottom())
            {
                stopTracking = zftrue;
            }
        }
    }
    else { // drag horizontal
        if(xOffset > 0)
        {
            if(this->scrollableLeft())
            {
                this->trackingParent = zfnull;
            }
            else if(this->trackingParent->d->scrollableLeft())
            {
                stopTracking = zftrue;
            }
            else if(this->scrollableOrBounceableLeft())
            {
                this->trackingParent = zfnull;
            }
            else if(this->trackingParent->d->scrollableOrBounceableLeft())
            {
                stopTracking = zftrue;
            }
        }
        else if(xOffset < 0)
        {
            if(this->scrollableRight())
            {
                this->trackingParent = zfnull;
            }
            else if(this->trackingParent->d->scrollableRight())
            {
                stopTracking = zftrue;
            }
            else if(this->scrollableOrBounceableRight())
            {
                this->trackingParent = zfnull;
            }
            else if(this->trackingParent->d->scrollableOrBounceableRight())
            {
                stopTracking = zftrue;
            }
        }
    }

    if(stopTracking)
    { // stop child's drag and restore parent's touch down
        void *mouseDownTmp = this->pimplOwner->implProtocol->mouseEventClone(nativeMouseEvent, zftrue, ZFUIMouseAction::e_MouseDown);

        void *evTmp = this->pimplOwner->implProtocol->mouseEventClone(nativeMouseEvent, zftrue, ZFUIMouseAction::e_MouseCancel);
        this->resolveMouseCancel(evTmp);
        this->pimplOwner->implProtocol->mouseEventCleanup(evTmp);

        this->mouseDownCleanup();
        mouseDownTmp = this->trackingParent->implProtocol->translateFromChildToParent(
            this->pimplOwner->scrollView->nativeImplView(),
            mouseDownTmp,
            x, y);
        if(this->trackingParent->d->trackingChild != zfnull)
        {
            this->pimplOwner->implProtocol->findTouchedChildScrollViewCleanup(this->trackingParent->d->trackingChild);
            this->trackingParent->d->trackingChild = zfnull;
        }
        this->trackingParent->d->dragState = _ZFP_ZFUIScrollViewImplHelperDragStateTracking;
        this->trackingParent->d->resolveMouseDown(mouseDownTmp);
        this->pimplOwner->implProtocol->mouseEventCleanup(mouseDownTmp);
    }
    else
    {
        this->resolveMouseMove(nativeMouseEvent);
    }
}
void _ZFP_ZFUIScrollViewImplHelperPrivate::trackMouseUp(ZF_IN void *nativeMouseEvent)
{
    #if _ZFP_ZFProtocolZFUIScrollView_DEBUG
    zfLogTrimT() << zfLogCurTimeString() << "[ScrollImpl]" << ZF_CALLER_LINE << this->pimplOwner->scrollView << "track" << ZFUIMouseAction::e_MouseUp
        << this->mouseEventPos(nativeMouseEvent);
    #endif
    ZFUIPoint mousePos = this->mouseEventPos(nativeMouseEvent);
    zffloat &x = mousePos.x;
    zffloat &y = mousePos.y;
    if(this->trackingChild != zfnull)
    {
        ZFUIScrollViewImplHelper *tmp = this->trackingChild;
        this->trackingChild->d->trackingParent = zfnull;
        this->pimplOwner->implProtocol->findTouchedChildScrollViewCleanup(this->trackingChild);
        this->trackingChild = zfnull;

        void *evTmp = this->pimplOwner->implProtocol->mouseEventClone(nativeMouseEvent);
        evTmp = this->pimplOwner->implProtocol->translateFromParentToChild(tmp->scrollView->nativeImplView(), evTmp, x, y);
        tmp->d->trackMouseUp(evTmp);
        this->pimplOwner->implProtocol->mouseEventCleanup(evTmp);
        return ;
    }
    this->resolveMouseUp(nativeMouseEvent);
}
void _ZFP_ZFUIScrollViewImplHelperPrivate::trackMouseCancel(ZF_IN void *nativeMouseEvent)
{
    #if _ZFP_ZFProtocolZFUIScrollView_DEBUG
    zfLogTrimT() << zfLogCurTimeString() << "[ScrollImpl]" << ZF_CALLER_LINE << this->pimplOwner->scrollView << "track" << ZFUIMouseAction::e_MouseCancel
        << this->mouseEventPos(nativeMouseEvent);
    #endif
    ZFUIPoint mousePos = this->mouseEventPos(nativeMouseEvent);
    zffloat &x = mousePos.x;
    zffloat &y = mousePos.y;
    if(this->trackingChild != zfnull)
    {
        ZFUIScrollViewImplHelper *tmp = this->trackingChild;
        this->trackingChild->d->trackingParent = zfnull;
        this->pimplOwner->implProtocol->findTouchedChildScrollViewCleanup(this->trackingChild);
        this->trackingChild = zfnull;

        void *evTmp = this->pimplOwner->implProtocol->mouseEventClone(nativeMouseEvent);
        evTmp = this->pimplOwner->implProtocol->translateFromParentToChild(tmp->scrollView->nativeImplView(), evTmp, x, y);
        tmp->d->trackMouseCancel(evTmp);
        this->pimplOwner->implProtocol->mouseEventCleanup(evTmp);
        return ;
    }
    this->resolveMouseCancel(nativeMouseEvent);
}

// ============================================================
// mouse resolve
void _ZFP_ZFUIScrollViewImplHelperPrivate::resolveMouseDown(ZF_IN void *nativeMouseEvent)
{
    #if _ZFP_ZFProtocolZFUIScrollView_DEBUG
    zfLogTrimT() << zfLogCurTimeString() << "[ScrollImpl]" << ZF_CALLER_LINE << this->pimplOwner->scrollView << "resolve" << ZFUIMouseAction::e_MouseDown
        << this->mouseEventPos(nativeMouseEvent);
    #endif
    ZFUIPoint mousePos = this->mouseEventPos(nativeMouseEvent);
    zffloat &x = mousePos.x;
    zffloat &y = mousePos.y;
    this->mouseDownCleanup();

    if(!this->scrollableOrBounceable())
    {
        this->dragState = _ZFP_ZFUIScrollViewImplHelperDragStateIgnored;
        this->processingChild = this->pimplOwner->implProtocol->findTouchedChild(x, y);

        if(this->processingChild != zfnull)
        {
            void *evTmp = this->pimplOwner->implProtocol->mouseEventClone(nativeMouseEvent);
            evTmp = this->pimplOwner->implProtocol->translateFromParentToChild(this->processingChild, evTmp, x, y);
            #if _ZFP_ZFProtocolZFUIScrollView_DEBUG
            zfLogTrimT() << zfLogCurTimeString() << "[ScrollImpl]" << ZF_CALLER_LINE << this->pimplOwner->scrollView << "forward" << ZFUIMouseAction::e_MouseDown
                << this->mouseEventPos(evTmp);
            #endif
            this->pimplOwner->implProtocol->mouseEventForward(this->processingChild, evTmp);
            this->pimplOwner->implProtocol->mouseEventCleanup(evTmp);
        }
        return ;
    }

    this->mouseDownSaved = this->pimplOwner->implProtocol->mouseEventClone(nativeMouseEvent);

    if(this->pimplOwner->scrollView->scrollViewState() != ZFUIScrollViewState::e_Idle)
    {
        if(this->processingChild != zfnull)
        {
            this->pimplOwner->implProtocol->findTouchedChildCleanup(this->processingChild);
            this->processingChild = zfnull;
        }
        this->dragState = _ZFP_ZFUIScrollViewImplHelperDragStateDragging;
        ZFPROTOCOL_ACCESS(ZFUIScrollView)->notifyScrollViewDragBegin(this->pimplOwner->scrollView, ZFUIPointMake(x, y), this->pimplOwner->implProtocol->nativeTime());
        return ;
    }

    this->processingChild = this->pimplOwner->implProtocol->findTouchedChild(x, y);
    if(this->processingChild != zfnull)
    {
        #if _ZFP_ZFProtocolZFUIScrollView_DEBUG
        zfLogTrimT() << zfLogCurTimeString() << "[ScrollImpl]" << ZF_CALLER_LINE << this->pimplOwner->scrollView << "track timer start";
        #endif
        this->pimplOwner->implProtocol->trackDelayStart((zftimet)200, this->pimplOwner);
    }
}
void _ZFP_ZFUIScrollViewImplHelperPrivate::resolveMouseMove(ZF_IN void *nativeMouseEvent)
{
    #if _ZFP_ZFProtocolZFUIScrollView_DEBUG
    zfLogTrimT() << zfLogCurTimeString() << "[ScrollImpl]" << ZF_CALLER_LINE << this->pimplOwner->scrollView << "resolve" << ZFUIMouseAction::e_MouseMove
        << this->mouseEventPos(nativeMouseEvent);
    #endif
    ZFUIPoint mousePos = this->mouseEventPos(nativeMouseEvent);
    zffloat &x = mousePos.x;
    zffloat &y = mousePos.y;
    if(this->dragState == _ZFP_ZFUIScrollViewImplHelperDragStateDragging)
    {
        ZFPROTOCOL_ACCESS(ZFUIScrollView)->notifyScrollViewDrag(this->pimplOwner->scrollView, ZFUIPointMake(x, y), this->pimplOwner->implProtocol->nativeTime());
        return ;
    }
    else if(this->dragState == _ZFP_ZFUIScrollViewImplHelperDragStateIgnored)
    {
        if(this->processingChild != zfnull)
        {
            void *evTmp = this->pimplOwner->implProtocol->mouseEventClone(nativeMouseEvent);
            evTmp = this->pimplOwner->implProtocol->translateFromParentToChild(this->processingChild, evTmp, x, y);
            #if _ZFP_ZFProtocolZFUIScrollView_DEBUG
            zfLogTrimT() << zfLogCurTimeString() << "[ScrollImpl]" << ZF_CALLER_LINE << this->pimplOwner->scrollView << "forward" << ZFUIMouseAction::e_MouseMove
                << this->mouseEventPos(evTmp);
            #endif
            this->pimplOwner->implProtocol->mouseEventForward(this->processingChild, evTmp);
            this->pimplOwner->implProtocol->mouseEventCleanup(evTmp);
        }
        return ;
    }

    ZFUIPoint mouseDownPos = this->mouseEventPos(this->mouseDownSaved);
    int xOffset = (int)(x - mouseDownPos.x);
    int yOffset = (int)(y - mouseDownPos.y);
    int xOffsetAbs = zfmAbs(xOffset);
    int yOffsetAbs = zfmAbs(yOffset);
    if(xOffsetAbs <= this->pimplOwner->nativeScrollTolerance && yOffsetAbs <= this->pimplOwner->nativeScrollTolerance) {
        return ;
    }

    zfbool childIntercepted = zffalse;
    if(this->processingChild != zfnull)
    {
        if(yOffsetAbs >= xOffsetAbs * 1.5f)
        { // drag vertical
            if(yOffset > 0)
            {
                if(!this->scrollableOrBounceableTop())
                {
                    childIntercepted = true;
                }
            }
            else
            {
                if(!this->scrollableOrBounceableBottom())
                {
                    childIntercepted = true;
                }
            }
        }
        else if(xOffsetAbs >= yOffsetAbs * 1.5f)
        { // drag horizontal
            if(xOffset > 0)
            {
                if(!this->scrollableOrBounceableLeft())
                {
                    childIntercepted = true;
                }
            }
            else
            {
                if(!this->scrollableOrBounceableRight())
                {
                    childIntercepted = true;
                }
            }
        }
    }

    if(childIntercepted)
    {
        this->dragState = _ZFP_ZFUIScrollViewImplHelperDragStateIgnored;
        this->mouseDownResend();
        void *evTmp = this->pimplOwner->implProtocol->mouseEventClone(nativeMouseEvent);
        evTmp = this->pimplOwner->implProtocol->translateFromParentToChild(this->processingChild, evTmp, x, y);
        #if _ZFP_ZFProtocolZFUIScrollView_DEBUG
        zfLogTrimT() << zfLogCurTimeString() << "[ScrollImpl]" << ZF_CALLER_LINE << this->pimplOwner->scrollView << "forward" << ZFUIMouseAction::e_MouseMove
            << this->mouseEventPos(evTmp);
        #endif
        this->pimplOwner->implProtocol->mouseEventForward(this->processingChild, evTmp);
        this->pimplOwner->implProtocol->mouseEventCleanup(evTmp);
    }
    else
    {
        this->dragState = _ZFP_ZFUIScrollViewImplHelperDragStateDragging;

        ZFUIPoint mouseDownPos = this->mouseEventPos(this->mouseDownSaved);
        this->mouseDownCleanup();
        ZFPROTOCOL_ACCESS(ZFUIScrollView)->notifyScrollViewDragBegin(this->pimplOwner->scrollView, ZFUIPointMake(mouseDownPos.x, mouseDownPos.y), this->pimplOwner->implProtocol->nativeTime());
        ZFPROTOCOL_ACCESS(ZFUIScrollView)->notifyScrollViewDrag(this->pimplOwner->scrollView, ZFUIPointMake(x, y), this->pimplOwner->implProtocol->nativeTime());
    }
}
void _ZFP_ZFUIScrollViewImplHelperPrivate::resolveMouseUp(ZF_IN void *nativeMouseEvent)
{
    #if _ZFP_ZFProtocolZFUIScrollView_DEBUG
    zfLogTrimT() << zfLogCurTimeString() << "[ScrollImpl]" << ZF_CALLER_LINE << this->pimplOwner->scrollView << "resolve" << ZFUIMouseAction::e_MouseUp
        << this->mouseEventPos(nativeMouseEvent);
    #endif
    ZFUIPoint mousePos = this->mouseEventPos(nativeMouseEvent);
    zffloat &x = mousePos.x;
    zffloat &y = mousePos.y;

    if(this->dragState == _ZFP_ZFUIScrollViewImplHelperDragStateDragging)
    {
        ZFPROTOCOL_ACCESS(ZFUIScrollView)->notifyScrollViewDragEnd(this->pimplOwner->scrollView, this->pimplOwner->implProtocol->nativeTime(), zftrue);
        return ;
    }
    else if(this->dragState == _ZFP_ZFUIScrollViewImplHelperDragStateIgnored)
    {
        if(this->processingChild != zfnull)
        {
            void *evTmp = this->pimplOwner->implProtocol->mouseEventClone(nativeMouseEvent);
            evTmp = this->pimplOwner->implProtocol->translateFromParentToChild(this->processingChild, evTmp, x, y);
            #if _ZFP_ZFProtocolZFUIScrollView_DEBUG
            zfLogTrimT() << zfLogCurTimeString() << "[ScrollImpl]" << ZF_CALLER_LINE << this->pimplOwner->scrollView << "forward" << ZFUIMouseAction::e_MouseUp
                << this->mouseEventPos(evTmp);
            #endif
            this->pimplOwner->implProtocol->mouseEventForward(this->processingChild, evTmp);
            this->pimplOwner->implProtocol->mouseEventCleanup(evTmp);
        }
        return ;
    }

    // mouse down then up quickly
    this->dragState = _ZFP_ZFUIScrollViewImplHelperDragStateTracking;
    if(this->processingChild != zfnull)
    {
        this->mouseDownResend();
        void *evTmp = this->pimplOwner->implProtocol->mouseEventClone(nativeMouseEvent);
        evTmp = this->pimplOwner->implProtocol->translateFromParentToChild(this->processingChild, evTmp, x, y);
        #if _ZFP_ZFProtocolZFUIScrollView_DEBUG
        zfLogTrimT() << zfLogCurTimeString() << "[ScrollImpl]" << ZF_CALLER_LINE << this->pimplOwner->scrollView << "forward" << ZFUIMouseAction::e_MouseUp
            << this->mouseEventPos(evTmp);
        #endif
        this->pimplOwner->implProtocol->mouseEventForward(this->processingChild, evTmp);
        this->pimplOwner->implProtocol->mouseEventCleanup(evTmp);
    }
    else {
        this->mouseDownCleanup();
    }
}
void _ZFP_ZFUIScrollViewImplHelperPrivate::resolveMouseCancel(ZF_IN void *nativeMouseEvent)
{
    #if _ZFP_ZFProtocolZFUIScrollView_DEBUG
    zfLogTrimT() << zfLogCurTimeString() << "[ScrollImpl]" << ZF_CALLER_LINE << this->pimplOwner->scrollView << "resolve" << ZFUIMouseAction::e_MouseCancel
        << this->mouseEventPos(nativeMouseEvent);
    #endif
    ZFUIPoint mousePos = this->mouseEventPos(nativeMouseEvent);
    zffloat &x = mousePos.x;
    zffloat &y = mousePos.y;

    if(this->dragState == _ZFP_ZFUIScrollViewImplHelperDragStateDragging)
    {
        ZFPROTOCOL_ACCESS(ZFUIScrollView)->notifyScrollViewDragEnd(this->pimplOwner->scrollView, this->pimplOwner->implProtocol->nativeTime(), zffalse);
        return ;
    }
    else if(this->dragState == _ZFP_ZFUIScrollViewImplHelperDragStateIgnored)
    {
        if(this->processingChild != zfnull)
        {
            void *evTmp = this->pimplOwner->implProtocol->mouseEventClone(nativeMouseEvent);
            evTmp = this->pimplOwner->implProtocol->translateFromParentToChild(this->processingChild, evTmp, x, y);
            #if _ZFP_ZFProtocolZFUIScrollView_DEBUG
            zfLogTrimT() << zfLogCurTimeString() << "[ScrollImpl]" << ZF_CALLER_LINE << this->pimplOwner->scrollView << "forward" << ZFUIMouseAction::e_MouseCancel
                << this->mouseEventPos(evTmp);
            #endif
            this->pimplOwner->implProtocol->mouseEventForward(this->processingChild, evTmp);
            this->pimplOwner->implProtocol->mouseEventCleanup(evTmp);
        }
        return ;
    }

    // mouse down then cancel quickly
    this->dragState = _ZFP_ZFUIScrollViewImplHelperDragStateTracking;
    this->mouseDownCleanup();
}

ZF_NAMESPACE_GLOBAL_END

