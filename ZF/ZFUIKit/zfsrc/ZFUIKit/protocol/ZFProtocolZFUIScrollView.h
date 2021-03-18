/**
 * @file ZFProtocolZFUIScrollView.h
 * @brief protocol for ZFUIScrollView
 */

#ifndef _ZFI_ZFProtocolZFUIScrollView_h_
#define _ZFI_ZFProtocolZFUIScrollView_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFUIKit/ZFUIScrollView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd ZFPROTOCOL_INTERFACE_CLASS(ZFUIScrollView);
extern ZF_ENV_EXPORT zftimet _ZFP_ZFProtocolZFUIScrollView_scrollAnimationStart(ZF_IN ZFPROTOCOL_INTERFACE_CLASS(ZFUIScrollView) *impl,
                                                                                ZF_IN ZFUIScrollView *scrollView,
                                                                                ZF_IN zftimet recommendTimerInterval);
extern ZF_ENV_EXPORT void _ZFP_ZFProtocolZFUIScrollView_scrollAnimationStop(ZF_IN ZFPROTOCOL_INTERFACE_CLASS(ZFUIScrollView) *impl,
                                                                            ZF_IN ZFUIScrollView *scrollView);

/**
 * @brief protocol for ZFUIScrollView
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFUIScrollView)
    /**
     * @brief create native scroll view
     */
    virtual void *nativeScrollViewCreate(ZF_IN ZFUIScrollView *scrollView) zfpurevirtual;
    /**
     * @brief destroy native scroll view
     */
    virtual void nativeScrollViewDestroy(ZF_IN ZFUIScrollView *scrollView,
                                         ZF_IN void *nativeScrollView) zfpurevirtual;

public:
    /**
     * @brief see #ZFUIScrollView::scrollEnable
     */
    virtual void scrollEnable(ZF_IN ZFUIScrollView *scrollView,
                              ZF_IN zfbool scrollEnable) zfpurevirtual;
    /**
     * @brief see #ZFUIScrollView::scrollBounceHorizontal
     *
     * actual bounce logic would be done by #ZFUIScrollView for you,
     * this is only a hint for implementation,
     * for example,
     * a none bounceable scroll view won't start drag action
     */
    virtual void scrollBounce(ZF_IN ZFUIScrollView *scrollView,
                              ZF_IN zfbool scrollBounceHorizontal,
                              ZF_IN zfbool scrollBounceVertical,
                              ZF_IN zfbool scrollBounceHorizontalAlways,
                              ZF_IN zfbool scrollBounceVerticalAlways) zfpurevirtual;
    /**
     * @brief change content's frame
     *
     * scroll view's content frame logic, including scrolling animation logic,
     * would be done by #ZFUIScrollView for you,
     * what you should do, is to supply proper mouse intercept logic,
     * make sure mouse down/up is paired both to the scroll view and its children,
     * and notify necessary events:
     * -  #notifyScrollViewDragBegin
     * -  #notifyScrollViewDrag
     * -  #notifyScrollViewDragEnd
     */
    virtual void scrollContentFrame(ZF_IN ZFUIScrollView *scrollView,
                                    ZF_IN const ZFUIRect &frame) zfpurevirtual;
    /**
     * @brief for impl to update scroll content frame without activating any other event
     */
    zffinal void scrollContentFrameUpdateByImpl(ZF_IN ZFUIScrollView *scrollView,
                                                ZF_IN const ZFUIRect &frame)
    {
        ZFUIRect frameNew = ZFUIRectApplyScaleReversely(frame, scrollView->scaleFixed());
        frameNew.x -= scrollView->scrollAreaMargin().left;
        frameNew.y -= scrollView->scrollAreaMargin().top;
        scrollView->_ZFP_ZFUIScrollView_scrollContentFrameByImpl(frameNew);
    }
    /**
     * @brief used to start scroll animation for performance
     *
     * you should schedule a timer task that repeatly call #notifyScrollViewScrollAnimation,
     * return a relative time in miliseconds to represents scroll animation's timing\n
     * timer interval should be decided by implementation internally,
     * according to runtime performance,
     * and typically 50 miliseconds would suit for most cases\n
     * \n
     * by default, we would use #ZFTimer to supply a default implementation,
     * which may have worse performance,
     * and depends on #ZFTimer and #ZFTime
     */
    virtual zftimet scrollAnimationStart(ZF_IN ZFUIScrollView *scrollView,
                                         ZF_IN zftimet recommendTimerInterval)
    {
        return _ZFP_ZFProtocolZFUIScrollView_scrollAnimationStart(this, scrollView, recommendTimerInterval);
    }
    /**
     * @brief see #scrollAnimationStart
     */
    virtual void scrollAnimationStop(ZF_IN ZFUIScrollView *scrollView)
    {
        _ZFP_ZFProtocolZFUIScrollView_scrollAnimationStop(this, scrollView);
    }

    // ============================================================
    // children
public:
    /**
     * @brief add child view
     *
     * you should not retain or release those views\n
     * atIndex is ensured valid, range [0, curChildCount],
     * meaning if 0 add to first and if curChildCount add to last
     */
    virtual void scrollChildAdd(ZF_IN ZFUIScrollView *parent,
                                ZF_IN ZFUIView *child,
                                ZF_IN zfindex atIndex) zfpurevirtual;
    /**
     * @brief remove child view
     *
     * you should not retain or release those views\n
     * atIndex is ensured valid, range [0, curChildCount)
     */
    virtual void scrollChildRemove(ZF_IN ZFUIScrollView *parent,
                                   ZF_IN ZFUIView *child,
                                   ZF_IN zfindex atIndex) zfpurevirtual;
    /**
     * @brief called to remove all children during parent dealloc for performance
     *
     * see #ZFUIView::implChildOnRemoveAllForDealloc
     */
    virtual void scrollChildRemoveAllForDealloc(ZF_IN ZFUIScrollView *parent) zfpurevirtual;

    // ============================================================
    // callbacks that implementations must notify
public:
    /**
     * @brief see #scrollContentFrame for how to implements scroll logic
     */
    zffinal void notifyScrollViewDragBegin(ZF_IN ZFUIScrollView *scrollView,
                                           ZF_IN const ZFUIPoint &mousePos,
                                           ZF_IN zftimet mouseTime)
    {
        scrollView->_ZFP_ZFUIScrollView_notifyDragBegin(ZFUIPointApplyScaleReversely(mousePos, scrollView->scaleFixed()), mouseTime);
    }
    /**
     * @brief see #scrollContentFrame for how to implements scroll logic
     */
    zffinal void notifyScrollViewDrag(ZF_IN ZFUIScrollView *scrollView,
                                      ZF_IN const ZFUIPoint &mousePos,
                                      ZF_IN zftimet mouseTime)
    {
        scrollView->_ZFP_ZFUIScrollView_notifyDrag(ZFUIPointApplyScaleReversely(mousePos, scrollView->scaleFixed()), mouseTime);
    }
    /**
     * @brief see #scrollContentFrame for how to implements scroll logic
     */
    zffinal void notifyScrollViewDragEnd(ZF_IN ZFUIScrollView *scrollView,
                                         ZF_IN zftimet mouseTime,
                                         ZF_IN zfbool needScrollAni)
    {
        scrollView->_ZFP_ZFUIScrollView_notifyDragEnd(mouseTime, needScrollAni);
    }
    /**
     * @brief see #scrollAnimationStart for how to implements scroll animation logic
     */
    zffinal void notifyScrollViewScrollAnimation(ZF_IN ZFUIScrollView *scrollView,
                                                 ZF_IN zftimet relativeTimeInMiliseconds)
    {
        scrollView->_ZFP_ZFUIScrollView_notifyScrollAnimation(relativeTimeInMiliseconds);
    }
ZFPROTOCOL_INTERFACE_END(ZFUIScrollView)

// ============================================================
zfclassFwd ZFUIScrollViewImplHelper;
/** @brief see #ZFUIScrollViewImplHelper */
zfclassNotPOD ZF_ENV_EXPORT ZFUIScrollViewImplHelperProtocol
{
public:
    virtual ~ZFUIScrollViewImplHelperProtocol(void) {}

public:
    /** @brief see #ZFUIScrollViewImplHelper */
    virtual zftimet nativeTime(void) zfpurevirtual;
    /** @brief see #ZFUIScrollViewImplHelper */
    virtual void mouseEventPos(ZF_OUT ZFUIPoint &ret, ZF_IN void *nativeMouseEvent) zfpurevirtual;
    /** @brief see #ZFUIScrollViewImplHelper */
    virtual void *mouseEventClone(ZF_IN void *nativeMouseEvent,
                                  ZF_IN_OPT zfbool changeMouseAction = zffalse,
                                  ZF_IN_OPT ZFUIMouseActionEnum mouseAction = ZFUIMouseAction::e_MouseCancel) zfpurevirtual;
    /** @brief see #ZFUIScrollViewImplHelper */
    virtual void mouseEventCleanup(ZF_IN void *nativeMouseEvent) zfpurevirtual;
    /**
     * @brief see #ZFUIScrollViewImplHelper
     *
     * the forwarded mouse event must be ignored from all scroll view's intercept step
     */
    virtual void mouseEventForward(ZF_IN void *nativeChild,
                                   ZF_IN void *nativeMouseEvent) zfpurevirtual;

    /** @brief see #ZFUIScrollViewImplHelper */
    virtual ZFUIScrollViewImplHelper *findTouchedChildScrollView(ZF_IN zfint x, ZF_IN zfint y) zfpurevirtual;
    /** @brief see #ZFUIScrollViewImplHelper */
    virtual void findTouchedChildScrollViewCleanup(ZF_IN void *nativeChild) zfpurevirtual;
    /** @brief see #ZFUIScrollViewImplHelper */
    virtual void *findTouchedChild(ZF_IN zfint x, ZF_IN zfint y) zfpurevirtual;
    /** @brief see #ZFUIScrollViewImplHelper */
    virtual void findTouchedChildCleanup(ZF_IN void *nativeChild) zfpurevirtual;

    /**
     * @brief see #ZFUIScrollViewImplHelper, after time out, you must invoke #ZFUIScrollViewImplHelper::trackDelayNotifyTimeout
     *
     * this method has default implementation using #ZFTimer,
     * you may supply your own logic by override this method and #trackDelayStop without calling super
     */
    virtual void trackDelayStart(ZF_IN zftimet timeoutMiliSeconds,
                                 ZF_IN ZFUIScrollViewImplHelper *owner);
    /** @brief see #ZFUIScrollViewImplHelper */
    virtual void trackDelayStop(ZF_IN ZFUIScrollViewImplHelper *owner);

    /**
     * @brief see #ZFUIScrollViewImplHelper
     *
     * return original native mouse event or create new copy if not able to modify,
     * if you created new native mouse event and return, you should destroy old one manually by #mouseEventCleanup
     */
    virtual void *translateFromParentToChild(ZF_IN void *nativeChild,
                                             ZF_IN_OUT void *nativeMouseEvent,
                                             ZF_IN zfint const &xInParent, ZF_IN zfint const &yInParent) zfpurevirtual;
    /** @brief see #translateFromParentToChild */
    virtual void *translateFromChildToParent(ZF_IN void *nativeChild,
                                             ZF_IN_OUT void *nativeMouseEvent,
                                             ZF_IN zfint const &xInChild, ZF_IN zfint const &yInChild) zfpurevirtual;
};
zfclassFwd _ZFP_ZFUIScrollViewImplHelperPrivate;
/**
 * @brief util class to achieve #ZFUIScrollView's drag tracking logic
 *
 * to use this util class, you native view must support:
 * -  intercept mouse event
 * -  simulate mouse event and event forwarding
 *
 * to use it:
 * -  implements all methods in ZFUIScrollViewImplHelperProtocol
 * -  have all ZFUIScrollViewImplHelper's member been setup properly
 * -  intercept all mouse events by #ZFUIScrollViewImplHelper::interceptMouse
 */
zfclassNotPOD ZF_ENV_EXPORT ZFUIScrollViewImplHelper
{
    ZFCLASS_DISALLOW_COPY_CONSTRUCTOR(ZFUIScrollViewImplHelper)

public:
    /** @brief see #ZFUIScrollViewImplHelper */
    ZFUIScrollView *scrollView;
    /** @brief see #ZFUIScrollViewImplHelper */
    ZFUIScrollViewImplHelperProtocol *implProtocol;
    /** @brief see #ZFUIScrollViewImplHelper */
    zfint nativeScrollTolerance;

public:
    /** @brief see #ZFUIScrollViewImplHelper */
    ZFUIScrollViewImplHelper(void);
    ~ZFUIScrollViewImplHelper(void);

public:
    /** @brief see #ZFUIScrollViewImplHelper */
    void interceptMouse(ZF_IN void *nativeMouseEvent,
                        ZF_IN ZFUIMouseActionEnum mouseAction);
    /** @brief see #ZFUIScrollViewImplHelper */
    void trackDelayNotifyTimeout(void);

private:
    _ZFP_ZFUIScrollViewImplHelperPrivate *d;
    friend zfclassFwd _ZFP_ZFUIScrollViewImplHelperPrivate;
    friend zfclassFwd ZFUIScrollViewImplHelperProtocol;
    ZFTimer *_trackDelayDefaultImplTimer;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFUIScrollView_h_

