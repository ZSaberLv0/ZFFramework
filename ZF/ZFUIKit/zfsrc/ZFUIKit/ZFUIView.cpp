#include "ZFUIView.h"
#include "protocol/ZFProtocolZFUIView.h"
#include "protocol/ZFProtocolZFUIViewFocus.h"
#include "protocol/ZFProtocolZFUIViewTransform.h"
#include "ZFUIViewFocus.h"

#include "ZFCore/ZFSTLWrapper/zfstl_string.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFSTYLE_DEFAULT_DEFINE(ZFUIView)

// ============================================================
// attached listeners
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewListenerHolder, ZFLevelZFFrameworkEssential)
{
    this->layoutParamChangedListener = ZFCallbackForFunc(zfself::layoutParamChanged);
}
public:
    ZFListener layoutParamChangedListener;
private:
    static void layoutParamChanged(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject *userData)
    {
        userData->objectHolded<ZFUIView *>()->layoutRequest();
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIViewListenerHolder)

// ============================================================
// _ZFP_ZFUIViewPrivate
zfclassLikePOD _ZFP_ZFUIViewLayerData
{
public:
    ZFCoreArrayPOD<ZFUIView *> views;
};
typedef zfstlmap<zfstlstringZ, zfbool> _ZFP_ZFUIViewInternalViewAutoSerializeTagMapType;
zfclassNotPOD _ZFP_ZFUIViewPrivate
{
public:
    void *nativeView;
    void *nativeImplView;
    ZFUIViewNativeImplViewDeleteCallback nativeImplViewDeleteCallback;
    ZFUIMargin nativeImplViewMargin;

    ZFUIView *viewParent;
    ZFUIViewChildLayerEnum viewLayer;
    ZFUILayoutParam *layoutParam; // retain
    ZFUILayoutParam *serializableRefLayoutParam; // retain
    zfautoObjectT<ZFUILayoutParam *> serializableRefLayoutParamCache;

    _ZFP_ZFUIViewInternalViewAutoSerializeTagMapType internalViewAutoSerializeTags;

    zffloat UIScaleInherited;
    zffloat UIScaleForImpl;
    zffloat UIScaleFixed;

    ZFUIViewMeasureResult *measureResult;
    ZFUIRect viewFrame;
    ZFUIRect viewFramePrev;
    ZFUIPoint viewCenter;
    ZFUIRect nativeImplViewFrame;
    _ZFP_ZFUIViewLayerData layerInternalImpl;
    _ZFP_ZFUIViewLayerData layerInternalBg;
    _ZFP_ZFUIViewLayerData layerNormal;
    _ZFP_ZFUIViewLayerData layerInternalFg;
    enum {
        stateFlag_layoutRequested = 1 << 0,
        stateFlag_layoutRequestedRecursively = 1 << 1,
        stateFlag_layouting = 1 << 2,
        stateFlag_viewFrameOverrideFlag = 1 << 3,
        stateFlag_UIScaleChanged = 1 << 4,
        stateFlag_viewTransformUpdate = 1 << 5,
        stateFlag_observerHasAddFlag_viewChildOnChange = 1 << 6,
        stateFlag_observerHasAddFlag_viewChildOnAdd = 1 << 7,
        stateFlag_observerHasAddFlag_viewChildOnRemove = 1 << 8,
        stateFlag_observerHasAddFlag_viewOnAddToParent = 1 << 9,
        stateFlag_observerHasAddFlag_viewOnRemoveFromParent = 1 << 10,
        stateFlag_observerHasAddFlag_layoutOnLayoutRequest = 1 << 11,
    };
    zfuint stateFlag;

public:
    _ZFP_ZFUIViewPrivate(void)
    : nativeView(zfnull)
    , nativeImplView(zfnull)
    , nativeImplViewDeleteCallback(zfnull)
    , nativeImplViewMargin(ZFUIMarginZero())
    , viewParent(zfnull)
    , viewLayer(ZFUIViewChildLayer::e_Normal)
    , layoutParam(zfnull)
    , serializableRefLayoutParam(zfnull)
    , serializableRefLayoutParamCache()
    , internalViewAutoSerializeTags()
    , UIScaleInherited(1)
    , UIScaleForImpl(1)
    , UIScaleFixed(1)
    , measureResult(zfnull)
    , viewFrame(ZFUIRectZero())
    , viewFramePrev(ZFUIRectZero())
    , viewCenter(ZFUIPointZero())
    , nativeImplViewFrame(ZFUIRectZero())
    , layerInternalImpl()
    , layerInternalBg()
    , layerNormal()
    , layerInternalFg()
    , stateFlag(0)
    {
        ZFBitSet(this->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutRequested);
        ZFBitSet(this->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutRequestedRecursively);
    }

public:
    void layoutRequest(ZF_IN ZFUIView *view, ZF_IN zfbool recursively)
    {
        if(recursively)
        {
            if(!ZFBitTest(view->d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutRequestedRecursively))
            {
                ZFPROTOCOL_INTERFACE_CLASS(ZFUIView) *impl = ZFPROTOCOL_ACCESS(ZFUIView);
                do
                {
                    ZFBitSet(view->d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutRequestedRecursively);
                    if(!ZFBitTest(view->d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutRequested))
                    {
                        ZFBitSet(view->d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutRequested);
                        view->layoutOnLayoutRequest();
                        impl->layoutRequest(view);
                    }
                    view = view->viewParent();
                } while(view != zfnull && !ZFBitTest(view->d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutRequestedRecursively));
            }
        }
        else
        {
            if(!ZFBitTest(view->d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutRequested))
            {
                ZFBitSet(view->d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutRequested);
                view->layoutOnLayoutRequest();
                ZFPROTOCOL_ACCESS(ZFUIView)->layoutRequest(view);
            }
        }
    }
    void layoutAction(ZF_IN ZFUIView *view, ZF_IN const ZFUIRect &bounds)
    {
        ZFBitSet(this->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layouting);

        // layout prepare
        view->layoutOnLayoutPrepare(bounds);
        view->observerNotify(ZFUIView::EventViewLayoutOnLayoutPrepare());

        if(view->d->nativeImplView != zfnull)
        {
            ZFUIRect nativeImplViewFrame = ZFUIRectZero();
            view->nativeImplViewOnLayout(nativeImplViewFrame, bounds, view->nativeImplViewMargin());
            if(view->d->nativeImplViewFrame != nativeImplViewFrame
                || ZFBitTest(view->d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_UIScaleChanged))
            {
                view->d->nativeImplViewFrame = nativeImplViewFrame;
                ZFPROTOCOL_ACCESS(ZFUIView)->nativeImplViewFrame(
                    view,
                    ZFUIRectApplyScale(nativeImplViewFrame, view->UIScaleFixed()));
            }
        }

        // layout
        view->layoutOnLayout(bounds);
        view->observerNotify(ZFUIView::EventViewLayoutOnLayout());

        // internal views
        view->internalViewOnLayout(bounds);

        // layout finish
        view->layoutOnLayoutFinish(bounds);
        view->observerNotify(ZFUIView::EventViewLayoutOnLayoutFinish());

        // update transform
        if(ZFBitTest(this->stateFlag, stateFlag_viewTransformUpdate))
        {
            this->viewTransformUpdateAction(view);
        }

        ZFBitUnset(this->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layouting);
    }
    void layoutParamChange(ZF_IN ZFUIView *owner, ZF_IN ZFUILayoutParam *newLayoutParam)
    {
        ZF_GLOBAL_INITIALIZER_CLASS(ZFUIViewListenerHolder) *listenerHolder = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewListenerHolder);
        zfRetain(newLayoutParam);
        if(newLayoutParam)
        {
            newLayoutParam->observerAdd(
                ZFUILayoutParam::EventLayoutParamOnChange(),
                listenerHolder->layoutParamChangedListener,
                owner->objectHolder());
        }
        if(this->layoutParam)
        {
            this->layoutParam->observerRemove(ZFUILayoutParam::EventLayoutParamOnChange(), listenerHolder->layoutParamChangedListener);
        }
        zfRelease(this->layoutParam);
        this->layoutParam = newLayoutParam;
    }
    void viewFrameUpdate(ZF_IN const ZFUIRect &viewFrame)
    {
        this->viewFramePrev = this->viewFrame;
        this->viewFrame = viewFrame;
        if(this->viewFrame.width < 0)
        {
            this->viewFrame.width = 0;
        }
        if(this->viewFrame.height < 0)
        {
            this->viewFrame.height = 0;
        }
        this->viewCenter.x = this->viewFrame.x + this->viewFrame.width / 2;
        this->viewCenter.y = this->viewFrame.y + this->viewFrame.height / 2;
    }
    void viewFrameUpdateForImpl(ZF_IN ZFUIView *view)
    {
        if(view->UIScale() == 1 || view->UIScale() == 0 || view->viewParent() == zfnull)
        {
            ZFPROTOCOL_ACCESS(ZFUIView)->viewFrame(
                    view,
                    ZFUIRectApplyScale(this->viewFrame, view->UIScaleFixed())
                );
        }
        else
        {
            ZFPROTOCOL_ACCESS(ZFUIView)->viewFrame(
                    view,
                    ZFUIRectMake(
                            this->viewCenter.x * this->UIScaleInherited * this->UIScaleForImpl - this->viewFrame.width * this->UIScaleFixed / 2,
                            this->viewCenter.y * this->UIScaleInherited * this->UIScaleForImpl - this->viewFrame.height * this->UIScaleFixed / 2,
                            this->viewFrame.width * this->UIScaleFixed,
                            this->viewFrame.height * this->UIScaleFixed
                        )
                );
        }
    }
    zfindex viewLayerPrevCount(ZF_IN _ZFP_ZFUIViewLayerData &layer)
    {
        zfindex nativeImplFix = (this->nativeImplView ? 1 : 0);
        if(&layer == &(this->layerNormal))
        {
            return this->layerInternalImpl.views.count() + nativeImplFix
                + this->layerInternalBg.views.count();
        }
        else if(&layer == &(this->layerInternalImpl))
        {
            return 0;
        }
        else if(&layer == &(this->layerInternalBg))
        {
            return this->layerInternalImpl.views.count() + nativeImplFix;
        }
        else if(&layer == &(this->layerInternalFg))
        {
            return this->layerInternalImpl.views.count() + nativeImplFix
                + this->layerInternalBg.views.count()
                + this->layerNormal.views.count();
        }
        else
        {
            zfCoreCriticalShouldNotGoHere();
            return 0;
        }
    }
    static void UIScaleUpdateRecursively(ZF_IN ZFUIView *child)
    {
        ZFBitSet(child->d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_UIScaleChanged);
        child->d->UIScaleFixed = child->d->UIScaleInherited * child->UIScale() * child->d->UIScaleForImpl;
        for(zfindex i = child->d->layerInternalImpl.views.count() - 1; i != zfindexMax(); --i)
        {
            UIScaleUpdateCheck(child, child->d->layerInternalImpl.views.get(i));
        }
        for(zfindex i = child->d->layerInternalBg.views.count() - 1; i != zfindexMax(); --i)
        {
            UIScaleUpdateCheck(child, child->d->layerInternalBg.views.get(i));
        }
        for(zfindex i = child->d->layerInternalFg.views.count() - 1; i != zfindexMax(); --i)
        {
            UIScaleUpdateCheck(child, child->d->layerInternalFg.views.get(i));
        }
        for(zfindex i = child->childCount() - 1; i != zfindexMax(); --i)
        {
            UIScaleUpdateCheck(child, child->childAtIndex(i));
        }
        child->layoutRequest();
        child->UIScaleOnChange();
    }
    static void UIScaleUpdateCheck(ZF_IN ZFUIView *parent, ZF_IN ZFUIView *child)
    {
        zffloat UIScaleInherited = parent->d->UIScaleInherited * parent->UIScale();
        if(child->d->UIScaleInherited == UIScaleInherited && child->d->UIScaleForImpl == parent->d->UIScaleForImpl)
        {
            return;
        }
        child->d->UIScaleInherited = UIScaleInherited;
        child->d->UIScaleForImpl = parent->d->UIScaleForImpl;
        UIScaleUpdateRecursively(child);
    }
    void childAdd(ZF_IN ZFUIView *owner,
                  ZF_IN ZFUIViewChildLayerEnum childLayer,
                  ZF_IN _ZFP_ZFUIViewLayerData &layer,
                  ZF_IN ZFUIView *view,
                  ZF_IN ZFUILayoutParam *layoutParam,
                  ZF_IN zfindex atIndex)
    {
        zfCoreAssertWithMessageTrim(!ZFBitTest(owner->objectInstanceState(), ZFObjectInstanceStateOnDealloc),
            "[ZFUIView] add child during parent's dealloc is not allowed");
        zfCoreAssertWithMessageTrim(view != zfnull, "[ZFUIView] add null child");
        zfCoreAssertWithMessageTrim(view->viewParent() == zfnull, "[ZFUIView] add child which already has parent, you should remove it first");
        zfRetain(view);

        if(this->serializableRefLayoutParamCache == zfnull)
        {
            this->serializableRefLayoutParamCache = owner->layoutParamCreate();
        }
        view->serializableRefLayoutParam(this->serializableRefLayoutParamCache);

        zfbool layoutParamNeedRelease = zffalse;
        if(layoutParam == zfnull)
        {
            layoutParam = view->layoutParam();
        }
        if(layoutParam == zfnull)
        {
            layoutParam = zfRetain(owner->layoutParamCreate().to<ZFUILayoutParam *>());
            layoutParamNeedRelease = zftrue;
        }
        else
        {
            if(!layoutParam->classData()->classIsTypeOf(owner->layoutParamClass()))
            {
                layoutParamNeedRelease = zftrue;
                ZFUILayoutParam *tmp = zfRetain(owner->layoutParamCreate().to<ZFUILayoutParam *>());
                tmp->styleableCopyFrom(layoutParam);
                layoutParam = tmp;
            }
        }
        view->_ZFP_ZFUIView_parentChanged(owner, layoutParam, childLayer);
        if(atIndex == zfindexMax())
        {
            atIndex = layer.views.count();
        }
        layer.views.add(atIndex, view);
        owner->implChildOnAdd(view, this->viewLayerPrevCount(layer) + atIndex, childLayer, atIndex);

        owner->layoutRequest();
        view->layoutRequest();

        if(layoutParamNeedRelease)
        {
            zfRelease(layoutParam);
        }

        UIScaleUpdateCheck(owner, view);
        owner->viewChildOnAdd(view, childLayer);
        view->viewOnAddToParent(owner);
        owner->viewChildOnChange();
    }
    void childRemove(ZF_IN ZFUIView *owner,
                     ZF_IN ZFUIViewChildLayerEnum childLayer,
                     ZF_IN _ZFP_ZFUIViewLayerData &layer,
                     ZF_IN ZFUIView *view)
    {
        zfindex index = this->childFind(layer, view);
        if(index != zfindexMax())
        {
            this->childRemoveAtIndex(owner, childLayer, layer, index);
        }
    }
    void childRemoveAtIndex(ZF_IN ZFUIView *owner,
                            ZF_IN ZFUIViewChildLayerEnum childLayer,
                            ZF_IN _ZFP_ZFUIViewLayerData &layer,
                            ZF_IN zfindex index)
    {
        ZFUIView *child = layer.views.get(index);

        child->_ZFP_ZFUIView_parentChanged(zfnull, zfnull, ZFUIViewChildLayer::e_Normal);
        layer.views.remove(index);
        owner->implChildOnRemove(child, this->viewLayerPrevCount(layer) + index, childLayer, index);

        owner->layoutRequest();

        owner->viewChildOnRemove(child, childLayer);
        child->viewOnRemoveFromParent(owner);
        owner->viewChildOnChange();
        zfRelease(child);
    }
    void childRemoveAll(ZF_IN ZFUIView *owner,
                        ZF_IN ZFUIViewChildLayerEnum childLayer,
                        ZF_IN _ZFP_ZFUIViewLayerData &layer)
    {
        if(layer.views.isEmpty())
        {
            return ;
        }

        owner->layoutRequest();

        ZFCoreArrayPOD<ZFUIView *> tmp = layer.views;
        layer.views = ZFCoreArrayPOD<ZFUIView *>();

        zfindex prevLayerCount = this->viewLayerPrevCount(layer);
        for(zfindex i = tmp.count() - 1; i != zfindexMax(); --i)
        {
            tmp[i]->_ZFP_ZFUIView_parentChanged(zfnull, zfnull, ZFUIViewChildLayer::e_Normal);
            owner->implChildOnRemove(tmp[i], prevLayerCount + i, childLayer, i);
        }

        for(zfindex i = tmp.count() - 1; i != zfindexMax(); --i)
        {
            ZFUIView *child = tmp[i];
            owner->viewChildOnRemove(child, childLayer);
            child->viewOnRemoveFromParent(owner);
            zfRelease(child);
        }

        owner->viewChildOnChange();
    }
    void childMove(ZF_IN ZFUIView *owner,
                   ZF_IN ZFUIViewChildLayerEnum childLayer,
                   ZF_IN _ZFP_ZFUIViewLayerData &layer,
                   ZF_IN zfindex fromIndex,
                   ZF_IN zfindex toIndexOrIndexMax)
    {
        if(toIndexOrIndexMax == zfindexMax())
        {
            toIndexOrIndexMax = this->childCount(layer) - 1;
        }
        if(fromIndex >= this->childCount(layer))
        {
            zfCoreCriticalIndexOutOfRange(fromIndex, this->childCount(layer));
            return ;
        }
        if(toIndexOrIndexMax >= this->childCount(layer))
        {
            zfCoreCriticalIndexOutOfRange(fromIndex, this->childCount(layer));
            return ;
        }
        if(fromIndex == toIndexOrIndexMax)
        {
            return ;
        }

        ZFUIView *child = layer.views.get(fromIndex);
        layer.views.move(fromIndex, toIndexOrIndexMax);

        zfindex prevLayerCount = this->viewLayerPrevCount(layer);
        owner->implChildOnRemove(child, prevLayerCount + fromIndex, childLayer, fromIndex);
        owner->implChildOnAdd(child, prevLayerCount + toIndexOrIndexMax, childLayer, toIndexOrIndexMax);

        owner->viewChildOnChange();
    }
    void childMove(ZF_IN ZFUIView *owner,
                   ZF_IN ZFUIViewChildLayerEnum childLayer,
                   ZF_IN _ZFP_ZFUIViewLayerData &layer,
                   ZF_IN ZFUIView *child,
                   ZF_IN zfindex toIndexOrIndexMax)
    {
        zfindex fromIndex = this->childFind(layer, child);
        if(fromIndex != zfindexMax())
        {
            this->childMove(owner, childLayer, layer, fromIndex, toIndexOrIndexMax);
        }
    }
    void childReplace(ZF_IN ZFUIView *owner,
                      ZF_IN ZFUIViewChildLayerEnum childLayer,
                      ZF_IN _ZFP_ZFUIViewLayerData &layer,
                      ZF_IN zfindex atIndex,
                      ZF_IN ZFUIView *toReplace)
    {
        zfCoreAssert(atIndex < layer.views.count());
        zfCoreAssert(toReplace != zfnull);
        zfCoreAssert(toReplace->viewParent() == zfnull);

        ZFUIView *old = layer.views[atIndex];
        layer.views[atIndex] = toReplace;
        zfindex fixedIndex = this->viewLayerPrevCount(layer) + atIndex;
        owner->implChildOnRemove(old, fixedIndex, childLayer, atIndex);
        owner->implChildOnAdd(toReplace, fixedIndex, childLayer, atIndex);

        UIScaleUpdateCheck(owner, toReplace);
        toReplace->_ZFP_ZFUIView_parentChanged(owner, old->layoutParam(), childLayer);
        old->_ZFP_ZFUIView_parentChanged(zfnull, zfnull, ZFUIViewChildLayer::e_Normal);

        owner->viewChildOnRemove(old, childLayer);
        old->viewOnRemoveFromParent(owner);
        owner->viewChildOnAdd(toReplace, childLayer);
        toReplace->viewOnAddToParent(owner);
        owner->viewChildOnChange();

        zfRelease(old);
    }

    zfindex childCount(ZF_IN _ZFP_ZFUIViewLayerData &layer)
    {
        return layer.views.count();
    }
    ZFUIView *childAtIndex(ZF_IN _ZFP_ZFUIViewLayerData &layer,
                           ZF_IN zfindex index)
    {
        return layer.views.get(index);
    }
    zfindex childFind(ZF_IN _ZFP_ZFUIViewLayerData &layer,
                      ZF_IN ZFUIView *view)
    {
        return layer.views.find(view);
    }
    zfbool childArrayIsTheSame(ZF_IN ZFUIView *view0,
                               ZF_IN ZFUIView *view1,
                               ZF_IN ZFUIViewChildLayerEnum childLayer)
    {
        const ZFCoreArrayPOD<ZFUIView *> *children0 = zfnull;
        const ZFCoreArrayPOD<ZFUIView *> *children1 = zfnull;
        switch(childLayer)
        {
            case ZFUIViewChildLayer::e_Normal:
                children0 = &(view0->d->layerNormal.views);
                children1 = &(view1->d->layerNormal.views);
                break;
            case ZFUIViewChildLayer::e_InternalImpl:
                children0 = &(view0->d->layerInternalImpl.views);
                children1 = &(view1->d->layerInternalImpl.views);
                break;
            case ZFUIViewChildLayer::e_InternalBg:
                children0 = &(view0->d->layerInternalBg.views);
                children1 = &(view1->d->layerInternalBg.views);
                break;
            case ZFUIViewChildLayer::e_InternalFg:
                children0 = &(view0->d->layerInternalFg.views);
                children1 = &(view1->d->layerInternalFg.views);
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return zffalse;
        }

        if(children0->count() != children1->count())
        {
            return zffalse;
        }
        for(zfindex i = children0->count() - 1; i != zfindexMax(); --i)
        {
            if(ZFObjectCompare(children0->get(i), children1->get(i)) != ZFCompareTheSame)
            {
                return zffalse;
            }
        }
        return zftrue;
    }
    ZFUILayoutParam *childLayoutParamAtIndex(ZF_IN _ZFP_ZFUIViewLayerData &layer,
                                             ZF_IN zfindex index)
    {
        return layer.views.get(index)->layoutParam();
    }

    zfbool serializeInternalViewFromCategoryData(ZF_IN ZFUIView *owner,
                                                 ZF_IN ZFUIViewChildLayerEnum childLayer,
                                                 ZF_IN const ZFSerializableData &categoryData,
                                                 ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                                 ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
    {
        zfautoObjectT<ZFUIView *> internalView;
        if(!ZFObjectFromData(internalView, categoryData, outErrorHint, outErrorPos))
        {
            return zffalse;
        }
        if(internalView == zfnull)
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, categoryData,
                "null view");
            return zffalse;
        }
        if(!internalView->classData()->classIsTypeOf(ZFUIView::ClassData()))
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, categoryData,
                "%s not type of %s",
                internalView->objectInfoOfInstance().cString(), ZFUIView::ClassData()->classNameFull());
            return zffalse;
        }
        ZFUIView *internalViewTmp = internalView;
        if(internalViewTmp->viewId().isEmpty())
        {
            ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, categoryData,
                "auto serialized internal view %s has no viewId",
                internalViewTmp->objectInfoOfInstance().cString());
            return zffalse;
        }
        ZFUIView *exist = zfnull;
        const ZFCoreArrayPOD<ZFUIView *> *views = zfnull;
        switch(childLayer)
        {
            case ZFUIViewChildLayer::e_Normal:
                zfCoreCriticalShouldNotGoHere();
                break;
            case ZFUIViewChildLayer::e_InternalImpl:
                views = &(this->layerInternalImpl.views);
                break;
            case ZFUIViewChildLayer::e_InternalBg:
                views = &(this->layerInternalBg.views);
                break;
            case ZFUIViewChildLayer::e_InternalFg:
                views = &(this->layerInternalFg.views);
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return zffalse;
        }
        for(zfindex i = views->count() - 1; i != zfindexMax(); --i)
        {
            ZFUIView *tmp = views->get(i)->to<ZFUIView *>();
            if(tmp->viewId().compare(internalViewTmp->viewId()) == 0)
            {
                exist = tmp;
                break;
            }
        }
        if(exist == zfnull)
        {
            owner->internalViewAutoSerializeTagAdd(internalViewTmp->viewId().cString());
            switch(childLayer)
            {
                case ZFUIViewChildLayer::e_Normal:
                    zfCoreCriticalShouldNotGoHere();
                    break;
                case ZFUIViewChildLayer::e_InternalImpl:
                    owner->internalImplViewAdd(internalViewTmp);
                    break;
                case ZFUIViewChildLayer::e_InternalBg:
                    owner->internalBgViewAdd(internalViewTmp);
                    break;
                case ZFUIViewChildLayer::e_InternalFg:
                    owner->internalFgViewAdd(internalViewTmp);
                    break;
                default:
                    zfCoreCriticalShouldNotGoHere();
                    return zffalse;
            }
        }
        else
        {
            exist->styleableCopyFrom(internalViewTmp);
        }
        return zftrue;
    }
    zfbool serializeInternalViewToCategoryData(ZF_IN ZFUIViewChildLayerEnum childLayer,
                                               ZF_IN_OUT ZFSerializableData &serializableData,
                                               ZF_IN ZFUIView *ref,
                                               ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
    {
        const ZFCoreArrayPOD<ZFUIView *> *views = zfnull;
        const ZFCoreArrayPOD<ZFUIView *> *viewsRef = zfnull;
        const zfchar *categoryTag = zfnull;
        switch(childLayer)
        {
            case ZFUIViewChildLayer::e_Normal:
                zfCoreCriticalShouldNotGoHere();
                break;
            case ZFUIViewChildLayer::e_InternalImpl:
                views = &(this->layerInternalImpl.views);
                if(ref != zfnull)
                {
                    viewsRef = &(ref->d->layerInternalImpl.views);
                }
                break;
                categoryTag = ZFSerializableKeyword_ZFUIView_internalImplView;
            case ZFUIViewChildLayer::e_InternalBg:
                views = &(this->layerInternalBg.views);
                if(ref != zfnull)
                {
                    viewsRef = &(ref->d->layerInternalBg.views);
                }
                categoryTag = ZFSerializableKeyword_ZFUIView_internalBgView;
                break;
            case ZFUIViewChildLayer::e_InternalFg:
                views = &(this->layerInternalFg.views);
                if(ref != zfnull)
                {
                    viewsRef = &(ref->d->layerInternalFg.views);
                }
                categoryTag = ZFSerializableKeyword_ZFUIView_internalFgView;
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return zffalse;
        }
        if(ref == zfnull || viewsRef->count() == 0)
        {
            for(zfindex i = 0; i < views->count(); ++i)
            {
                ZFUIView *tmp = views->get(i)->to<ZFUIView *>();
                if(tmp->viewId().isEmpty()
                    || this->internalViewAutoSerializeTags.find(tmp->viewId().cString()) == this->internalViewAutoSerializeTags.end())
                {
                    continue;
                }
                ZFSerializableData childData;
                if(!ZFObjectToData(childData, tmp, outErrorHint))
                {
                    return zffalse;
                }
                childData.category(categoryTag);
                serializableData.elementAdd(childData);
            }
        }
        else
        {
            for(zfindex i = 0; i < views->count(); ++i)
            {
                ZFUIView *tmp = views->get(i)->to<ZFUIView *>();
                if(tmp->viewId().isEmpty()
                    || this->internalViewAutoSerializeTags.find(tmp->viewId().cString()) == this->internalViewAutoSerializeTags.end())
                {
                    continue;
                }
                zfbool exist = zffalse;
                for(zfindex iRef = viewsRef->count() - 1; iRef != zfindexMax(); --iRef)
                {
                    if(viewsRef->get(iRef)->to<ZFUIView *>()->viewId().compare(tmp->viewId().cString()) == 0)
                    {
                        exist = zftrue;
                        break;
                    }
                }
                if(exist)
                {
                    continue;
                }
                ZFSerializableData childData;
                if(!ZFObjectToData(childData, tmp, outErrorHint))
                {
                    return zffalse;
                }
                childData.category(categoryTag);
                serializableData.elementAdd(childData);
            }
        }
        return zftrue;
    }
    void viewTransformUpdate(ZF_IN ZFUIView *view)
    {
        if(ZFBitTest(this->stateFlag, stateFlag_layoutRequested))
        {
            // update during layout
            ZFBitSet(this->stateFlag, stateFlag_viewTransformUpdate);
        }
        else
        {
            this->viewTransformUpdateAction(view);
        }
    }
    void viewTransformUpdateAction(ZF_IN ZFUIView *view)
    {
        ZFPROTOCOL_INTERFACE_CLASS(ZFUIViewTransform) *impl = ZFPROTOCOL_TRY_ACCESS(ZFUIViewTransform);
        if(impl != zfnull)
        {
            ZFBitUnset(this->stateFlag, stateFlag_viewTransformUpdate);
            impl->viewTransform(view);
        }
    }
};

static zfuint _ZFP_ZFUIView_stateFlags = 0;
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIView_stateFlags, ZFLevelZFFrameworkStatic)
{
    ZFGlobalObserver().observerHasAddStateAttach(ZFUIView::EventViewChildOnChange(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewChildOnChange);
    ZFGlobalObserver().observerHasAddStateAttach(ZFUIView::EventViewChildOnAdd(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewChildOnAdd);
    ZFGlobalObserver().observerHasAddStateAttach(ZFUIView::EventViewChildOnRemove(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewChildOnRemove);
    ZFGlobalObserver().observerHasAddStateAttach(ZFUIView::EventViewOnAddToParent(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewOnAddToParent);
    ZFGlobalObserver().observerHasAddStateAttach(ZFUIView::EventViewOnRemoveFromParent(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewOnRemoveFromParent);
    ZFGlobalObserver().observerHasAddStateAttach(ZFUIView::EventViewLayoutOnLayoutRequest(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_layoutOnLayoutRequest);
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIView_stateFlags)
{
    ZFGlobalObserver().observerHasAddStateDetach(ZFUIView::EventViewChildOnChange(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewChildOnChange);
    ZFGlobalObserver().observerHasAddStateDetach(ZFUIView::EventViewChildOnAdd(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewChildOnAdd);
    ZFGlobalObserver().observerHasAddStateDetach(ZFUIView::EventViewChildOnRemove(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewChildOnRemove);
    ZFGlobalObserver().observerHasAddStateDetach(ZFUIView::EventViewOnAddToParent(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewOnAddToParent);
    ZFGlobalObserver().observerHasAddStateDetach(ZFUIView::EventViewOnRemoveFromParent(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewOnRemoveFromParent);
    ZFGlobalObserver().observerHasAddStateDetach(ZFUIView::EventViewLayoutOnLayoutRequest(), &_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_layoutOnLayoutRequest);
}
ZF_GLOBAL_INITIALIZER_END(ZFUIView_stateFlags)

// ============================================================
// ZFUIView
ZFOBJECT_REGISTER(ZFUIView)

ZFOBSERVER_EVENT_REGISTER(ZFUIView, ViewChildOnChange)
ZFOBSERVER_EVENT_REGISTER(ZFUIView, ViewChildOnAdd)
ZFOBSERVER_EVENT_REGISTER(ZFUIView, ViewChildOnRemove)
ZFOBSERVER_EVENT_REGISTER(ZFUIView, ViewOnAddToParent)
ZFOBSERVER_EVENT_REGISTER(ZFUIView, ViewOnRemoveFromParent)
ZFOBSERVER_EVENT_REGISTER(ZFUIView, UIScaleOnChange)
ZFOBSERVER_EVENT_REGISTER(ZFUIView, ViewFocusOnChange)
ZFOBSERVER_EVENT_REGISTER(ZFUIView, ViewOnEvent)
ZFOBSERVER_EVENT_REGISTER(ZFUIView, ViewLayoutOnLayoutRequest)
ZFOBSERVER_EVENT_REGISTER(ZFUIView, ViewLayoutOnMeasureFinish)
ZFOBSERVER_EVENT_REGISTER(ZFUIView, ViewLayoutOnLayoutPrepare)
ZFOBSERVER_EVENT_REGISTER(ZFUIView, ViewLayoutOnLayout)
ZFOBSERVER_EVENT_REGISTER(ZFUIView, ViewLayoutOnLayoutFinish)
ZFOBSERVER_EVENT_REGISTER(ZFUIView, NativeImplViewMarginOnUpdate)

// ============================================================
// serialize
void ZFUIView::serializableRefLayoutParam(ZF_IN ZFUILayoutParam *serializableRefLayoutParam)
{
    zfRetain(serializableRefLayoutParam);
    zfRelease(d->serializableRefLayoutParam);
    d->serializableRefLayoutParam = serializableRefLayoutParam;
}
ZFUILayoutParam *ZFUIView::serializableRefLayoutParam(void)
{
    return d->serializableRefLayoutParam;
}

zfbool ZFUIView::serializableOnSerializeFromData(ZF_IN const ZFSerializableData &serializableData,
                                                 ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */,
                                                 ZF_OUT_OPT ZFSerializableData *outErrorPos /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeFromData(serializableData, outErrorHint, outErrorPos)) {return zffalse;}

    if(this->serializableOnCheckNeedSerializeChildren())
    {
        this->childRemoveAll();
    }
    d->layoutParamChange(this, zfnull);

    for(zfindex i = 0; i < serializableData.elementCount(); ++i)
    {
        const ZFSerializableData &categoryData = serializableData.elementAtIndex(i);
        if(categoryData.resolved()) {continue;}
        const zfchar *category = ZFSerializableUtil::checkCategory(categoryData);
        if(category == zfnull) {continue;}

        if(zfscmpTheSame(category, ZFSerializableKeyword_ZFUIView_child))
        {
            zfautoObject element;
            if(!ZFObjectFromData(element, categoryData, outErrorHint, outErrorPos))
            {
                return zffalse;
            }
            if(element == zfnull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, categoryData,
                    "null view");
                return zffalse;
            }
            if(!element.toObject()->classData()->classIsTypeOf(ZFUIView::ClassData()))
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, categoryData,
                    "%s not type of %s",
                    element.toObject()->objectInfoOfInstance().cString(), ZFUIView::ClassData()->classNameFull());
                return zffalse;
            }
            ZFUIView *child = element.to<ZFUIView *>();
            this->childAdd(child, child->layoutParam());

            categoryData.resolveMark();
        }
        else if(zfscmpTheSame(category, ZFSerializableKeyword_ZFUIView_layoutParam))
        {
            zfautoObject layoutParam;
            if(!ZFObjectFromData(layoutParam, categoryData, outErrorHint, outErrorPos))
            {
                return zffalse;
            }
            if(layoutParam == zfnull)
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, categoryData,
                    "null layoutParam");
                return zffalse;
            }
            if(!layoutParam.toObject()->classData()->classIsTypeOf(ZFUILayoutParam::ClassData()))
            {
                ZFSerializableUtil::errorOccurred(outErrorHint, outErrorPos, categoryData,
                    "%s not type of %s",
                    layoutParam.toObject()->objectInfoOfInstance().cString(), ZFUILayoutParam::ClassData()->classNameFull());
                return zffalse;
            }

            d->layoutParamChange(this, layoutParam);
        }
        else if(zfscmpTheSame(category, ZFSerializableKeyword_ZFUIView_internalImplView))
        {
            if(!d->serializeInternalViewFromCategoryData(this,
                ZFUIViewChildLayer::e_InternalImpl,
                categoryData,
                outErrorHint,
                outErrorPos))
            {
                return zffalse;
            }
        }
        else if(zfscmpTheSame(category, ZFSerializableKeyword_ZFUIView_internalBgView))
        {
            if(!d->serializeInternalViewFromCategoryData(this,
                ZFUIViewChildLayer::e_InternalBg,
                categoryData,
                outErrorHint,
                outErrorPos))
            {
                return zffalse;
            }
        }
        else if(zfscmpTheSame(category, ZFSerializableKeyword_ZFUIView_internalFgView))
        {
            if(!d->serializeInternalViewFromCategoryData(this,
                ZFUIViewChildLayer::e_InternalFg,
                categoryData,
                outErrorHint,
                outErrorPos))
            {
                return zffalse;
            }
        }
    }
    return zftrue;
}
zfbool ZFUIView::serializableOnSerializeToData(ZF_IN_OUT ZFSerializableData &serializableData,
                                               ZF_IN ZFSerializable *referencedOwnerOrNull,
                                               ZF_OUT_OPT zfstring *outErrorHint /* = zfnull */)
{
    if(!zfsuperI(ZFSerializable)::serializableOnSerializeToData(serializableData, referencedOwnerOrNull, outErrorHint)) {return zffalse;}
    zfself *ref = ZFCastZFObject(zfself *, referencedOwnerOrNull);

    // layoutParam
    if(d->layoutParam != zfnull && this->viewParent() != zfnull)
    {
        ZFUILayoutParam *refLayoutParam = ((ref == zfnull) ? zfnull : ref->d->layoutParam);
        if(refLayoutParam == zfnull)
        {
            refLayoutParam = d->serializableRefLayoutParam;
        }
        if(refLayoutParam != zfnull)
        {
            ZFSerializableData categoryData;
            if(!ZFObjectToData(categoryData, d->layoutParam, outErrorHint, refLayoutParam))
            {
                return zffalse;
            }
            if(categoryData.attributeCount() > 0 || categoryData.elementCount() > 0)
            {
                categoryData.category(ZFSerializableKeyword_ZFUIView_layoutParam);
                serializableData.elementAdd(categoryData);
            }
        }
    }

    { // internal views
        if(!d->internalViewAutoSerializeTags.empty())
        {
            if(!d->serializeInternalViewToCategoryData(ZFUIViewChildLayer::e_InternalImpl, serializableData, ref, outErrorHint))
            {
                return zffalse;
            }
            if(!d->serializeInternalViewToCategoryData(ZFUIViewChildLayer::e_InternalBg, serializableData, ref, outErrorHint))
            {
                return zffalse;
            }
            if(!d->serializeInternalViewToCategoryData(ZFUIViewChildLayer::e_InternalFg, serializableData, ref, outErrorHint))
            {
                return zffalse;
            }
        }
    }

    // all children
    if(this->serializableOnCheckNeedSerializeChildren())
    {
        if(ref == zfnull || ref->childCount() == 0)
        {
            for(zfindex i = 0, count = this->childCount(); i < count; ++i)
            {
                ZFSerializableData childData;
                if(!ZFObjectToData(childData, this->childAtIndex(i), outErrorHint))
                {
                    return zffalse;
                }
                childData.category(ZFSerializableKeyword_ZFUIView_child);
                serializableData.elementAdd(childData);
            }
        }
        else
        {
            if(!d->childArrayIsTheSame(this, ref, ZFUIViewChildLayer::e_Normal))
            {
                ZFSerializableUtil::errorOccurred(outErrorHint,
                    "child mismatch, this: %s, ref: %s",
                    this->objectInfoOfInstance().cString(), ref->objectInfoOfInstance().cString());
                return zffalse;
            }
        }
    }
    return zftrue;
}

// ============================================================
// properties
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIView, zfbool, viewVisible)
{
    if(this->viewVisible() != propertyValueOld)
    {
        ZFPROTOCOL_ACCESS(ZFUIView)->viewVisible(this, this->viewVisible());
        this->layoutRequest();
    }
}
ZFPROPERTY_OVERRIDE_ON_VERIFY_DEFINE(ZFUIView, zffloat, viewAlpha)
{
    propertyValue = zfmApplyRange<zffloat>(propertyValue, 0, 1);
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIView, zffloat, viewAlpha)
{
    if(propertyValue != propertyValueOld)
    {
        ZFPROTOCOL_ACCESS(ZFUIView)->viewAlpha(this, this->viewAlpha());
    }
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIView, zfbool, viewFocusable)
{
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIViewFocus) *impl = ZFPROTOCOL_TRY_ACCESS(ZFUIViewFocus);
    if(impl != zfnull)
    {
        impl->viewFocusable(this, this->viewFocusable());
    }
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIView, zfbool, viewUIEnable)
{
    ZFPROTOCOL_ACCESS(ZFUIView)->viewUIEnable(this, this->viewUIEnable());
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIView, zfbool, viewUIEnableTree)
{
    ZFPROTOCOL_ACCESS(ZFUIView)->viewUIEnableTree(this, this->viewUIEnableTree());
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIView, zfbool, viewMouseHoverEventEnable)
{
    ZFPROTOCOL_ACCESS(ZFUIView)->viewMouseHoverEventEnable(this, this->viewMouseHoverEventEnable());
}

ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIView, ZFUISize, viewSizePrefer)
{
    if(this->viewSizePrefer() != propertyValueOld)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIView, ZFUISize, viewSizeMin)
{
    if(this->viewSizeMin() != propertyValueOld)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIView, ZFUISize, viewSizeMax)
{
    if(this->viewSizeMax() != propertyValueOld)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIView, ZFUIColor, viewBackgroundColor)
{
    ZFPROTOCOL_ACCESS(ZFUIView)->viewBackgroundColor(this, this->viewBackgroundColor());
}

ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIView, zffloat, viewTranslateX)
{
    if(propertyValue != propertyValueOld)
    {
        d->viewTransformUpdate(this);
    }
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIView, zffloat, viewTranslateY)
{
    if(propertyValue != propertyValueOld)
    {
        d->viewTransformUpdate(this);
    }
}
ZFPROPERTY_OVERRIDE_ON_VERIFY_DEFINE(ZFUIView, zffloat, viewScaleX)
{
    if(propertyValue < 0)
    {
        propertyValue = 0;
    }
    if(propertyValue != propertyValueOld)
    {
        d->viewTransformUpdate(this);
    }
}
ZFPROPERTY_OVERRIDE_ON_VERIFY_DEFINE(ZFUIView, zffloat, viewScaleY)
{
    if(propertyValue < 0)
    {
        propertyValue = 0;
    }
    if(propertyValue != propertyValueOld)
    {
        d->viewTransformUpdate(this);
    }
}
ZFPROPERTY_OVERRIDE_ON_VERIFY_DEFINE(ZFUIView, zffloat, viewRotate)
{
    while(propertyValue < 0)
    {
        propertyValue += 360;
    }
    while(propertyValue >= 360)
    {
        propertyValue -= 360;
    }
    if(propertyValue != propertyValueOld)
    {
        d->viewTransformUpdate(this);
    }
}

// ============================================================
// init and dealloc
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIViewNativeViewCache, ZFLevelZFFrameworkNormal)
{
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIViewNativeViewCache)
{
    while(!this->nativeViewCache.isEmpty())
    {
        ZFPROTOCOL_ACCESS(ZFUIView)->nativeViewDestroy(this->nativeViewCache.removeLastAndGet());
    }
}
ZFCoreArrayPOD<void *> nativeViewCache;
ZF_GLOBAL_INITIALIZER_END(ZFUIViewNativeViewCache)

void ZFUIView::objectOnInit(void)
{
    zfsuper::objectOnInit();

    d = zfpoolNew(_ZFP_ZFUIViewPrivate);

    d->measureResult = zflockfree_zfAllocWithCache(ZFUIViewMeasureResult);
    if(ZFFrameworkStateCheck(ZFLevelZFFrameworkNormal) != ZFFrameworkStateAvailable)
    {
        d->nativeView = ZFPROTOCOL_ACCESS(ZFUIView)->nativeViewCreate(this);
    }
    else
    {
        ZFCoreArrayPOD<void *> &nativeViewCache = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewNativeViewCache)->nativeViewCache;
        if(nativeViewCache.isEmpty())
        {
            d->nativeView = ZFPROTOCOL_ACCESS(ZFUIView)->nativeViewCreate(this);
        }
        else
        {
            d->nativeView = nativeViewCache.removeLastAndGet();
            ZFPROTOCOL_ACCESS(ZFUIView)->nativeViewCacheOnRestore(this, d->nativeView);
        }
    }
}
void ZFUIView::objectOnDealloc(void)
{
    this->nativeImplView(zfnull, zfnull);
    if(ZFFrameworkStateCheck(ZFLevelZFFrameworkNormal) == ZFFrameworkStateAvailable
        && ZFPROTOCOL_ACCESS(ZFUIView)->nativeViewCacheOnSave(d->nativeView)
        && ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewNativeViewCache)->nativeViewCache.count() < 100)
    {
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIViewNativeViewCache)->nativeViewCache.add(d->nativeView);
    }
    else
    {
        ZFPROTOCOL_ACCESS(ZFUIView)->nativeViewDestroy(d->nativeView);
    }
    d->nativeView = zfnull;

    for(zfindex i = d->layerNormal.views.count() - 1; i != zfindexMax(); --i)
    {
        zfRelease(d->layerNormal.views.get(i));
    }
    for(zfindex i = d->layerInternalImpl.views.count() - 1; i != zfindexMax(); --i)
    {
        zfRelease(d->layerInternalImpl.views.get(i));
    }
    for(zfindex i = d->layerInternalBg.views.count() - 1; i != zfindexMax(); --i)
    {
        zfRelease(d->layerInternalBg.views.get(i));
    }
    for(zfindex i = d->layerInternalFg.views.count() - 1; i != zfindexMax(); --i)
    {
        zfRelease(d->layerInternalFg.views.get(i));
    }
    d->layoutParamChange(this, zfnull);
    zfRelease(d->measureResult);
    zfpoolDelete(d);
    d = zfnull;
    zfsuper::objectOnDealloc();
}
void ZFUIView::objectOnDeallocPrepare(void)
{
    // directly remove all children, better performance
    this->implChildOnRemoveAllForDealloc();

    ZFCoreArrayPOD<ZFUIView *> &layerNormal = d->layerNormal.views;
    ZFCoreArrayPOD<ZFUIView *> &layerInternalFg = d->layerInternalFg.views;
    ZFCoreArrayPOD<ZFUIView *> &layerInternalBg = d->layerInternalBg.views;
    ZFCoreArrayPOD<ZFUIView *> &layerInternalImpl = d->layerInternalImpl.views;

    for(zfindex i = layerNormal.count() - 1; i != zfindexMax(); --i)
    {
        ZFUIView *child = layerNormal[i];
        child->_ZFP_ZFUIView_parentChanged(zfnull, zfnull, ZFUIViewChildLayer::e_Normal);
        this->viewChildOnRemove(child, ZFUIViewChildLayer::e_Normal);
        child->viewOnRemoveFromParent(this);
        zfRelease(child);
    }
    for(zfindex i = layerInternalFg.count() - 1; i != zfindexMax(); --i)
    {
        ZFUIView *child = layerInternalFg[i];
        child->_ZFP_ZFUIView_parentChanged(zfnull, zfnull, ZFUIViewChildLayer::e_Normal);
        this->viewChildOnRemove(child, ZFUIViewChildLayer::e_InternalFg);
        child->viewOnRemoveFromParent(this);
        zfRelease(child);
    }
    for(zfindex i = layerInternalBg.count() - 1; i != zfindexMax(); --i)
    {
        ZFUIView *child = layerInternalBg[i];
        child->_ZFP_ZFUIView_parentChanged(zfnull, zfnull, ZFUIViewChildLayer::e_Normal);
        this->viewChildOnRemove(child, ZFUIViewChildLayer::e_InternalBg);
        child->viewOnRemoveFromParent(this);
        zfRelease(child);
    }
    for(zfindex i = layerInternalImpl.count() - 1; i != zfindexMax(); --i)
    {
        ZFUIView *child = layerInternalImpl[i];
        child->_ZFP_ZFUIView_parentChanged(zfnull, zfnull, ZFUIViewChildLayer::e_Normal);
        this->viewChildOnRemove(child, ZFUIViewChildLayer::e_InternalImpl);
        child->viewOnRemoveFromParent(this);
        zfRelease(child);
    }

    layerNormal.removeAll();
    layerInternalFg.removeAll();
    layerInternalBg.removeAll();
    layerInternalImpl.removeAll();
    this->viewChildOnChange();

    zfsuper::objectOnDeallocPrepare();
}

zfidentity ZFUIView::objectHash(void)
{
    zfidentity hash = zfidentityHash(zfidentityCalcString(this->classData()->classNameFull()) , this->childCount());
    for(zfindex i = 0; i < this->childCount(); ++i)
    {
        hash = zfidentityHash(hash, this->childAtIndex(i)->objectHash());
    }
    return hash;
}
ZFCompareResult ZFUIView::objectCompare(ZF_IN ZFObject *anotherObj)
{
    if(this == anotherObj) {return ZFCompareTheSame;}
    zfself *another = ZFCastZFObject(zfself *, anotherObj);
    if(another == zfnull) {return ZFCompareUncomparable;}

    if(this->childCount() != another->childCount())
    {
        return ZFCompareUncomparable;
    }
    if(ZFObjectCompare(d->layoutParam, another->d->layoutParam) != ZFCompareTheSame)
    {
        if((d->layoutParam == zfnull || ZFObjectCompare(d->layoutParam, d->serializableRefLayoutParam) == ZFCompareTheSame)
            != (another->d->layoutParam == zfnull || ZFObjectCompare(another->d->layoutParam, another->d->serializableRefLayoutParam) == ZFCompareTheSame))
        {
            return ZFCompareUncomparable;
        }
    }
    if(!ZFClassUtil::allPropertyIsEqual(this, another)
        || !d->childArrayIsTheSame(this, another, ZFUIViewChildLayer::e_InternalImpl)
        || !d->childArrayIsTheSame(this, another, ZFUIViewChildLayer::e_InternalBg)
        || !d->childArrayIsTheSame(this, another, ZFUIViewChildLayer::e_InternalFg)
        || !d->childArrayIsTheSame(this, another, ZFUIViewChildLayer::e_Normal))
    {
        return ZFCompareUncomparable;
    }
    return ZFCompareTheSame;
}

void ZFUIView::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);

    if(!this->viewId().isEmpty())
    {
        zfstringAppend(ret, " [%s]", this->viewId().cString());
    }

    ret += " ";
    ZFUIRectToString(ret, this->viewFrame());

    if(!this->viewVisible())
    {
        ret += " Hidden";
    }

    if(!this->viewUIEnableTree())
    {
        ret += " UITreeDisabled";
    }
    else if(!this->viewUIEnable())
    {
        ret += " UIDisabled";
    }
}

ZFMETHOD_DEFINE_0(ZFUIView, void *, nativeImplView)
{
    return d->nativeImplView;
}
void ZFUIView::nativeImplView(ZF_IN void *nativeImplView,
                              ZF_IN ZFUIViewNativeImplViewDeleteCallback nativeImplViewDeleteCallback)
{
    void *nativeImplViewOld = d->nativeImplView;
    ZFUIViewNativeImplViewDeleteCallback nativeImplViewDeleteCallbackOld = d->nativeImplViewDeleteCallback;
    d->nativeImplView = nativeImplView;
    d->nativeImplViewDeleteCallback = nativeImplViewDeleteCallback;
    ZFPROTOCOL_ACCESS(ZFUIView)->nativeImplView(this, nativeImplViewOld, d->nativeImplView, d->layerInternalImpl.views.count());
    if(nativeImplViewOld && nativeImplViewDeleteCallbackOld)
    {
        nativeImplViewDeleteCallbackOld(this, nativeImplViewOld);
    }
}
ZFMETHOD_DEFINE_0(ZFUIView, const ZFUIMargin &, nativeImplViewMargin)
{
    return d->nativeImplViewMargin;
}
ZFMETHOD_DEFINE_0(ZFUIView, void, nativeImplViewMarginUpdate)
{
    ZFUIMargin newValue = this->nativeImplViewMarginCustom();
    this->nativeImplViewMarginImplUpdate(newValue);
    if(d->nativeImplViewMargin != newValue)
    {
        d->nativeImplViewMargin = newValue;
        this->layoutRequest();
        this->nativeImplViewMarginOnUpdate();
    }
}
ZFMETHOD_DEFINE_0(ZFUIView, const ZFUIRect &, nativeImplViewFrame)
{
    return d->nativeImplViewFrame;
}

ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIView, ZFUIMargin, nativeImplViewMarginCustom)
{
    if(propertyValue != propertyValueOld)
    {
        this->nativeImplViewMarginUpdate();
    }
}

// ============================================================
void ZFUIView::implChildOnAdd(ZF_IN ZFUIView *child,
                              ZF_IN zfindex virtualIndex,
                              ZF_IN ZFUIViewChildLayerEnum childLayer,
                              ZF_IN zfindex childLayerIndex)
{
    ZFPROTOCOL_ACCESS(ZFUIView)->childAdd(this, child, virtualIndex, childLayer, childLayerIndex);
}
void ZFUIView::implChildOnRemove(ZF_IN ZFUIView *child,
                                 ZF_IN zfindex virtualIndex,
                                 ZF_IN ZFUIViewChildLayerEnum childLayer,
                                 ZF_IN zfindex childLayerIndex)
{
    ZFPROTOCOL_ACCESS(ZFUIView)->childRemove(this, child, virtualIndex, childLayer, childLayerIndex);
}
void ZFUIView::implChildOnRemoveAllForDealloc(void)
{
    ZFPROTOCOL_ACCESS(ZFUIView)->childRemoveAllForDealloc(this);
}

// ============================================================
// parent
void ZFUIView::_ZFP_ZFUIView_parentChanged(ZF_IN ZFUIView *viewParent,
                                           ZF_IN ZFUILayoutParam *layoutParam,
                                           ZF_IN ZFUIViewChildLayerEnum viewLayer)
{
    if(viewParent == zfnull)
    {
        d->viewParent = zfnull;
        d->viewFramePrev = ZFUIRectZero();
    }
    else
    {
        d->viewParent = viewParent;
    }

    if(layoutParam != zfnull)
    {
        // only change layoutParam if not null
        // keep old one for performance
        d->layoutParamChange(this, layoutParam);
    }
    else
    {
        this->serializableRefLayoutParam(zfnull);
    }

    d->viewLayer = viewLayer;
}
ZFMETHOD_DEFINE_0(ZFUIView, void *, nativeView)
{
    return d->nativeView;
}
void ZFUIView::_ZFP_ZFUIView_nativeViewNotifyAdd(ZF_IN ZFUIView *view, ZF_IN void *nativeParentView)
{
    zfCoreAssert(view != zfnull && nativeParentView != zfnull);

    zfRetain(view);
    zffloat UIScaleForImpl = ZFPROTOCOL_ACCESS(ZFUIView)->UIScaleForImpl(nativeParentView);
    if(view->d->UIScaleForImpl != UIScaleForImpl)
    {
        view->d->UIScaleForImpl = UIScaleForImpl;
        _ZFP_ZFUIViewPrivate::UIScaleUpdateRecursively(view);
    }
}
void ZFUIView::_ZFP_ZFUIView_nativeViewNotifyRemove(ZF_IN ZFUIView *view)
{
    zfCoreAssert(view != zfnull);
    zfRelease(view);
}

// ============================================================
// view focus
ZFMETHOD_DEFINE_0(ZFUIView, zfbool, viewFocused)
{
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIViewFocus) *impl = ZFPROTOCOL_TRY_ACCESS(ZFUIViewFocus);
    if(impl != zfnull)
    {
        return impl->viewFocused(this);
    }
    else
    {
        return zffalse;
    }
}
ZFMETHOD_DEFINE_1(ZFUIView, void, viewFocusRequest,
                  ZFMP_IN(zfbool, viewFocus))
{
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIViewFocus) *impl = ZFPROTOCOL_TRY_ACCESS(ZFUIViewFocus);
    if(impl != zfnull)
    {
        if(!viewFocus || this->viewFocusable())
        {
            impl->viewFocusRequest(this, viewFocus);
        }
    }
}
ZFMETHOD_DEFINE_0(ZFUIView, ZFUIView *, viewFocusFind)
{
    ZFPROTOCOL_INTERFACE_CLASS(ZFUIViewFocus) *impl = ZFPROTOCOL_TRY_ACCESS(ZFUIViewFocus);
    if(impl == zfnull)
    {
        return zfnull;
    }
    else
    {
        return impl->viewFocusFind(this);
    }
}

// ============================================================
// parent
ZFMETHOD_DEFINE_0(ZFUIView, ZFUIView *, viewParent)
{
    return d->viewParent;
}

ZFMETHOD_DEFINE_0(ZFUIView, void, viewRemoveFromParent)
{
    if(this->viewParent() != zfnull)
    {
        switch(this->viewLayer())
        {
            case ZFUIViewChildLayer::e_Normal:
                this->viewParent()->childRemove(this);
                break;
            case ZFUIViewChildLayer::e_InternalImpl:
                this->viewParent()->internalImplViewRemove(this);
                break;
            case ZFUIViewChildLayer::e_InternalBg:
                this->viewParent()->internalBgViewRemove(this);
                break;
            case ZFUIViewChildLayer::e_InternalFg:
                this->viewParent()->internalFgViewRemove(this);
                break;
            default:
                zfCoreCriticalShouldNotGoHere();
                return ;
        }
    }
}

// ============================================================
// scale settings
ZFPROPERTY_OVERRIDE_ON_VERIFY_DEFINE(ZFUIView, zffloat, UIScale)
{
    if(propertyValue < 0)
    {
        propertyValue = 0;
    }
}
ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIView, zffloat, UIScale)
{
    if(propertyValue != propertyValueOld)
    {
        _ZFP_ZFUIViewPrivate::UIScaleUpdateRecursively(this);
    }
}
ZFMETHOD_DEFINE_0(ZFUIView, zffloat, UIScaleInherited)
{
    return d->UIScaleInherited;
}
ZFMETHOD_DEFINE_0(ZFUIView, zffloat, UIScaleForImpl)
{
    return d->UIScaleForImpl;
}
ZFMETHOD_DEFINE_0(ZFUIView, zffloat, UIScaleForPixel)
{
    return d->UIScaleFixed * ZFPROTOCOL_ACCESS(ZFUIView)->UIScaleForPixel(this->nativeView());
}
ZFMETHOD_DEFINE_0(ZFUIView, zffloat, UIScaleFixed)
{
    return d->UIScaleFixed;
}
void ZFUIView::UIScaleOnChange(void)
{
    this->observerNotify(ZFUIView::EventUIScaleOnChange());
}

// ============================================================
// layout logic
ZFMETHOD_DEFINE_0(ZFUIView, zfautoObjectT<ZFUILayoutParam *>, layoutParamCreate)
{
    zfautoObjectT<ZFUILayoutParam *> layoutParam = this->layoutParamClass()->newInstance();
    if(layoutParam == zfnull || !layoutParam.toObject()->classData()->classIsTypeOf(ZFUILayoutParam::ClassData()))
    {
        return zfnull;
    }
    this->layoutParamOnUpdate(layoutParam.to<ZFUILayoutParam *>());
    return layoutParam;
}
const ZFClass *ZFUIView::layoutParamClass(void)
{
    return ZFUILayoutParam::ClassData();
}

ZFMETHOD_DEFINE_1(ZFUIView, void, layoutParam,
                  ZFMP_IN(ZFUILayoutParam *, layoutParam))
{
    if(this->viewParent() != zfnull && layoutParam != zfnull && !layoutParam->classData()->classIsTypeOf(this->layoutParamClass()))
    {
        zfautoObjectT<ZFUILayoutParam *> layoutParamHolder = this->layoutParamCreate();
        ZFUILayoutParam *layoutParamTmp = layoutParamHolder.to<ZFUILayoutParam *>();
        layoutParamTmp->styleableCopyFrom(layoutParam);
        d->layoutParamChange(this, layoutParamTmp);
    }
    else
    {
        d->layoutParamChange(this, layoutParam);
    }
}
ZFMETHOD_DEFINE_0(ZFUIView, ZFUILayoutParam *, layoutParam)
{
    return d->layoutParam;
}

ZFMETHOD_DEFINE_0(ZFUIView, void, layoutRequest)
{
    d->layoutRequest(this, zftrue);
}
ZFMETHOD_DEFINE_0(ZFUIView, zfbool, layoutRequested)
{
    return ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutRequested);
}
ZFMETHOD_DEFINE_0(ZFUIView, zfbool, layouting)
{
    return ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layouting);
}
ZFMETHOD_DEFINE_2(ZFUIView, const ZFUISize &, layoutMeasure,
                  ZFMP_IN(const ZFUISize &, sizeHint),
                  ZFMP_IN(const ZFUISizeParam &, sizeParam))
{
    if(ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_viewFrameOverrideFlag))
    {
        d->measureResult->measuredSize.width = d->viewFrame.width;
        d->measureResult->measuredSize.height = d->viewFrame.height;
        return d->measureResult->measuredSize;
    }

    if(ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutRequested)
        || !ZFUISizeIsEqual(d->measureResult->sizeHint, sizeHint)
        || !ZFUISizeParamIsEqual(d->measureResult->sizeParam, sizeParam))
    {
        if(!ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutRequested)
            && ZFUISizeParamIsEqual(d->measureResult->sizeParam, sizeParam))
        {
            // if all these cond matchs, result is ensured not changed
            // * prev measure with wrap, and result a size less than size hint
            // * cur measure with a larger size hint than prev
            if(zftrue
                && (d->measureResult->sizeHint.width == sizeHint.width
                    || (sizeParam.width == ZFUISizeType::e_Wrap && (
                        (d->measureResult->sizeHint.width < 0 && (sizeHint.width < 0 || d->measureResult->measuredSize.width <= sizeHint.width))
                        || (d->measureResult->measuredSize.width < d->measureResult->sizeHint.width && (sizeHint.width == -1 || sizeHint.width >= d->measureResult->sizeHint.width))
                    )))
                && (d->measureResult->sizeHint.height == sizeHint.height
                    || (sizeParam.height == ZFUISizeType::e_Wrap && (
                        (d->measureResult->sizeHint.height < 0 && (sizeHint.height < 0 || d->measureResult->measuredSize.height <= sizeHint.height))
                        || (d->measureResult->measuredSize.height < d->measureResult->sizeHint.height && (sizeHint.height == -1 || sizeHint.height >= d->measureResult->sizeHint.height))
                    )))
                )
            {
                return d->measureResult->measuredSize;
            }
        }

        d->measureResult->measuredSize = ZFUISizeInvalid();
        d->measureResult->sizeHint = sizeHint;
        d->measureResult->sizeParam = sizeParam;
        if(sizeParam.width == ZFUISizeType::e_Fill && sizeParam.height == ZFUISizeType::e_Fill)
        {
            d->measureResult->sizeHint = sizeHint;
            d->measureResult->measuredSize.width = zfmMax((zffloat)0, d->measureResult->measuredSize.width);
            d->measureResult->measuredSize.height = zfmMax((zffloat)0, d->measureResult->measuredSize.height);
        }
        else
        {
            this->layoutOnMeasure(d->measureResult->measuredSize, sizeHint, sizeParam);
        }

        this->layoutOnMeasureFinish(d->measureResult->measuredSize, sizeHint, sizeParam);
        {
            this->observerNotify(ZFUIView::EventViewLayoutOnMeasureFinish(), d->measureResult);
        }

        if(d->measureResult->measuredSize.width < 0)
        {
            if(this->viewSizePrefer().width >= 0)
            {
                d->measureResult->measuredSize.width = this->viewSizePrefer().width;
            }
            else
            {
                d->measureResult->measuredSize.width = 0;
            }
        }
        if(d->measureResult->measuredSize.height < 0)
        {
            if(this->viewSizePrefer().height >= 0)
            {
                d->measureResult->measuredSize.height = this->viewSizePrefer().height;
            }
            else
            {
                d->measureResult->measuredSize.height = 0;
            }
        }

        ZFUILayoutParam::sizeHintApply(d->measureResult->measuredSize, d->measureResult->measuredSize, sizeHint, sizeParam);
        ZFUISizeApplyRange(d->measureResult->measuredSize, d->measureResult->measuredSize, this->viewSizeMin(), this->viewSizeMax());
    }
    return d->measureResult->measuredSize;
}
ZFMETHOD_DEFINE_0(ZFUIView, const ZFUISize &, layoutMeasuredSize)
{
    return d->measureResult->measuredSize;
}

ZFMETHOD_DEFINE_0(ZFUIView, ZFUIRect const &, viewFrame)
{
    return d->viewFrame;
}
ZFMETHOD_DEFINE_1(ZFUIView, void, viewFrame,
                  ZFMP_IN(ZFUIRect const &, viewFrame))
{
    if(!ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layouting)
        && (d->viewParent == zfnull || !ZFBitTest(d->viewParent->d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layouting)))
    { // changed by user or animation
        ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_viewFrameOverrideFlag);
        d->viewFrameUpdate(viewFrame);
        if(d->viewFrame.width != d->viewFramePrev.width
            || d->viewFrame.height != d->viewFramePrev.height)
        {
            // request layout only for the changed view
            d->layoutRequest(this, zffalse);
        }
        d->viewFrameUpdateForImpl(this);
        return;
    }
    // else, changed by parent layout step

    if(ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_viewFrameOverrideFlag))
    { // user has set viewFrame, ignore parent layout logic
        if(!ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layouting))
        {
            d->layoutAction(this, ZFUIRectGetBounds(d->viewFrame));
        }
        return;
    }

    if(ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutRequested)
        || d->viewFrame.width != viewFrame.width
        || d->viewFrame.height != viewFrame.height)
    {
        d->viewFrameUpdate(viewFrame);
        if(d->viewFrame != d->viewFramePrev || ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_UIScaleChanged))
        {
            d->viewFrameUpdateForImpl(this);
        }

        ZFUIRect bounds = ZFUIRectGetBounds(d->viewFrame);

        d->layoutAction(this, bounds);
    }
    else if(d->viewFrame != viewFrame || ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_UIScaleChanged)) {
        d->viewFrameUpdate(viewFrame);
        // size not changed but point changed, notify impl to move the view is enough
        d->viewFrameUpdateForImpl(this);
    }
    ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutRequested);
    ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutRequestedRecursively);
    ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_UIScaleChanged);
}
ZFMETHOD_DEFINE_0(ZFUIView, const ZFUIRect &, viewFramePrev)
{
    return d->viewFramePrev;
}

ZFMETHOD_DEFINE_0(ZFUIView, void, viewFrameReset)
{
    if(ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_viewFrameOverrideFlag))
    {
        ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_viewFrameOverrideFlag);
        if(this->viewParent() != zfnull)
        {
            this->viewParent()->layoutRequest();
        }
    }
}

ZFMETHOD_DEFINE_0(ZFUIView, zffloat const &, viewX)
{
    return this->viewFrame().x;
}
ZFMETHOD_DEFINE_1(ZFUIView, void, viewX, ZFMP_IN(zffloat const &, propertyValue))
{
    ZFUIRect viewFrame = this->viewFrame();
    viewFrame.x = propertyValue;
    this->viewFrame(viewFrame);
}
ZFMETHOD_DEFINE_0(ZFUIView, zffloat const &, viewY)
{
    return this->viewFrame().y;
}
ZFMETHOD_DEFINE_1(ZFUIView, void, viewY, ZFMP_IN(zffloat const &, propertyValue))
{
    ZFUIRect viewFrame = this->viewFrame();
    viewFrame.y = propertyValue;
    this->viewFrame(viewFrame);
}
ZFMETHOD_DEFINE_0(ZFUIView, zffloat const &, viewWidth)
{
    return this->viewFrame().width;
}
ZFMETHOD_DEFINE_1(ZFUIView, void, viewWidth, ZFMP_IN(zffloat const &, propertyValue))
{
    ZFUIRect viewFrame = this->viewFrame();
    viewFrame.width = propertyValue;
    this->viewFrame(viewFrame);
}
ZFMETHOD_DEFINE_0(ZFUIView, zffloat const &, viewHeight)
{
    return this->viewFrame().height;
}
ZFMETHOD_DEFINE_1(ZFUIView, void, viewHeight, ZFMP_IN(zffloat const &, propertyValue))
{
    ZFUIRect viewFrame = this->viewFrame();
    viewFrame.height = propertyValue;
    this->viewFrame(viewFrame);
}
ZFMETHOD_DEFINE_0(ZFUIView, zffloat const &, viewCenterX)
{
    return d->viewCenter.x;
}
ZFMETHOD_DEFINE_1(ZFUIView, void, viewCenterX, ZFMP_IN(zffloat const &, propertyValue))
{
    ZFUIRect viewFrame = this->viewFrame();
    viewFrame.x = propertyValue - viewFrame.width / 2;
    this->viewFrame(viewFrame);
}
ZFMETHOD_DEFINE_0(ZFUIView, zffloat const &, viewCenterY)
{
    return d->viewCenter.y;
}
ZFMETHOD_DEFINE_1(ZFUIView, void, viewCenterY, ZFMP_IN(zffloat const &, propertyValue))
{
    ZFUIRect viewFrame = this->viewFrame();
    viewFrame.y = propertyValue - viewFrame.height / 2;
    this->viewFrame(viewFrame);
}

ZFMETHOD_DEFINE_0(ZFUIView, void, layoutIfNeed)
{
    if(ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layoutRequested))
    {
        this->_ZFP_ZFUIView_notifyLayoutView(this->viewFrame());
    }
}

ZFMETHOD_DEFINE_0(ZFUIView, ZFUIPoint, layoutChildOffset)
{
    ZFUIPoint ret = ZFUIPointZero();
    this->layoutChildOffsetOnUpdate(ret);
    return ret;
}

void ZFUIView::_ZFP_ZFUIView_notifyLayoutView(ZF_IN const ZFUIRect &viewFrame)
{
    ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layouting);
    this->viewFrame(viewFrame);
    ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_layouting);
}

void ZFUIView::layoutOnLayoutRequest(void)
{
    if(ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_layoutOnLayoutRequest)
        || ZFBitTest(_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_layoutOnLayoutRequest))
    {
        this->observerNotify(ZFUIView::EventViewLayoutOnLayoutRequest());
    }
}
void ZFUIView::layoutOnLayout(ZF_IN const ZFUIRect &bounds)
{
    for(zfindex i = 0; i < this->childCount(); ++i)
    {
        ZFUIView *child = this->childAtIndex(i);
        child->viewFrame(
            ZFUILayoutParam::layoutParamApply(
                bounds,
                child,
                child->layoutParam()));
    }
}

// ============================================================
// child views
ZFMETHOD_DEFINE_3(ZFUIView, ZFUIView *, childFindById,
                  ZFMP_IN(const zfchar *, viewId),
                  ZFMP_IN_OPT(zfbool, findRecursively, zftrue),
                  ZFMP_IN_OPT(zfbool, includeInternalViews, zffalse))
{
    if(viewId == zfnull || *viewId == '\0')
    {
        return zfnull;
    }

    if(!findRecursively)
    {
        for(zfindex i = 0; i < d->layerNormal.views.count(); ++i)
        {
            if(zfscmpTheSame(d->layerNormal.views[i]->viewId().cString(), viewId))
            {
                return d->layerNormal.views[i];
            }
        }
        if(includeInternalViews)
        {
            for(zfindex i = 0; i < d->layerInternalImpl.views.count(); ++i)
            {
                if(zfscmpTheSame(d->layerInternalImpl.views[i]->viewId().cString(), viewId))
                {
                    return d->layerInternalImpl.views[i];
                }
            }
            for(zfindex i = 0; i < d->layerInternalBg.views.count(); ++i)
            {
                if(zfscmpTheSame(d->layerInternalBg.views[i]->viewId().cString(), viewId))
                {
                    return d->layerInternalBg.views[i];
                }
            }
            for(zfindex i = 0; i < d->layerInternalFg.views.count(); ++i)
            {
                if(zfscmpTheSame(d->layerInternalFg.views[i]->viewId().cString(), viewId))
                {
                    return d->layerInternalFg.views[i];
                }
            }
        }
        return zfnull;
    }

    ZFCoreArrayPOD<ZFUIView *> toFind;
    toFind.add(this);
    while(!toFind.isEmpty())
    {
        ZFUIView *view = toFind.get(0);
        toFind.remove(0);
        if(zfscmpTheSame(view->viewId().cString(), viewId))
        {
            return view;
        }
        toFind.addFrom(view->childArray());
        if(includeInternalViews)
        {
            toFind.addFrom(view->d->layerInternalImpl.views);
            toFind.addFrom(view->d->layerInternalBg.views);
            toFind.addFrom(view->d->layerInternalFg.views);
        }
    }
    return zfnull;
}

ZFMETHOD_DEFINE_3(ZFUIView, void, childAdd,
                  ZFMP_IN(ZFUIView *, view),
                  ZFMP_IN_OPT(ZFUILayoutParam *, layoutParam, zfnull),
                  ZFMP_IN_OPT(zfindex, atIndex, zfindexMax()))
{
    d->childAdd(this, ZFUIViewChildLayer::e_Normal, d->layerNormal, view, layoutParam, atIndex);
}
ZFMETHOD_DEFINE_5(ZFUIView, void, childAdd,
                  ZFMP_IN(ZFUIView *, view),
                  ZFMP_IN(const ZFUISizeParam &, sizeParam),
                  ZFMP_IN_OPT(ZFUIAlignFlags const &, layoutAlign, ZFUIAlign::e_LeftInner | ZFUIAlign::e_TopInner),
                  ZFMP_IN_OPT(ZFUIMargin const &, layoutMargin, ZFUIMarginZero()),
                  ZFMP_IN_OPT(const ZFUISize &, sizeHint, ZFUISizeInvalid()))
{
    this->childAdd(view);
    ZFUILayoutParam *lp = view->layoutParam();
    lp->sizeParam(sizeParam);
    lp->layoutAlign(layoutAlign);
    lp->layoutMargin(layoutMargin);
    lp->sizeHint(sizeHint);
}
ZFMETHOD_DEFINE_1(ZFUIView, void, childRemove,
                  ZFMP_IN(ZFUIView *, view))
{
    d->childRemove(this, ZFUIViewChildLayer::e_Normal, d->layerNormal, view);
}
ZFMETHOD_DEFINE_1(ZFUIView, void, childRemoveAtIndex,
                  ZFMP_IN(zfindex, index))
{
    d->childRemoveAtIndex(this, ZFUIViewChildLayer::e_Normal, d->layerNormal, index);
}
ZFMETHOD_DEFINE_0(ZFUIView, void, childRemoveAll)
{
    d->childRemoveAll(this, ZFUIViewChildLayer::e_Normal, d->layerNormal);
}

ZFMETHOD_DEFINE_2(ZFUIView, void, childMove,
                  ZFMP_IN(zfindex, fromIndex),
                  ZFMP_IN(zfindex, toIndexOrIndexMax))
{
    d->childMove(this, ZFUIViewChildLayer::e_Normal, d->layerNormal, fromIndex, toIndexOrIndexMax);
}
ZFMETHOD_DEFINE_2(ZFUIView, void, childMove,
                  ZFMP_IN(ZFUIView *, child),
                  ZFMP_IN(zfindex, toIndexOrIndexMax))
{
    d->childMove(this, ZFUIViewChildLayer::e_Normal, d->layerNormal, child, toIndexOrIndexMax);
}
ZFMETHOD_DEFINE_2(ZFUIView, void, childReplaceAtIndex,
                  ZFMP_IN(zfindex, atIndex),
                  ZFMP_IN(ZFUIView *, toReplace))
{
    d->childReplace(this, ZFUIViewChildLayer::e_Normal, d->layerNormal, atIndex, toReplace);
}

ZFMETHOD_DEFINE_0(ZFUIView, zfindex, childCount)
{
    return d->childCount(d->layerNormal);
}
ZFMETHOD_DEFINE_1(ZFUIView, ZFUIView *, childAtIndex,
                  ZFMP_IN(zfindex, index))
{
    return d->childAtIndex(d->layerNormal, index);
}
ZFMETHOD_DEFINE_1(ZFUIView, zfindex, childFind,
                  ZFMP_IN(ZFUIView *, view))
{
    return d->childFind(d->layerNormal, view);
}
ZFMETHOD_DEFINE_0(ZFUIView, ZFCoreArrayPOD<ZFUIView *>, childArray)
{
    return d->layerNormal.views;
}
ZFMETHOD_DEFINE_0(ZFUIView, ZFUIViewChildLayerEnum, viewLayer)
{
    return d->viewLayer;
}
ZFMETHOD_DEFINE_0(ZFUIView, ZFCoreArrayPOD<ZFUIView *>, childRawArray)
{
    ZFCoreArrayPOD<ZFUIView *> ret;
    ret.capacity(
        d->layerInternalBg.views.count()
        + d->layerInternalBg.views.count()
        + this->childCount()
        + d->layerInternalFg.views.count());
    ret.addFrom(d->layerInternalImpl.views);
    ret.addFrom(d->layerInternalBg.views);
    ret.addFrom(d->layerNormal.views);
    ret.addFrom(d->layerInternalFg.views);
    return ret;
}

// ============================================================
// events
void ZFUIView::viewChildOnChange(void)
{
    if(ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewChildOnChange)
        || ZFBitTest(_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewChildOnChange))
    {
        this->observerNotify(ZFUIView::EventViewChildOnChange());
    }
}
void ZFUIView::viewChildOnAdd(ZF_IN ZFUIView *child,
                              ZF_IN ZFUIViewChildLayerEnum childLayer)
{
    if(ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewChildOnAdd)
        || ZFBitTest(_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewChildOnAdd))
    {
        zfblockedAlloc(ZFUIViewChildLayer, t, childLayer);
        this->observerNotify(ZFUIView::EventViewChildOnAdd(), child, t);
    }
}
void ZFUIView::viewChildOnRemove(ZF_IN ZFUIView *child,
                                 ZF_IN ZFUIViewChildLayerEnum childLayer)
{
    if(ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewChildOnRemove)
        || ZFBitTest(_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewChildOnRemove))
    {
        zfblockedAlloc(ZFUIViewChildLayer, t, childLayer);
        this->observerNotify(ZFUIView::EventViewChildOnRemove(), child, t);
    }
}
void ZFUIView::viewOnAddToParent(ZF_IN ZFUIView *parent)
{
    if(ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewOnAddToParent)
        || ZFBitTest(_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewOnAddToParent))
    {
        this->observerNotify(ZFUIView::EventViewOnAddToParent(), parent);
    }
}
void ZFUIView::viewOnRemoveFromParent(ZF_IN ZFUIView *parent)
{
    if(ZFBitTest(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewOnRemoveFromParent)
        || ZFBitTest(_ZFP_ZFUIView_stateFlags, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewOnRemoveFromParent))
    {
        this->observerNotify(ZFUIView::EventViewOnRemoveFromParent(), parent);
    }
}

// ============================================================
// internal views
ZFMETHOD_DEFINE_3(ZFUIView, void, internalImplViewAdd,
                  ZFMP_IN(ZFUIView *, view),
                  ZFMP_IN_OPT(ZFUILayoutParam *, layoutParam, zfnull),
                  ZFMP_IN_OPT(zfbool, addAsTopMost, zftrue))
{
    d->childAdd(this, ZFUIViewChildLayer::e_InternalImpl, d->layerInternalImpl, view, layoutParam, (addAsTopMost ? zfindexMax() : 0));
}
ZFMETHOD_DEFINE_1(ZFUIView, void, internalImplViewRemove,
                  ZFMP_IN(ZFUIView *, view))
{
    d->childRemove(this, ZFUIViewChildLayer::e_InternalImpl, d->layerInternalImpl, view);
}
ZFMETHOD_DEFINE_0(ZFUIView, ZFCoreArrayPOD<ZFUIView *>, internalImplViewArray)
{
    return d->layerInternalImpl.views;
}

ZFMETHOD_DEFINE_3(ZFUIView, void, internalBgViewAdd,
                  ZFMP_IN(ZFUIView *, view),
                  ZFMP_IN_OPT(ZFUILayoutParam *, layoutParam, zfnull),
                  ZFMP_IN_OPT(zfbool, addAsTopMost, zftrue))
{
    d->childAdd(this, ZFUIViewChildLayer::e_InternalBg, d->layerInternalBg, view, layoutParam, (addAsTopMost ? zfindexMax() : 0));
}
ZFMETHOD_DEFINE_1(ZFUIView, void, internalBgViewRemove,
                  ZFMP_IN(ZFUIView *, view))
{
    d->childRemove(this, ZFUIViewChildLayer::e_InternalBg, d->layerInternalBg, view);
}
ZFMETHOD_DEFINE_0(ZFUIView, ZFCoreArrayPOD<ZFUIView *>, internalBgViewArray)
{
    return d->layerInternalBg.views;
}

ZFMETHOD_DEFINE_3(ZFUIView, void, internalFgViewAdd,
                  ZFMP_IN(ZFUIView *, view),
                  ZFMP_IN_OPT(ZFUILayoutParam *, layoutParam, zfnull),
                  ZFMP_IN_OPT(zfbool, addAsTopMost, zftrue))
{
    d->childAdd(this, ZFUIViewChildLayer::e_InternalFg, d->layerInternalFg, view, layoutParam, (addAsTopMost ? zfindexMax() : 0));
}
ZFMETHOD_DEFINE_1(ZFUIView, void, internalFgViewRemove,
                  ZFMP_IN(ZFUIView *, view))
{
    d->childRemove(this, ZFUIViewChildLayer::e_InternalFg, d->layerInternalFg, view);
}
ZFMETHOD_DEFINE_0(ZFUIView, ZFCoreArrayPOD<ZFUIView *>, internalFgViewArray)
{
    return d->layerInternalFg.views;
}

// ============================================================
// other internal view logic
ZFMETHOD_DEFINE_1(ZFUIView, void, internalViewAutoSerializeTagAdd,
                  ZFMP_IN(const zfchar *, tag))
{
    if(zfsIsEmpty(tag))
    {
        return ;
    }
    d->internalViewAutoSerializeTags[tag] = zftrue;
}
ZFMETHOD_DEFINE_1(ZFUIView, void, internalViewAutoSerializeTagRemove,
                  ZFMP_IN(const zfchar *, tag))
{
    if(zfsIsEmpty(tag))
    {
        return ;
    }
    d->internalViewAutoSerializeTags.erase(tag);
}
ZFMETHOD_DEFINE_0(ZFUIView, void, internalViewAutoSerializeTagRemoveAll)
{
    d->internalViewAutoSerializeTags.clear();
}
ZFMETHOD_DEFINE_1(ZFUIView, void, internalViewAutoSerializeTagGetAllT,
                  ZFMP_IN_OUT(ZFCoreArray<zfstring> &, ret))
{
    ret.capacity(ret.count() + d->internalViewAutoSerializeTags.size());
    for(_ZFP_ZFUIViewInternalViewAutoSerializeTagMapType::iterator it = d->internalViewAutoSerializeTags.begin();
        it != d->internalViewAutoSerializeTags.end();
        ++it)
    {
        ret.add(it->first.c_str());
    }
}
ZFMETHOD_DEFINE_0(ZFUIView, ZFCoreArray<zfstring>, internalViewAutoSerializeTagGetAll)
{
    ZFCoreArray<zfstring> ret;
    this->internalViewAutoSerializeTagGetAllT(ret);
    return ret;
}

void ZFUIView::internalViewOnLayout(ZF_IN const ZFUIRect &bounds)
{
    for(zfindex i = 0; i < d->childCount(d->layerInternalImpl); ++i)
    {
        ZFUIView *child = d->childAtIndex(d->layerInternalImpl, i);
        if(this->internalViewShouldLayout(child))
        {
            child->viewFrame(
                ZFUILayoutParam::layoutParamApply(
                    bounds,
                    child,
                    d->childLayoutParamAtIndex(d->layerInternalImpl, i)));
        }
    }
    for(zfindex i = 0; i < d->childCount(d->layerInternalBg); ++i)
    {
        ZFUIView *child = d->childAtIndex(d->layerInternalBg, i);
        if(this->internalViewShouldLayout(child))
        {
            child->viewFrame(
                ZFUILayoutParam::layoutParamApply(
                    bounds,
                    child,
                    d->childLayoutParamAtIndex(d->layerInternalBg, i)));
        }
    }
    for(zfindex i = 0; i < d->childCount(d->layerInternalFg); ++i)
    {
        ZFUIView *child = d->childAtIndex(d->layerInternalFg, i);
        if(this->internalViewShouldLayout(child))
        {
            child->viewFrame(
                ZFUILayoutParam::layoutParamApply(
                    bounds,
                    child,
                    d->childLayoutParamAtIndex(d->layerInternalFg, i)));
        }
    }
}

// ============================================================
// events
ZFMETHOD_DEFINE_1(ZFUIView, void, viewEventSend,
                  ZFMP_IN(ZFUIEvent *, event))
{
    if(event == zfnull)
    {
        return ;
    }

    zfRetain(this);
    zfblockedRelease(this);

    this->viewEventOnEvent(event);
    this->observerNotify(ZFUIView::EventViewOnEvent(), event);
}
void ZFUIView::viewEventOnEvent(ZF_IN ZFUIEvent *event)
{
    const ZFClass *eventClass = event->classData();
    if(eventClass->classIsTypeOf(ZFUIMouseEvent::ClassData()))
    {
        ZFUIMouseEvent *mouseEvent = ZFCastZFObjectUnchecked(ZFUIMouseEvent *, event);
        this->viewEventOnMouseEvent(mouseEvent);
        mouseEvent->eventResolved(zftrue);
    }
    else if(eventClass->classIsTypeOf(ZFUIKeyEvent::ClassData()))
    {
        ZFUIKeyEvent *eventTmp = ZFCastZFObjectUnchecked(ZFUIKeyEvent *, event);
        this->viewEventOnKeyEvent(eventTmp);
        {
            ZFUIView *view = this->viewParent();
            while(view != zfnull && !event->eventResolved())
            {
                view->viewEventOnKeyEvent(eventTmp);
                view = view->viewParent();
            }
        }
    }
    else if(eventClass->classIsTypeOf(ZFUIWheelEvent::ClassData()))
    {
        ZFUIWheelEvent *eventTmp = ZFCastZFObjectUnchecked(ZFUIWheelEvent *, event);
        this->viewEventOnWheelEvent(eventTmp);
        {
            ZFUIView *view = this->viewParent();
            while(view != zfnull && !event->eventResolved())
            {
                view->viewEventOnWheelEvent(eventTmp);
                view = view->viewParent();
            }
        }
    }
}

void ZFUIView::viewEventOnMouseEvent(ZF_IN ZFUIMouseEvent *mouseEvent)
{
    if(this->viewUIEnable())
    {
        if(mouseEvent->mouseAction == ZFUIMouseAction::e_MouseUp
            && this->viewFocusable()
            && this->viewFocusObtainWhenClick())
        {
            this->viewFocusRequest(zftrue);
        }
        mouseEvent->eventResolved(zftrue);
    }
}

void ZFUIView::viewEventOnKeyEvent(ZF_IN ZFUIKeyEvent *keyEvent)
{
    this->viewEventOnKeyEventResolveFocus(keyEvent);
}
void ZFUIView::viewEventOnKeyEventResolveFocus(ZF_IN ZFUIKeyEvent *keyEvent)
{
    ZFUIViewFocusResolveKeyEvent(this, keyEvent);
}

void ZFUIView::viewEventOnWheelEvent(ZF_IN ZFUIWheelEvent *wheelEvent)
{
    // nothing to do
}

// ============================================================
// override
void ZFUIView::styleableOnCopyFrom(ZF_IN ZFStyleable *anotherStyleable)
{
    zfsuperI(ZFStyleable)::styleableOnCopyFrom(anotherStyleable);
    ZFUIView *ref = ZFCastZFObject(ZFUIView *, anotherStyleable);
    if(ref == zfnull || this->childCount() != 0)
    {
        return ;
    }
    for(zfindex i = 0; i < ref->childCount(); ++i)
    {
        zfautoObjectT<ZFUIView *> child = ref->childAtIndex(i)->copy();
        zfautoObjectT<ZFUILayoutParam *> childLayoutParam = ref->childAtIndex(i)->layoutParam()->copy();
        this->childAdd(child, childLayoutParam);
    }
}

void ZFUIView::observerOnAdd(ZF_IN zfidentity eventId)
{
    zfsuper::observerOnAdd(eventId);
    if(eventId == ZFUIView::EventViewLayoutOnLayoutRequest())
    {
        ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_layoutOnLayoutRequest);
    }
    else if(eventId == ZFUIView::EventViewChildOnChange())
    {
        ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewChildOnChange);
    }
    else if(eventId == ZFUIView::EventViewChildOnAdd())
    {
        ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewChildOnAdd);
    }
    else if(eventId == ZFUIView::EventViewChildOnRemove())
    {
        ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewChildOnRemove);
    }
    else if(eventId == ZFUIView::EventViewOnAddToParent())
    {
        ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewOnAddToParent);
    }
    else if(eventId == ZFUIView::EventViewOnRemoveFromParent())
    {
        ZFBitSet(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewOnRemoveFromParent);
    }
}
void ZFUIView::observerOnRemove(ZF_IN zfidentity eventId)
{
    zfsuper::observerOnRemove(eventId);
    if(eventId == ZFUIView::EventViewLayoutOnLayoutRequest())
    {
        ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_layoutOnLayoutRequest);
    }
    else if(eventId == ZFUIView::EventViewChildOnChange())
    {
        ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewChildOnChange);
    }
    else if(eventId == ZFUIView::EventViewChildOnAdd())
    {
        ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewChildOnAdd);
    }
    else if(eventId == ZFUIView::EventViewChildOnRemove())
    {
        ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewChildOnRemove);
    }
    else if(eventId == ZFUIView::EventViewOnAddToParent())
    {
        ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewOnAddToParent);
    }
    else if(eventId == ZFUIView::EventViewOnRemoveFromParent())
    {
        ZFBitUnset(d->stateFlag, _ZFP_ZFUIViewPrivate::stateFlag_observerHasAddFlag_viewOnRemoveFromParent);
    }
}

ZF_NAMESPACE_GLOBAL_END

