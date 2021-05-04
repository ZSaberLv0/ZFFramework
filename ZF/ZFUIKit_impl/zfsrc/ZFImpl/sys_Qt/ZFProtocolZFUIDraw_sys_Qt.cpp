#include "ZFImpl_sys_Qt_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIDraw.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIImage.h"

#if ZF_ENV_sys_Qt

#include <QGraphicsWidget>
#include <QPainter>

class _ZFP_ZFUIDrawableViewImpl_sys_Qt : public QGraphicsWidget
{
    Q_OBJECT

public:
    ZFUIDrawableView *_ZFP_owner;
    QPainter *_ZFP_painter;

public:
    _ZFP_ZFUIDrawableViewImpl_sys_Qt(ZF_IN ZFUIDrawableView *owner)
    : QGraphicsWidget()
    , _ZFP_owner(owner)
    , _ZFP_painter(NULL)
    {
    }

public:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr)
    {
        QGraphicsWidget::paint(painter, option, widget);
        if(this->_ZFP_owner != zfnull)
        {
            _ZFP_painter = painter;
            ZFPROTOCOL_ACCESS(ZFUIDrawForView)->notifyOnDraw(this->_ZFP_owner);
            _ZFP_painter = NULL;
        }
    }
 };

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIDrawForView
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIDrawForViewImpl_sys_Qt, ZFUIDrawForView, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Qt:QGraphicsWidget")

public:
    virtual void *nativeDrawableViewCreate(ZF_IN ZFUIDrawableView *drawableView)
    {
        return new _ZFP_ZFUIDrawableViewImpl_sys_Qt(drawableView);
    }
    virtual void nativeDrawableViewDestroy(ZF_IN ZFUIDrawableView *drawableView,
                                           ZF_IN void *nativeDrawableView)
    {
        delete ZFCastStatic(_ZFP_ZFUIDrawableViewImpl_sys_Qt *, nativeDrawableView);
    }

    virtual void drawRequest(ZF_IN ZFUIDrawableView *drawableView)
    {
        _ZFP_ZFUIDrawableViewImpl_sys_Qt *nativeDrawableView = (_ZFP_ZFUIDrawableViewImpl_sys_Qt *)drawableView->nativeImplView();
        nativeDrawableView->update();
    }

public:
    virtual zfbool beginForView(ZF_IN_OUT ZFUIDrawToken &token)
    {
        ZFUIDrawableView *drawableView = token.target;
        _ZFP_ZFUIDrawableViewImpl_sys_Qt *nativeDrawableView = (_ZFP_ZFUIDrawableViewImpl_sys_Qt *)drawableView->nativeImplView();
        token.impl = (void *)nativeDrawableView->_ZFP_painter;
        return zftrue;
    }
    virtual void endForView(ZF_IN_OUT ZFUIDrawToken &token)
    {
        // nothing to do
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIDrawForViewImpl_sys_Qt)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIDrawForViewImpl_sys_Qt)

// ============================================================
// ZFUIDrawForImage
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIDrawForImageImpl_sys_Qt, ZFUIDrawForImage, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Qt:QImage")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIImage, "Qt:QImage")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()

public:
    virtual zfbool beginForImage(ZF_IN_OUT ZFUIDrawToken &token,
                                 ZF_IN const ZFUISize &imageSizePixel)
    {
        QImage *image = new QImage(imageSizePixel.width, imageSizePixel.height, QImage::Format_ARGB32);
        QPainter *painter = new QPainter(image);
        token.impl = (void *)painter;
        painter->setPen(QColorConstants::Transparent);
        painter->setBrush(QColorConstants::Transparent);
        QPainter::CompositionMode oldMode = painter->compositionMode();
        painter->setCompositionMode(QPainter::CompositionMode_Clear);
        painter->drawRect(0, 0, imageSizePixel.width, imageSizePixel.height);
        painter->setCompositionMode(oldMode);
        return zftrue;
    }
    virtual void *endForImage(ZF_IN_OUT ZFUIDrawToken &token)
    {
        QPainter *painter = (QPainter *)token.impl;
        QImage *image = (QImage *)painter->device();
        delete painter;
        return image;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIDrawForImageImpl_sys_Qt)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIDrawForImageImpl_sys_Qt)

// ============================================================
// ZFUIDraw
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIDrawImpl_sys_Qt, ZFUIDraw, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Qt:QPainter")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIDrawForView, "Qt:QGraphicsWidget")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIDrawForImage, "Qt:QImage")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()

public:
    virtual void antialiasing(ZF_IN ZFUIDrawToken &token, ZF_IN zfbool antialiasing)
    {
        QPainter *painter = (QPainter *)token.impl;
        painter->setRenderHint(QPainter::Antialiasing, antialiasing);
    }

public:
    virtual void drawClear(ZF_IN_OUT ZFUIDrawToken &token,
                           ZF_IN const ZFUIRect &targetFramePixel)
    {
        QPainter *painter = (QPainter *)token.impl;
        painter->setPen(QColorConstants::Transparent);
        painter->setBrush(QColorConstants::Transparent);
        QPainter::CompositionMode oldMode = painter->compositionMode();
        painter->setCompositionMode(QPainter::CompositionMode_Clear);
        painter->drawRect(targetFramePixel.x, targetFramePixel.y ,targetFramePixel.width, targetFramePixel.height);
        painter->setCompositionMode(oldMode);
    }
    virtual void drawColor(ZF_IN_OUT ZFUIDrawToken &token,
                           ZF_IN const ZFUIColor &color,
                           ZF_IN const ZFUIRect &targetFramePixel)
    {
        QPainter *painter = (QPainter *)token.impl;
        QColor nativeColor = ZFImpl_sys_Qt_ZFUIColorToQColor(color);
        painter->setPen(nativeColor);
        painter->setBrush(nativeColor);
        QPainter::CompositionMode oldMode = painter->compositionMode();
        painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
        painter->drawRect(targetFramePixel.x, targetFramePixel.y ,targetFramePixel.width, targetFramePixel.height);
        painter->setCompositionMode(oldMode);
    }
    virtual void drawImage(ZF_IN_OUT ZFUIDrawToken &token,
                           ZF_IN ZFUIImage *image,
                           ZF_IN const ZFUIRect &imageFramePixel,
                           ZF_IN const ZFUIRect &targetFramePixel)
    {
        QPainter *painter = (QPainter *)token.impl;
        QPainter::CompositionMode oldMode = painter->compositionMode();
        painter->setCompositionMode(QPainter::CompositionMode_SourceOver);
        painter->drawImage(
                ZFImpl_sys_Qt_ZFUIRectToQRect(targetFramePixel),
                *(QImage *)image->nativeImage(),
                ZFImpl_sys_Qt_ZFUIRectToQRect(imageFramePixel)
            );
        painter->setCompositionMode(oldMode);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIDrawImpl_sys_Qt)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIDrawImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

#include "ZFProtocolZFUIDraw_sys_Qt.moc"
#endif // #if ZF_ENV_sys_Qt

