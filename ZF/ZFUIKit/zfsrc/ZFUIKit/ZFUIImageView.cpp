#include "ZFUIImageView.h"
#include "protocol/ZFProtocolZFUIView.h"
#include "protocol/ZFProtocolZFUIImageView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIImageViewListenerHolder, ZFLevelZFFrameworkEssential)
{
    this->imageNinePatchChangedListener = ZFCallbackForFunc(zfself::imageNinePatchChanged);
}
public:
    ZFListener imageNinePatchChangedListener;
    static void imageNinePatchChanged(ZF_IN const ZFListenerData &listenerData, ZF_IN ZFObject *userData)
    {
        const ZFProperty *property = listenerData.param0<v_ZFProperty *>()->zfv;
        if(property == ZFPropertyAccess(ZFUIImage, imageNinePatch)
            || property == ZFPropertyAccess(ZFUIImage, imageScale))
        {
            ZFUIImageView *imageView = userData->objectHolded();
            ZFUIImage *image = imageView->image();
            ZFPROTOCOL_ACCESS(ZFUIImageView)->imageNinePatchChanged(
                imageView,
                image->imageScaleFixed(),
                ZFUIMarginApplyScale(image->imageNinePatch(), image->imageScaleFixed()));
        }
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIImageViewListenerHolder)

// ============================================================
// ZFUIImageView
ZFOBJECT_REGISTER(ZFUIImageView)
ZFSTYLE_DEFAULT_DEFINE(ZFUIImageView)

ZFPROPERTY_ON_INIT_DEFINE(ZFUIImageView, zfbool, viewUIEnable)
{
    propertyValue = zffalse;
}
ZFPROPERTY_ON_INIT_DEFINE(ZFUIImageView, zfbool, viewUIEnableTree)
{
    propertyValue = zffalse;
}

ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIImageView, ZFUIImage *, image)
{
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIImageViewListenerHolder) *listenerHolder = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIImageViewListenerHolder);
    if(this->image() != zfnull)
    {
        this->image()->observerAdd(ZFObject::EventObjectPropertyValueOnUpdate(), listenerHolder->imageNinePatchChangedListener, this->objectHolder());
    }

    ZFPROTOCOL_ACCESS(ZFUIImageView)->image(this,
        this->image() && this->image()->nativeImage()
            ? this->image()
            : zfnull);
    ZFUIImage *image = this->image();
    if(image != zfnull)
    {
        ZFPROTOCOL_ACCESS(ZFUIImageView)->imageNinePatchChanged(
            this,
            image->imageScaleFixed(),
            ZFUIMarginApplyScale(image->imageNinePatch(), image->imageScaleFixed()));
    }

    if(((propertyValueOld != zfnull) ? propertyValueOld.to<ZFUIImage *>()->imageSize() : ZFUISizeZero())
        != ((this->image() != zfnull) ? this->image()->imageSize() : ZFUISizeZero()))
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_ON_DETACH_DEFINE(ZFUIImageView, ZFUIImage *, image)
{
    ZF_GLOBAL_INITIALIZER_CLASS(ZFUIImageViewListenerHolder) *listenerHolder = ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIImageViewListenerHolder);
    if(this->image() != zfnull)
    {
        this->image()->observerRemove(ZFObject::EventObjectPropertyValueOnUpdate(), listenerHolder->imageNinePatchChangedListener);
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIImageView, ZFUIContentScaleTypeEnum, imageScaleType)
{
    if(propertyValue != propertyValueOld)
    {
        this->layoutRequest();
    }
}
ZFPROPERTY_ON_ATTACH_DEFINE(ZFUIImageView, ZFUIMargin, imageMargin)
{
    if(propertyValue != propertyValueOld)
    {
        this->nativeImplViewMarginUpdate();
    }
}

ZFMETHOD_DEFINE_1(ZFUIImageView, void, measureImageView,
                  ZFMP_OUT(ZFUISize &, ret))
{
    ret = ((this->image() != zfnull) ? this->image()->imageSize() : ZFUISizeZero());
    ZFUISizeApplyMarginReversely(ret, ret, this->imageMargin());
}

ZFOBJECT_ON_INIT_DEFINE_1(ZFUIImageView,
                          ZFMP_IN(ZFUIImage *, image))
{
    this->objectOnInit();
    this->image(image);
}
void ZFUIImageView::objectOnInit(void)
{
    zfsuper::objectOnInit();

    zfCoreAssert(this->nativeImplView() == zfnull);
    zfclassNotPOD _ZFP_ZFUIImageView_nativeImplViewDestroy
    {
    public:
        static void action(ZF_IN ZFUIView *view,
                           ZF_IN void *nativeImplView)
        {
            ZFPROTOCOL_ACCESS(ZFUIImageView)->nativeImageViewDestroy(view->to<ZFUIImageView *>(), nativeImplView);
        }
    };
    this->nativeImplView(
        ZFPROTOCOL_ACCESS(ZFUIImageView)->nativeImageViewCreate(this),
        _ZFP_ZFUIImageView_nativeImplViewDestroy::action);
}
void ZFUIImageView::objectOnDealloc(void)
{
    this->image(zfnull);
    zfsuper::objectOnDealloc();
}

void ZFUIImageView::objectInfoOnAppend(ZF_IN_OUT zfstring &ret)
{
    zfsuper::objectInfoOnAppend(ret);
    if(this->image() != zfnull)
    {
        ret += " ";
        this->image()->objectInfoT(ret);
    }
}

void ZFUIImageView::nativeImplViewMarginImplUpdate(ZF_IN_OUT ZFUIMargin &nativeImplViewMargin)
{
    zfsuper::nativeImplViewMarginImplUpdate(nativeImplViewMargin);
    ZFUIMarginInc(nativeImplViewMargin, nativeImplViewMargin, this->imageMargin());
}
void ZFUIImageView::nativeImplViewOnLayout(ZF_OUT ZFUIRect &ret,
                                           ZF_IN const ZFUIRect &bounds,
                                           ZF_IN const ZFUIMargin &nativeImplViewMargin)
{
    ZFUIContentScaleTypeApply(
        ret,
        this->imageScaleType(),
        ZFUIRectApplyMargin(bounds, nativeImplViewMargin),
        this->image() ? this->image()->imageSize() : ZFUISizeZero());
}
void ZFUIImageView::layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                    ZF_IN const ZFUISize &sizeHint,
                                    ZF_IN const ZFUISizeParam &sizeParam)
{
    this->measureImageView(ret);
}

ZF_NAMESPACE_GLOBAL_END

