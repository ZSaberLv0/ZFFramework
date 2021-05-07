#include "ZFUIRootView.h"
#include "ZFUISysWindow.h"
#include "ZFUIWindow.h"

#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIRootView)

ZFMETHOD_DEFINE_1(ZFUIRootView, void, scaleForApp,
                  ZFMP_IN(zffloat, scale))
{
    if(scale > 0)
    {
        this->_ZFP_ZFUIView_scaleSetRecursively(scale * this->scaleForImpl(), this->scaleForImpl());
    }
}

ZFMETHOD_USER_REGISTER_FOR_ZFOBJECT_FUNC_0(ZFUIRootView, ZFUISysWindow *, rootViewOwnerSysWindow)

ZFMETHOD_DEFINE_0(ZFUIRootView, const ZFCoreArrayPOD<ZFUIWindow *> &, windowList)
{
    return _ZFP_ZFUIRootView_windowList;
}

void ZFUIRootView::viewOnAddToParent(ZF_IN ZFUIView *parent)
{
    zfCoreCriticalMessage(
        "you must not add ZFUIRootView to another parent, parent: %s",
        ZFObjectInfo(parent).cString());
}
void ZFUIRootView::viewOnRemoveFromParent(ZF_IN ZFUIView *parent)
{
    zfCoreCriticalMessage(
        "you must not add ZFUIRootView to another parent, parent: %s",
        ZFObjectInfo(parent).cString());
}

void ZFUIRootView::layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                   ZF_IN const ZFUISize &sizeHint,
                                   ZF_IN const ZFUISizeParam &sizeParam)
{
    ret = sizeHint;
}

static void _ZFP_ZFUIRootView_layoutParamApply(ZF_OUT ZFUIRect &ret,
                                               ZF_IN const ZFUIRect &rect,
                                               ZF_IN ZFUIView *child,
                                               ZF_IN ZFUILayoutParam *lp,
                                               ZF_IN const ZFUIMargin &sysWindowMargin)
{
    ZFUIMargin totalMargin = ZFUIMarginInc(lp->layoutMargin(), sysWindowMargin);
    ZFUISize refSizeTmp = ZFUIRectGetSize(ZFUIRectApplyMargin(rect, totalMargin));
    if(refSizeTmp.width < 0)
    {
        refSizeTmp.width = 0;
    }
    if(refSizeTmp.height < 0)
    {
        refSizeTmp.height = 0;
    }

    if(lp->sizeHint().width >= 0)
    {
        refSizeTmp.width = zfmMin(lp->sizeHint().width, refSizeTmp.width);
    }
    if(lp->sizeHint().height >= 0)
    {
        refSizeTmp.height = zfmMin(lp->sizeHint().height, refSizeTmp.height);
    }
    child->layoutMeasure(refSizeTmp, lp->sizeParam());
    ZFUIAlignApply(
            ret,
            lp->layoutAlign(),
            rect,
            child->layoutMeasuredSize(),
            totalMargin
        );
}
void ZFUIRootView::layoutOnLayout(ZF_IN const ZFUIRect &bounds)
{
    for(zfindex i = 0; i < this->childCount(); ++i)
    {
        ZFUIView *child = this->childAtIndex(i);
        ZFUIWindow *window = ZFCastZFObject(ZFUIWindow *, child);
        if(window == zfnull || !window->sysWindowMarginShouldApply())
        {
            child->viewFrame(
                ZFUILayoutParam::layoutParamApply(
                    bounds,
                    child,
                    child->layoutParam()));
            continue;
        }

        ZFUIRect result = ZFUIRectZero();
        _ZFP_ZFUIRootView_layoutParamApply(
            result,
            bounds,
            child,
            child->layoutParam(),
            window->windowOwnerSysWindow()->sysWindowMargin());
        child->viewFrame(result);
    }
}

ZF_NAMESPACE_GLOBAL_END

