#include "ZFImpl_sys_Qt_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIImageView.h"

#if ZF_ENV_sys_Qt

#include <QGraphicsWidget>
#include <QImage>
#include <QPainter>

class _ZFP_ZFUIImageViewImpl_sys_Qt_ImageView : public QGraphicsWidget
{
    Q_OBJECT

public:
    ZFUIImageView *_ZFP_owner;
    zffloat _ZFP_imageScale;
    ZFUIMargin _ZFP_imageNinePatchScaled;

public:
    _ZFP_ZFUIImageViewImpl_sys_Qt_ImageView(ZF_IN ZFUIImageView *owner)
    : QGraphicsWidget()
    , _ZFP_owner(owner)
    , _ZFP_imageScale(1)
    , _ZFP_imageNinePatchScaled(ZFUIMarginZero())
    {
    }

protected:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr)
    {
        QGraphicsWidget::paint(painter, option, widget);
        if(_ZFP_owner == zfnull || _ZFP_owner->image() == zfnull || _ZFP_owner->image()->nativeImage() == zfnull)
        {
            return ;
        }

        QImage *image = (QImage *)_ZFP_owner->image()->nativeImage();
        painter->setRenderHint(QPainter::SmoothPixmapTransform);
        QRectF bounds(QPointF(), this->geometry().size());
        if(_ZFP_imageNinePatchScaled == ZFUIMarginZero())
        {
            painter->drawImage(bounds, *image);
            return ;
        }

        ZFUIImageImplNinePatchDrawData drawDatas[9];
        zfmemset(drawDatas, 0, sizeof(drawDatas));
        zfindex drawDatasCount = ZFUIImageImplNinePatchCalc(
                drawDatas,
                ZFUISizeApplyScaleReversely(ZFImpl_sys_Qt_ZFUISizeFromQSize(image->size()), _ZFP_imageScale),
                _ZFP_imageNinePatchScaled,
                ZFImpl_sys_Qt_ZFUISizeFromQSizeF(bounds.size()));

        for(zfindex i = 0; i < drawDatasCount; ++i)
        {
            const ZFUIImageImplNinePatchDrawData &drawData = drawDatas[i];
            painter->drawImage(
                ZFImpl_sys_Qt_ZFUIRectToQRect(drawData.dst),
                *image,
                ZFImpl_sys_Qt_ZFUIRectToQRect(ZFUIRectApplyScale(drawData.src, _ZFP_imageScale)));
        }
    }
};

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIImageViewImpl_sys_Qt, ZFUIImageView, ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Qt:QGraphicsWidget")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIImage, "Qt:QImage")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()

public:
    virtual void *nativeImageViewCreate(ZF_IN ZFUIImageView *imageView)
    {
        return new _ZFP_ZFUIImageViewImpl_sys_Qt_ImageView(imageView);
    }
    virtual void nativeImageViewDestroy(ZF_IN ZFUIImageView *imageView,
                                        ZF_IN void *nativeImageView)
    {
        delete ZFCastStatic(_ZFP_ZFUIImageViewImpl_sys_Qt_ImageView *, nativeImageView);
    }

    virtual void image(ZF_IN ZFUIImageView *imageView,
                       ZF_IN ZFUIImage *image)
    {
        _ZFP_ZFUIImageViewImpl_sys_Qt_ImageView *nativeImplView = ZFCastStatic(_ZFP_ZFUIImageViewImpl_sys_Qt_ImageView *, imageView->nativeImplView());
        nativeImplView->update();
    }
    virtual void imageNinePatchChanged(ZF_IN ZFUIImageView *imageView,
                                       ZF_IN zffloat imageScale,
                                       ZF_IN const ZFUIMargin &imageNinePatch)
    {
        _ZFP_ZFUIImageViewImpl_sys_Qt_ImageView *nativeImplView = ZFCastStatic(_ZFP_ZFUIImageViewImpl_sys_Qt_ImageView *, imageView->nativeImplView());
        nativeImplView->_ZFP_imageScale = imageScale;
        nativeImplView->_ZFP_imageNinePatchScaled = ZFUIMarginApplyScaleReversely(imageNinePatch, imageScale);
        nativeImplView->update();
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIImageViewImpl_sys_Qt)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIImageViewImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

#include "ZFProtocolZFUIImageView_sys_Qt.moc"
#endif // #if ZF_ENV_sys_Qt

