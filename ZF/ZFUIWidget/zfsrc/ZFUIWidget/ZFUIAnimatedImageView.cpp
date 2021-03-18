#include "ZFUIAnimatedImageView.h"

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
ZFSTYLE_DEFAULT_DEFINE(ZFUIAnimatedImageView)

// ============================================================
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIAnimatedImageViewDataHolder, ZFLevelZFFrameworkEssential)
{
    this->animatedImageOnUpdateListener = ZFCallbackForFunc(zfself::animatedImageOnUpdate);
}
public:
    ZFListener animatedImageOnUpdateListener;
    static ZFLISTENER_PROTOTYPE_EXPAND(animatedImageOnUpdate)
    {
        ZFUIAnimatedImageView *view = userData->objectHolded();
        if(view->animatedImageWrapToImage())
        {
            view->image(view->animatedImage()->aniFrameImageCurrent());
        }
        else
        {
            view->image(view->animatedImage()->aniFrameImageCurrent());
        }
    }
ZF_GLOBAL_INITIALIZER_END(ZFUIAnimatedImageViewDataHolder)

// ============================================================
// ZFUIAnimatedImageView
ZFOBJECT_REGISTER(ZFUIAnimatedImageView)

ZFPROPERTY_OVERRIDE_ON_ATTACH_DEFINE(ZFUIAnimatedImageView, zfbool, animatedImageWrapToImage)
{
    if(this->animatedImageWrapToImage() != propertyValueOld)
    {
        this->layoutRequest();
    }
}

void ZFUIAnimatedImageView::objectOnInitFinish(void)
{
    zfsuper::objectOnInitFinish();
    this->animatedImage()->observerAdd(ZFUIAnimatedImage::EventAniFrameOnUpdate(),
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIAnimatedImageViewDataHolder)->animatedImageOnUpdateListener,
        this->objectHolder());
}
void ZFUIAnimatedImageView::objectOnDeallocPrepare(void)
{
    this->animatedImage()->observerRemove(ZFUIAnimatedImage::EventAniFrameOnUpdate(),
        ZF_GLOBAL_INITIALIZER_INSTANCE(ZFUIAnimatedImageViewDataHolder)->animatedImageOnUpdateListener);
    this->aniStop();
    zfsuper::objectOnDeallocPrepare();
}

void ZFUIAnimatedImageView::layoutOnLayoutPrepare(ZF_IN const ZFUIRect &bounds)
{
    zfsuper::layoutOnLayoutPrepare(bounds);
    if(this->image() == zfnull && this->animatedImage()->aniFrameCount() > 0)
    {
        this->animatedImage()->aniManualNext();
    }
}
void ZFUIAnimatedImageView::layoutOnMeasure(ZF_OUT ZFUISize &ret,
                                            ZF_IN const ZFUISize &sizeHint,
                                            ZF_IN const ZFUISizeParam &sizeParam)
{
    if(this->animatedImageWrapToImage())
    {
        zfsuper::layoutOnMeasure(ret, sizeHint, sizeParam);
    }
    else
    {
        ret = ZFUISizeZero();
        ZFUIAnimatedImage *animatedImage = this->animatedImage();
        for(zfindex i = animatedImage->aniFrameCount() - 1; i != zfindexMax(); --i)
        {
            ZFUIImage *aniFrame = animatedImage->aniFrameImageAtIndex(i);
            ret.width = zfmMax(ret.width, aniFrame->imageSize().width);
            ret.height = zfmMax(ret.height, aniFrame->imageSize().height);
        }
    }
}

ZF_NAMESPACE_GLOBAL_END

