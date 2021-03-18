#include "ZFUIOnScreenKeyboardAutoFit.h"

ZF_NAMESPACE_GLOBAL_BEGIN

#define _ZFP_ZFUIOnScreenKeyboardAutoFitLayout_DEBUG 0

// ============================================================
ZFSTYLE_DEFAULT_DEFINE(ZFUIOnScreenKeyboardAutoFitLayout)

// ============================================================
// _ZFP_ZFUIOnScreenKeyboardAutoFitLayoutPrivate
zfclassNotPOD _ZFP_ZFUIOnScreenKeyboardAutoFitLayoutPrivate
{
public:
    ZFUIOnScreenKeyboardAutoFitLayout *pimplOwner;
    zfbool scrollEnableFlag;
    ZFUIView *autoFitFocusedView;
    ZFUIMargin autoFitMargin;
    ZFListener onScreenKeyboardStateOnChangeListener;
    ZFListener viewFocusOnChangeListener;
    ZFListener scrollFocusedViewToVisibleDelayListener;
    zfbool autoFitFocusedViewNeedUpdate;

public:
    void autoFitEnable(ZF_IN zfbool value)
    {
        if(value)
        {
            ZFObjectGlobalEventObserver().observerAdd(
                ZFUIOnScreenKeyboardState::EventKeyboardStateOnChange(),
                this->onScreenKeyboardStateOnChangeListener,
                this->pimplOwner->objectHolder());

            this->pimplOwner->layoutRequest();
        }
        else
        {
            this->scrollEnableFlag = zffalse;
            this->pimplOwner->scrollEnable(this->scrollEnableFlag && this->pimplOwner->autoFitScrollEnable());
            ZFObjectGlobalEventObserver().observerRemove(
                ZFUIOnScreenKeyboardState::EventKeyboardStateOnChange(),
                this->onScreenKeyboardStateOnChangeListener);

            this->pimplOwner->scrollContentFrame(ZFUIRectGetBounds(this->pimplOwner->viewFrame()));
            if(this->autoFitMargin == ZFUIMarginZero() || this->pimplOwner->layoutParam() == zfnull)
            {
                return ;
            }
            this->autoFitMargin = ZFUIMarginZero();
            #if _ZFP_ZFUIOnScreenKeyboardAutoFitLayout_DEBUG
                zfLogTrimT() << "[ZFUIOnScreenKeyboardAutoFitLayout] margin reset to" << this->autoFitMargin;
            #endif
            this->pimplOwner->layoutParam()->layoutMargin(this->autoFitMargin);
        }
    }
    void autoFitFocusedViewToVisible(ZF_IN zfbool value)
    {
        if(value)
        {
            ZFObjectGlobalEventObserver().observerAdd(
                ZFUIView::EventViewFocusOnChange(),
                this->viewFocusOnChangeListener,
                this->pimplOwner->objectHolder());
        }
        else
        {
            ZFObjectGlobalEventObserver().observerRemove(
                ZFUIView::EventViewFocusOnChange(),
                this->viewFocusOnChangeListener);
            #if _ZFP_ZFUIOnScreenKeyboardAutoFitLayout_DEBUG
                zfLogTrimT() << "[ZFUIOnScreenKeyboardAutoFitLayout] focused view changed to" << (void *)zfnull;
            #endif
            this->autoFitFocusedView = zfnull;
        }
    }

public:
    void autoFitUpdateFrame(void)
    {
        if(this->pimplOwner->layoutParam() == zfnull)
        {
            return ;
        }

        ZFUIRect orgRect = ZFUIViewPositionOnScreen(this->pimplOwner);
        if(orgRect.width != this->pimplOwner->viewFrame().width
            || orgRect.height != this->pimplOwner->viewFrame().height)
        {
            return ;
        }
        orgRect.x -= this->autoFitMargin.left;
        orgRect.width += this->autoFitMargin.left + this->autoFitMargin.right;
        orgRect.y -= this->autoFitMargin.top;
        orgRect.height += this->autoFitMargin.top + this->autoFitMargin.bottom;

        this->autoFitMarginCalc(this->autoFitMargin, orgRect, ZFUIOnScreenKeyboardState::instanceForView(this->pimplOwner));
        #if _ZFP_ZFUIOnScreenKeyboardAutoFitLayout_DEBUG
            zfLogTrimT() << "[ZFUIOnScreenKeyboardAutoFitLayout] margin changed to" << this->autoFitMargin;
        #endif
        this->pimplOwner->layoutParam()->layoutMargin(this->autoFitMargin);
    }
    void autoFitMarginCalc(ZF_OUT ZFUIMargin &ret,
                           ZF_IN const ZFUIRect &orgRect,
                           ZF_IN ZFUIOnScreenKeyboardState *keyboardState)
    {
        ret = ZFUIMarginZero();
        if(keyboardState->keyboardShowing())
        {
            ZFUIRect clientFrame = keyboardState->keyboardFixClientFrame();
            if(ZFUIRectGetLeft(clientFrame) > ZFUIRectGetLeft(orgRect))
            {
                ret.left += ZFUIRectGetLeft(clientFrame) - ZFUIRectGetLeft(orgRect);
            }
            if(ZFUIRectGetTop(clientFrame) > ZFUIRectGetTop(orgRect))
            {
                ret.top += ZFUIRectGetTop(clientFrame) - ZFUIRectGetTop(orgRect);
            }
            if(ZFUIRectGetRight(clientFrame) < ZFUIRectGetRight(orgRect))
            {
                ret.right += ZFUIRectGetRight(orgRect) - ZFUIRectGetRight(clientFrame);
            }
            if(ZFUIRectGetBottom(clientFrame) < ZFUIRectGetBottom(orgRect))
            {
                ret.bottom += ZFUIRectGetBottom(orgRect) - ZFUIRectGetBottom(clientFrame);
            }
        }
    }

public:
    static ZFLISTENER_PROTOTYPE_EXPAND(onScreenKeyboardStateOnChange)
    {
        ZFUIOnScreenKeyboardState *keyboardState = listenerData.sender<ZFUIOnScreenKeyboardState *>();
        ZFUIOnScreenKeyboardAutoFitLayout *layout = userData->objectHolded();
        if(keyboardState->keyboardShowing())
        {
            if(layout->autoFitFocusedView() != zfnull)
            {
                layout->d->autoFitFocusedViewNeedUpdate = zftrue;
            }
            layout->d->scrollEnableFlag = zftrue;
            layout->scrollEnable(layout->d->scrollEnableFlag && layout->autoFitScrollEnable());
            layout->layoutRequest();
        }
        else
        {
            layout->d->scrollEnableFlag = zffalse;
            layout->scrollEnable(layout->d->scrollEnableFlag && layout->autoFitScrollEnable());
            layout->d->autoFitMargin = ZFUIMarginZero();
            #if _ZFP_ZFUIOnScreenKeyboardAutoFitLayout_DEBUG
                zfLogTrimT() << "[ZFUIOnScreenKeyboardAutoFitLayout] margin reset to" << layout->d->autoFitMargin;
            #endif
            layout->layoutParam()->layoutMargin(layout->d->autoFitMargin);
            layout->scrollContentFrame(ZFUIRectGetBounds(layout->viewFrame()));
        }
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(viewFocusOnChange)
    {
        ZFUIOnScreenKeyboardAutoFitLayout *layout = userData->objectHolded();
        ZFUIView *view = listenerData.sender<ZFUIView *>();
        if(!view->viewFocused())
        {
            if(view == layout->d->autoFitFocusedView)
            {
                layout->d->autoFitFocusedView = zfnull;
            }
            return ;
        }

        {
            ZFUIView *parent = view;
            while(parent != zfnull && parent != layout)
            {
                parent = parent->viewParent();
            }
            if(parent == zfnull)
            {
                return ;
            }
        }

        #if _ZFP_ZFUIOnScreenKeyboardAutoFitLayout_DEBUG
            zfLogTrimT() << "[ZFUIOnScreenKeyboardAutoFitLayout] focused view changed to" << view;
        #endif
        layout->d->autoFitFocusedView = view;
        layout->d->autoFitFocusedViewNeedUpdate = zftrue;
        layout->layoutRequest();
    }
    static ZFLISTENER_PROTOTYPE_EXPAND(scrollFocusedViewToVisibleDelay)
    {
        ZFUIOnScreenKeyboardAutoFitLayout *layout = userData->objectHolded();
        if(layout->autoFitFocusedView() != zfnull)
        {
            #if _ZFP_ZFUIOnScreenKeyboardAutoFitLayout_DEBUG
                zfLogTrimT() << "[ZFUIOnScreenKeyboardAutoFitLayout] scrollFocusedViewToVisible" << layout->autoFitFocusedView();
            #endif
            layout->scrollChildToVisible(layout->autoFitFocusedView());
        }
    }

public:
    _ZFP_ZFUIOnScreenKeyboardAutoFitLayoutPrivate(void)
    : pimplOwner(zfnull)
    , scrollEnableFlag(zffalse)
    , autoFitFocusedView(zfnull)
    , autoFitMargin(ZFUIMarginZero())
    , onScreenKeyboardStateOnChangeListener(ZFCallbackForFunc(_ZFP_ZFUIOnScreenKeyboardAutoFitLayoutPrivate::onScreenKeyboardStateOnChange))
    , viewFocusOnChangeListener(ZFCallbackForFunc(_ZFP_ZFUIOnScreenKeyboardAutoFitLayoutPrivate::viewFocusOnChange))
    , scrollFocusedViewToVisibleDelayListener(ZFCallbackForFunc(_ZFP_ZFUIOnScreenKeyboardAutoFitLayoutPrivate::scrollFocusedViewToVisibleDelay))
    , autoFitFocusedViewNeedUpdate(zffalse)
    {
    }
};

// ============================================================
// ZFUIOnScreenKeyboardAutoFitLayout
ZFOBJECT_REGISTER(ZFUIOnScreenKeyboardAutoFitLayout)

ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIOnScreenKeyboardAutoFitLayout, zfbool, autoFitEnable)
{
    if(this->autoFitEnable() != propertyValueOld)
    {
        d->autoFitEnable(this->autoFitEnable());
    }
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIOnScreenKeyboardAutoFitLayout, zfbool, autoFitFocusedViewToVisible)
{
    if(this->autoFitFocusedViewToVisible() != propertyValueOld)
    {
        d->autoFitFocusedViewToVisible(this->autoFitFocusedViewToVisible());
    }
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIOnScreenKeyboardAutoFitLayout, zfbool, autoFitScrollEnable)
{
    if(this->autoFitFocusedViewToVisible() != propertyValueOld)
    {
        this->scrollEnable(d->scrollEnableFlag && this->autoFitScrollEnable());
    }
}

ZFUIView *ZFUIOnScreenKeyboardAutoFitLayout::autoFitFocusedView(void)
{
    return d->autoFitFocusedView;
}

void ZFUIOnScreenKeyboardAutoFitLayout::objectOnInit(void)
{
    zfsuper::objectOnInit();
    d = zfpoolNew(_ZFP_ZFUIOnScreenKeyboardAutoFitLayoutPrivate);
    d->pimplOwner = this;
    d->scrollEnableFlag = zffalse;
    this->scrollEnable(d->scrollEnableFlag && this->autoFitScrollEnable());
}
void ZFUIOnScreenKeyboardAutoFitLayout::objectOnDealloc(void)
{
    ZFThreadTaskCancel(d->scrollFocusedViewToVisibleDelayListener);

    d->autoFitEnable(zffalse);
    d->autoFitFocusedViewToVisible(zffalse);

    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}
void ZFUIOnScreenKeyboardAutoFitLayout::objectOnInitFinish(void)
{
    zfsuper::objectOnInitFinish();
    d->autoFitEnable(this->autoFitEnable());
    d->autoFitFocusedViewToVisible(this->autoFitFocusedViewToVisible());
}

ZFSerializablePropertyType ZFUIOnScreenKeyboardAutoFitLayout::serializableOnCheckPropertyType(ZF_IN const ZFProperty *property)
{
    if(property == ZFPropertyAccess(ZFUIScrollView, scrollContentFrame)
        || property == ZFPropertyAccess(ZFUIScrollView, scrollEnable)
        )
    {
        return ZFSerializablePropertyTypeNotSerializable;
    }
    else
    {
        return zfsuperI(ZFSerializable)::serializableOnCheckPropertyType(property);
    }
}

void ZFUIOnScreenKeyboardAutoFitLayout::layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                                        ZF_IN const ZFUISize &sizeHint,
                                                        ZF_IN const ZFUISizeParam &sizeParam)
{
    ret = ZFUISizeZero();
    ZFUISize sizeHintTmp = ZFUIViewLayoutParam::sizeHintOffset(sizeHint,
        ZFUISizeMake(ZFUIMarginGetWidth(d->autoFitMargin), ZFUIMarginGetHeight(d->autoFitMargin)));
    for(zfindex i = 0; i < this->childCount(); ++i)
    {
        ZFUIView *child = this->childAtIndex(i);
        child->layoutMeasure(sizeHintTmp, child->layoutParam()->sizeParam());
        ret.width = zfmMax(ret.width, child->layoutMeasuredSize().width + ZFUIMarginGetWidth(child->layoutParam()->layoutMargin()));
        ret.height = zfmMax(ret.height, child->layoutMeasuredSize().height + ZFUIMarginGetWidth(child->layoutParam()->layoutMargin()));
    }
}
void ZFUIOnScreenKeyboardAutoFitLayout::layoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds)
{
    zfsuper::layoutOnLayoutPrepare(bounds);
    if(!this->autoFitEnable()
        || (this->viewFrame().width == 0 && this->viewFrame().height == 0)
        || this->viewFrame().width != bounds.width || this->viewFrame().height != bounds.height
        || !d->scrollEnableFlag)
    {
        this->scrollContentFrame(bounds);
    }
    else
    {
        ZFUIRect scrollContentFrame = this->scrollContentFrame();
        scrollContentFrame.width = bounds.width + ZFUIMarginGetWidth(d->autoFitMargin);
        scrollContentFrame.height = bounds.height + ZFUIMarginGetHeight(d->autoFitMargin);
        this->scrollContentFrameUpdate(scrollContentFrame);
        d->autoFitUpdateFrame();
        if(d->autoFitFocusedViewNeedUpdate)
        {
            d->autoFitFocusedViewNeedUpdate = zffalse;
            ZFThreadTaskRequest(d->scrollFocusedViewToVisibleDelayListener, this->objectHolder());
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

