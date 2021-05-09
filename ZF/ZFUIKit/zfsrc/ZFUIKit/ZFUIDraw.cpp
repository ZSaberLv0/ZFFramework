#include "ZFUIDraw.h"
#include "protocol/ZFProtocolZFUIDraw.h"

ZF_NAMESPACE_GLOBAL_BEGIN

ZFOBJECT_REGISTER(ZFUIDrawableView)
ZFOBSERVER_EVENT_REGISTER(ZFUIDrawableView, OnDraw)

ZFMETHOD_DEFINE_0(ZFUIDrawableView, void, drawRequest)
{
    ZFPROTOCOL_ACCESS(ZFUIDrawForView)->drawRequest(this);
}

ZFMETHOD_DEFINE_0(ZFUIDrawableView, zfbool const &, drawing)
{
    return this->_drawing;
}

void ZFUIDrawableView::objectOnInit(void)
{
    zfsuper::objectOnInit();
    this->_drawing = zffalse;

    zfclassNotPOD _ZFP_ZFUIDrawableView_nativeImplViewDestroy
    {
    public:
        static void action(ZF_IN ZFUIView *view,
                           ZF_IN void *nativeImplView)
        {
            ZFPROTOCOL_ACCESS(ZFUIDrawForView)->nativeDrawableViewDestroy(view->to<ZFUIDrawableView *>(), nativeImplView);
        }
    };
    this->nativeImplView(
        ZFPROTOCOL_ACCESS(ZFUIDrawForView)->nativeDrawableViewCreate(this),
        _ZFP_ZFUIDrawableView_nativeImplViewDestroy::action);
}

ZF_NAMESPACE_BEGIN(ZFUIDraw)

// ============================================================
// context
ZFMETHOD_FUNC_DEFINE_1(void *, beginForView,
                       ZFMP_IN(ZFUIDrawableView *, view))
{
    if(view == zfnull)
    {
        return zfnull;
    }
    ZFUIDrawToken *token = zfpoolNew(ZFUIDrawToken,
        ZFUIDrawToken::TypeView,
        view,
        ZFUISizeMake(
            ZFUISizeApplyScale(view->viewFrame().width, view->UIScaleFixed()),
            ZFUISizeApplyScale(view->viewFrame().height, view->UIScaleFixed()))
        );
    if(!ZFPROTOCOL_ACCESS(ZFUIDrawForView)->beginForView(*token))
    {
        zfpoolDelete(token);
        return zfnull;
    }
    return token;
}
ZFMETHOD_FUNC_DEFINE_1(void, endForView,
                       ZFMP_IN(void *, context))
{
    ZFUIDrawToken *token = (ZFUIDrawToken *)context;
    zfCoreAssertWithMessage(token->type == ZFUIDrawToken::TypeView,
        "ZFUIDraw::endForView called with a mismatch context");
    ZFPROTOCOL_ACCESS(ZFUIDrawForView)->endForView(*token);
    zfpoolDelete(token);
}

ZFMETHOD_FUNC_DEFINE_1(void *, beginForImage,
                       ZFMP_IN(const ZFUISize &, imageSizePixel))
{
    if(imageSizePixel.width <= 0 || imageSizePixel.height <= 0)
    {
        return zfnull;
    }
    ZFUIDrawToken *token = zfpoolNew(ZFUIDrawToken,
        ZFUIDrawToken::TypeImage,
        ZFUIImage::ClassData()->newInstance(),
        imageSizePixel);
    if(!ZFPROTOCOL_ACCESS(ZFUIDrawForImage)->beginForImage(*token, imageSizePixel))
    {
        zfpoolDelete(token);
        return zfnull;
    }
    return token;
}
ZFMETHOD_FUNC_DEFINE_1(zfautoObjectT<ZFUIImage *>, endForImage,
                       ZFMP_IN(void *, context))
{
    ZFUIDrawToken *token = (ZFUIDrawToken *)context;
    zfCoreAssertWithMessage(token->type == ZFUIDrawToken::TypeImage,
        "ZFUIDraw::endForImage called with a mismatch context");
    void *nativeImage = ZFPROTOCOL_ACCESS(ZFUIDrawForImage)->endForImage(*token);
    zfautoObjectT<ZFUIImage *> ret = token->target;
    ret->nativeImage(nativeImage, zffalse);
    zfpoolDelete(token);
    if(nativeImage == zfnull)
    {
        return zfnull;
    }
    else
    {
        return ret;
    }
}

// ============================================================
// settings
ZFMETHOD_FUNC_DEFINE_1(zfbool const &, antialiasing,
                       ZFMP_IN(void *, context))
{
    return ((ZFUIDrawToken *)context)->antialiasing;
}
ZFMETHOD_FUNC_DEFINE_2(void, antialiasing,
                       ZFMP_IN(void *, context),
                       ZFMP_IN(zfbool const &, value))
{
    ZFPROTOCOL_ACCESS(ZFUIDraw)->antialiasing(*(ZFUIDrawToken *)context, value);
}

// ============================================================
// draw api
ZFMETHOD_FUNC_DEFINE_2(void, drawClear,
                       ZFMP_IN(void *, context),
                       ZFMP_IN_OPT(const ZFUIRect &, targetFramePixel, ZFUIRectZero()))
{
    ZFPROTOCOL_ACCESS(ZFUIDraw)->drawClear(*(ZFUIDrawToken *)context,
        targetFramePixel == ZFUIRectZero()
            ? ZFUIRectMake(ZFUIPointZero(), ((ZFUIDrawToken *)context)->targetSizePixel)
            : targetFramePixel
        );
}

ZFMETHOD_FUNC_DEFINE_3(void, drawColor,
                       ZFMP_IN(void *, context),
                       ZFMP_IN(const ZFUIColor &, color),
                       ZFMP_IN_OPT(const ZFUIRect &, targetFramePixel, ZFUIRectZero()))
{
    ZFPROTOCOL_ACCESS(ZFUIDraw)->drawColor(*(ZFUIDrawToken *)context, color,
        targetFramePixel == ZFUIRectZero()
            ? ZFUIRectMake(ZFUIPointZero(), ((ZFUIDrawToken *)context)->targetSizePixel)
            : targetFramePixel
        );
}

ZFMETHOD_FUNC_DEFINE_4(void, drawImage,
                       ZFMP_IN(void *, context),
                       ZFMP_IN(ZFUIImage *, image),
                       ZFMP_IN_OPT(const ZFUIRect &, imageFramePixel, ZFUIRectZero()),
                       ZFMP_IN_OPT(const ZFUIRect &, targetFramePixel, ZFUIRectZero()))
{
    if(image != zfnull && image->nativeImage() != zfnull)
    {
        ZFPROTOCOL_ACCESS(ZFUIDraw)->drawImage(*(ZFUIDrawToken *)context, image,
        imageFramePixel == ZFUIRectZero()
            ? ZFUIRectMake(ZFUIPointZero(), image->imageSizeFixed())
            : imageFramePixel,
        targetFramePixel == ZFUIRectZero()
            ? ZFUIRectMake(ZFUIPointZero(), ((ZFUIDrawToken *)context)->targetSizePixel)
            : targetFramePixel
        );
    }
}

ZF_NAMESPACE_END_WITH_REGISTER(ZFUIDraw, ZF_NAMESPACE_GLOBAL)

ZF_NAMESPACE_GLOBAL_END

