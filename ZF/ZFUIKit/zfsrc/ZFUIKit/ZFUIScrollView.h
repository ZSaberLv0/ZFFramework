/**
 * @file ZFUIScrollView.h
 * @brief scroll view
 */

#ifndef _ZFI_ZFUIScrollView_h_
#define _ZFI_ZFUIScrollView_h_

#include "ZFUIView.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief state for #ZFUIScrollView
 */
ZFENUM_BEGIN(ZFUIScrollViewState)
    ZFENUM_VALUE(Idle)
    ZFENUM_VALUE(Dragging)
    ZFENUM_VALUE(Scrolling)
ZFENUM_SEPARATOR(ZFUIScrollViewState)
    ZFENUM_VALUE_REGISTER(Idle)
    ZFENUM_VALUE_REGISTER(Dragging)
    ZFENUM_VALUE_REGISTER(Scrolling)
ZFENUM_END(ZFUIScrollViewState)

zfclassFwd _ZFP_ZFUIScrollViewPrivate;
// ============================================================
// ZFUIScrollView
/**
 * @brief native scroll view
 *
 * note that background view of scroll view and all other views behind scroll view
 * are ensured won't receive mouse event
 */
zfclass ZF_ENV_EXPORT ZFUIScrollView : zfextends ZFUIView
{
    ZFOBJECT_DECLARE(ZFUIScrollView, ZFUIView)
    ZFSTYLE_DEFAULT_DECLARE(ZFUIScrollView)

public:
    // ============================================================
    // event
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when drag begin
     */
    ZFOBSERVER_EVENT(ScrollOnDragBegin)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when drag
     */
    ZFOBSERVER_EVENT(ScrollOnDrag)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when drag end
     */
    ZFOBSERVER_EVENT(ScrollOnDragEnd)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when scroll begin
     */
    ZFOBSERVER_EVENT(ScrollOnScrollBegin)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when scroll
     */
    ZFOBSERVER_EVENT(ScrollOnScroll)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when scroll end
     */
    ZFOBSERVER_EVENT(ScrollOnScrollEnd)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when #scrollArea changed
     */
    ZFOBSERVER_EVENT(ScrollAreaOnChange)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when #scrollContentFrame changed
     */
    ZFOBSERVER_EVENT(ScrollContentFrameOnChange)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when auto scroll started
     */
    ZFOBSERVER_EVENT(ScrollAutoScrollOnStart)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when auto scroll stopped
     */
    ZFOBSERVER_EVENT(ScrollAutoScrollOnStop)
    /**
     * @brief see #ZFObject::observerNotify
     *
     * called when scrolled by user, activated if any of these methods called manually or event occurred:
     * -  #scrollByPoint
     * -  #scrollBySpeed
     * -  #scrollContentFrame or #scrollContentFrameAnimated
     * -  #autoScrollStartX series
     * -  drag begin
     *
     * "called manually" means not overrided by #scrollOverride
     */
    ZFOBSERVER_EVENT(ScrollOnScrolledByUser)

public:
    // ============================================================
    // properties
    /**
     * @brief whether the scroll view is scrollable, zftrue by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, scrollEnable,
                                zftrue)
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(zfbool, scrollEnable)

    /**
     * @brief if content is larger than scroll view, make it scrollable even if reaches edge, true by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, scrollBounceHorizontal,
                                zftrue)
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(zfbool, scrollBounceHorizontal)
    /**
     * @brief if content is larger than scroll view, make it scrollable even if reaches edge, true by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, scrollBounceVertical,
                                zftrue)
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(zfbool, scrollBounceVertical)
    /**
     * @brief scrollable even if content is not larger than scrollview, false by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, scrollBounceHorizontalAlways,
                                zffalse)
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(zfbool, scrollBounceHorizontalAlways)
    /**
     * @brief scrollable even if content is not larger than scrollview, false by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, scrollBounceVerticalAlways,
                                zffalse)
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(zfbool, scrollBounceVerticalAlways)

    /**
     * @brief whether limit scroll horizontal or vertical only, false by default
     */
    ZFPROPERTY_ASSIGN_WITH_INIT(zfbool, scrollAlignToAxis,
                                zffalse)
    /**
     * @brief whether align to page when scroll, false by default
     *
     * if true, during scrolling, scroll view would try to stop at integer multiples of scroll view's size,
     * excluding scroll thumb's size\n
     * if align to page, #scrollAlignToAxis would be activated no matter what value of it
     */
    ZFPROPERTY_ASSIGN(zfbool, scrollAlignToPageHorizontal)
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(zfbool, scrollAlignToPageHorizontal)
    /**
     * @brief whether align to page when scroll, false by default
     *
     * if true, during scrolling, scroll view would try to stop at integer multiples of scroll view's size,
     * excluding scroll thumb's size\n
     * if align to page, #scrollAlignToAxis would be activated no matter what value of it
     */
    ZFPROPERTY_ASSIGN(zfbool, scrollAlignToPageVertical)
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(zfbool, scrollAlignToPageVertical)

    /**
     * @brief scroll view's content frame
     *
     * change this value would have no scroll animation,
     * and any of previous scroll animation would be stopped\n
     * use #ZFUIScrollView::scrollContentFrameAnimated if you want to start scroll animation,
     * or #ZFUIScrollView::scrollContentFrameUpdate if you want to change frame
     * and keep previous scroll animation
     * @note the rect's 0 point starts from #nativeImplViewMargin + #scrollAreaMargin,
     *   instead of the view's frame
     */
    ZFPROPERTY_ASSIGN(ZFUIRect, scrollContentFrame)
    ZFPROPERTY_OVERRIDE_ON_ATTACH_DECLARE(ZFUIRect, scrollContentFrame)
    zffinal void _ZFP_ZFUIScrollView_scrollContentFrameByImpl(ZF_IN const ZFUIRect &rect);

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);
    zfoverride
    virtual void objectOnDeallocPrepare(void);

protected:
    zfoverride
    virtual void objectInfoOnAppend(ZF_IN_OUT zfstring &ret);

    // ============================================================
    // override ZFUIView
protected:
    zfoverride
    virtual void implChildOnAdd(ZF_IN ZFUIView *child,
                                ZF_IN zfindex virtualIndex,
                                ZF_IN ZFUIViewChildLayerEnum childLayer,
                                ZF_IN zfindex childLayerIndex);
    zfoverride
    virtual void implChildOnRemove(ZF_IN ZFUIView *child,
                                   ZF_IN zfindex virtualIndex,
                                   ZF_IN ZFUIViewChildLayerEnum childLayer,
                                   ZF_IN zfindex childLayerIndex);
    zfoverride
    virtual void implChildOnRemoveAllForDealloc(void);

protected:
    zfoverride
    virtual void layoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds);
    /**
     * @brief override ZFUIView to layout scroll view's internal view and content view
     *
     * usually you should not override this method,
     * if necessary, you must call super
     */
    zfoverride
    virtual void layoutOnLayout(ZF_IN const ZFUIRect &bounds);
    zfoverride
    virtual void layoutOnLayoutFinish(ZF_IN const ZFUIRect &bounds);
    zfoverride
    virtual inline void layoutChildOffsetOnUpdate(ZF_IN_OUT ZFUIPoint &ret)
    {
        ret.x += this->scrollContentFrame().x;
        ret.y += this->scrollContentFrame().y;
    }

    /**
     * @brief (ZFTAG_LIMITATION) due to implementations limitation, ZFUIScrollView may or may not have mouse event callbacks
     */
    zfoverride
    virtual void viewEventOnMouseEvent(ZF_IN ZFUIMouseEvent *mouseEvent);
    zfoverride
    virtual void viewEventOnWheelEvent(ZF_IN ZFUIWheelEvent *wheelEvent);

    zfoverride
    virtual void nativeImplViewMarginOnUpdate(void);

    // ============================================================
    // util methods
public:
    /**
     * @brief cancel over scroll and ensure content offset in range
     */
    ZFMETHOD_DECLARE_0(void, scrollToFitRange)
    /**
     * @brief scroll child to proper position so that it's visible to user
     *
     * it's your responsibility to ensure th child is indeed this view's child
     */
    ZFMETHOD_DECLARE_3(void, scrollChildToVisible,
                       ZFMP_IN(ZFUIView *, child),
                       ZFMP_IN_OPT(const ZFUIMargin &, margin, ZFUIMarginMake(ZFUIGlobalStyle::DefaultStyle()->itemMargin())),
                       ZFMP_IN_OPT(zfbool, scrollWithAni, zftrue))

    // ============================================================
    // scroll area
public:
    /**
     * @brief margin for impl to modify scrollable area, #ZFUIMarginZero by default
     */
    ZFMETHOD_DECLARE_1(void, scrollAreaMarginAdd,
                       ZFMP_IN(const ZFUIMargin &, margin))
    /**
     * @brief see #scrollAreaMarginAdd
     */
    ZFMETHOD_DECLARE_1(void, scrollAreaMarginRemove,
                       ZFMP_IN(const ZFUIMargin &, margin))
    /**
     * @brief see #scrollAreaMarginAdd
     */
    ZFMETHOD_DECLARE_0(const ZFUIMargin &, scrollAreaMargin)
    /**
     * @brief scrollable frame excluding #scrollAreaMargin,
     *   valid only if layouted
     */
    ZFMETHOD_DECLARE_0(const ZFUIRect &, scrollArea)

    // ============================================================
    // scroll thumb
public:
    /**
     * @brief change current scroll thumb, null to disable
     *
     * by default, #ZFUIScrollThumbHorizontalClass and #ZFUIScrollThumbHorizontalClass
     * would be used
     */
    ZFMETHOD_DECLARE_1(void, scrollThumbHorizontalClass,
                       ZFMP_IN(const ZFClass *, cls))
    /**
     * @brief see #scrollThumbHorizontalClass
     */
    ZFMETHOD_DECLARE_0(const ZFClass *, scrollThumbHorizontalClass)
    /**
     * @brief see #scrollThumbHorizontalClass
     */
    ZFMETHOD_DECLARE_1(void, scrollThumbVerticalClass,
                       ZFMP_IN(const ZFClass *, cls))
    /**
     * @brief see #scrollThumbHorizontalClass
     */
    ZFMETHOD_DECLARE_0(const ZFClass *, scrollThumbVerticalClass)
protected:
    /**
     * @brief called to init scroll thumb
     */
    virtual void scrollThumbHorizontalOnInit(void);
    /**
     * @brief called to init scroll thumb
     */
    virtual void scrollThumbVerticalOnInit(void);

    // ============================================================
    // scroll control
public:
    /**
     * @brief class for internal scroller, must be type of #ZFUIScroller,
     *   #ZFUIScrollerClass by default
     *
     * scroller can only be changed by #ZFUIScrollerClass,
     * and only affect newly created scroll view
     */
    ZFMETHOD_DECLARE_0(const ZFClass *, scrollerClass)

public:
    /**
     * @brief util method to get offset of the scroll content
     *
     * positive if normal scroll, negative if bouncing
     */
    ZFMETHOD_DECLARE_0(zfint, scrollContentOffsetLeft)
    /** @brief see #scrollContentOffsetLeft */
    ZFMETHOD_DECLARE_0(zfint, scrollContentOffsetTop)
    /** @brief see #scrollContentOffsetLeft */
    ZFMETHOD_DECLARE_0(zfint, scrollContentOffsetRight)
    /** @brief see #scrollContentOffsetLeft */
    ZFMETHOD_DECLARE_0(zfint, scrollContentOffsetBottom)

public:
    /**
     * @brief animated change scroll content frame
     *
     * this is a util method to combine #scrollContentFrameUpdate
     * and #scrollByPoint
     */
    ZFMETHOD_DECLARE_1(void, scrollContentFrameAnimated,
                       ZFMP_IN(const ZFUIRect &, scrollContentFrame))
    /**
     * @brief change scroll content frame without interrupt current scroll animation
     */
    ZFMETHOD_DECLARE_1(void, scrollContentFrameUpdate,
                       ZFMP_IN(const ZFUIRect &, scrollContentFrame))
    /**
     * @brief animated scroll to desired position
     */
    ZFMETHOD_DECLARE_2(void, scrollByPoint,
                       ZFMP_IN(zfint, xPos),
                       ZFMP_IN(zfint, yPos))
    /**
     * @brief return end point of #scrollByPoint, or current content offset if not scrolling
     */
    ZFMETHOD_DECLARE_0(ZFUIPoint, scrollByPointEndPoint)
    /**
     * @brief scroll by desired initial speed, in pixels per second
     */
    ZFMETHOD_DECLARE_2(void, scrollBySpeed,
                       ZFMP_IN(zfint, xSpeedInPixelsPerSecond),
                       ZFMP_IN(zfint, ySpeedInPixelsPerSecond))
    /**
     * @brief return current speed of #scrollBySpeed
     */
    ZFMETHOD_DECLARE_0(zfint, scrollBySpeedCurrentSpeedX)
    /**
     * @brief return current speed of #scrollBySpeed
     */
    ZFMETHOD_DECLARE_0(zfint, scrollBySpeedCurrentSpeedY)
    /**
     * @brief return predicted end point of #scrollBySpeed, or current content offset if not scrolling,
     *   valid only if isn't bouncing and won't bouncing when stop
     */
    ZFMETHOD_DECLARE_0(ZFUIPoint, scrollBySpeedEndPointPredicted)

    /**
     * @brief #scrollByPointEndPoint or #scrollBySpeedEndPointPredicted depending on scroll type
     */
    ZFMETHOD_DECLARE_0(ZFUIPoint, scrollEndPointPredicted)

public:
    /**
     * @brief start an auto scroll with specified speed
     *
     * auto scroll would be stopped if:
     * -  #autoScrollStopX/#autoScrollStopY is called
     * -  drag event occurred
     * -  #scrollByPoint/#scrollContentFrameAnimated occurred
     * -  scroll reached content's edge
     */
    ZFMETHOD_DECLARE_1(void, autoScrollStartX,
                       ZFMP_IN(zfint, speedInPixelsPerSecond))
    /**
     * @brief see #autoScrollStartX
     */
    ZFMETHOD_DECLARE_1(void, autoScrollStartY,
                       ZFMP_IN(zfint, speedInPixelsPerSecond))
    /**
     * @brief see #autoScrollStartX
     */
    ZFMETHOD_DECLARE_0(void, autoScrollStopX)
    /**
     * @brief see #autoScrollStartX
     */
    ZFMETHOD_DECLARE_0(void, autoScrollStopY)
    /**
     * @brief current auto scroll speed, 0 means not auto scrolling, see #autoScrollStartX
     */
    ZFMETHOD_DECLARE_0(zfint, autoScrollSpeedX)
    /**
     * @brief current auto scroll speed, 0 means not auto scrolling, see #autoScrollStartX
     */
    ZFMETHOD_DECLARE_0(zfint, autoScrollSpeedY)

public:
    /**
     * @brief simulate drag
     */
    ZFMETHOD_DECLARE_2(void, scrollSimulateDragBegin,
                       ZFMP_IN(const ZFUIPoint &, mousePos),
                       ZFMP_IN(zftimet, mouseTime))
    /**
     * @brief simulate drag
     */
    ZFMETHOD_DECLARE_2(void, scrollSimulateDrag,
                       ZFMP_IN(const ZFUIPoint &, mousePos),
                       ZFMP_IN(zftimet, mouseTime))
    /**
     * @brief simulate drag
     */
    ZFMETHOD_DECLARE_2(void, scrollSimulateDragEnd,
                       ZFMP_IN(zftimet, mouseTime),
                       ZFMP_IN_OPT(zfbool, needScrollAni, zftrue))

protected:
    /**
     * @brief for subclass to supply advanced scroll controll
     *
     * this method is used to ignore some internal scroll actions
     * from being recognized as user's manual scroll action,
     * see #EventScrollOnScrolledByUser\n
     * this method can be called more than one time,
     * but must be paired
     */
    zffinal void scrollOverride(ZF_IN zfbool scrollOverride);
    /** @brief see #scrollOverride */
    zffinal zfbool scrollOverride(void);

    // ============================================================
    // scroll status
public:
    /**
     * @brief current state for the scroll view
     */
    ZFMETHOD_DECLARE_0(ZFUIScrollViewStateEnum, scrollViewState)

    // ============================================================
    // scroll callbacks
public:
    zffinal void _ZFP_ZFUIScrollView_notifyDragBegin(ZF_IN const ZFUIPoint &mousePos,
                                                     ZF_IN zftimet mouseTime);
    zffinal void _ZFP_ZFUIScrollView_notifyDrag(ZF_IN const ZFUIPoint &mousePos,
                                                ZF_IN zftimet mouseTime);
    zffinal void _ZFP_ZFUIScrollView_notifyDragEnd(ZF_IN zftimet mouseTime,
                                                   ZF_IN zfbool needScrollAni);
    zffinal void _ZFP_ZFUIScrollView_notifyScrollAnimation(ZF_IN zftimet relativeTimeInMiliseconds);
protected:
    /** @brief see #EventScrollOnDragBegin */
    virtual inline void scrollOnDragBegin(void)
    {
        this->observerNotify(ZFUIScrollView::EventScrollOnDragBegin());
    }
    /** @brief see #EventScrollOnDrag */
    virtual inline void scrollOnDrag(void)
    {
        this->observerNotify(ZFUIScrollView::EventScrollOnDrag());
    }
    /** @brief see #EventScrollOnDragEnd */
    virtual inline void scrollOnDragEnd(void)
    {
        this->observerNotify(ZFUIScrollView::EventScrollOnDragEnd());
    }
    /** @brief see #EventScrollOnScrollBegin */
    virtual inline void scrollOnScrollBegin(void)
    {
        this->observerNotify(ZFUIScrollView::EventScrollOnScrollBegin());
    }
    /** @brief see #EventScrollOnScroll */
    virtual inline void scrollOnScroll(void)
    {
        this->observerNotify(ZFUIScrollView::EventScrollOnScroll());
    }
    /** @brief see #EventScrollOnScrollEnd */
    virtual inline void scrollOnScrollEnd(void)
    {
        this->observerNotify(ZFUIScrollView::EventScrollOnScrollEnd());
    }
    /** @brief see #EventScrollAreaOnChange */
    virtual inline void scrollAreaOnChange(void)
    {
        this->observerNotify(ZFUIScrollView::EventScrollAreaOnChange());
    }
    /** @brief see #EventScrollContentFrameOnChange */
    virtual inline void scrollContentFrameOnChange(void)
    {
        this->observerNotify(ZFUIScrollView::EventScrollContentFrameOnChange());
    }
    /** @brief see #EventScrollAutoScrollOnStart */
    virtual inline void scrollAutoScrollOnStart(void)
    {
        this->observerNotify(ZFUIScrollView::EventScrollAutoScrollOnStart());
    }
    /** @brief see #EventScrollAutoScrollOnStop */
    virtual inline void scrollAutoScrollOnStop(void)
    {
        this->observerNotify(ZFUIScrollView::EventScrollAutoScrollOnStop());
    }
    /** @brief see #EventScrollOnScrolledByUser */
    virtual inline void scrollOnScrolledByUser(void)
    {
        this->observerNotify(ZFUIScrollView::EventScrollOnScrolledByUser());
    }

private:
    _ZFP_ZFUIScrollViewPrivate *d;
    friend zfclassFwd _ZFP_ZFUIScrollViewPrivate;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFUIScrollView_h_

