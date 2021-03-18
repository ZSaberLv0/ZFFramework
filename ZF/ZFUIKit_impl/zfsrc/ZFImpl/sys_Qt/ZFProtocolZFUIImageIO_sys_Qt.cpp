#include "ZFImpl_sys_Qt_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIImageIO.h"

#if ZF_ENV_sys_Qt

#include <QImage>
#include <QPainter>

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIImageIOImpl_sys_Qt, ZFUIImageIO, ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Qt:QImage")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIImage, "Qt:QImage")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()

public:
    virtual void *imageApplyScale(ZF_IN zffloat imageScale,
                                  ZF_IN void *nativeImage,
                                  ZF_IN const ZFUISize &newSize,
                                  ZF_IN const ZFUIMargin &ninePatch)
    {
        QImage *nativeImageTmp = ZFCastStatic(QImage *, nativeImage);
        if(ninePatch == ZFUIMarginZero())
        {
            QImage *ret = new QImage();
            *ret = nativeImageTmp->scaled(
                newSize.width, newSize.height,
                Qt::IgnoreAspectRatio,
                Qt::SmoothTransformation);
            return ret;
        }
        return this->_scaleImage(nativeImageTmp, imageScale, newSize, ninePatch);
    }
    virtual void *imageLoadInFrame(ZF_IN zffloat imageScale,
                                   ZF_IN void *nativeImage,
                                   ZF_IN const ZFUIRect &frameInImage)
    {
        QImage *nativeImageTmp = ZFCastStatic(QImage *, nativeImage);
        QImage retTmp = nativeImageTmp->copy(frameInImage.x, frameInImage.y, frameInImage.width, frameInImage.height);
        return new QImage(retTmp);
    }
    virtual void *imageLoadFromColor(ZF_IN zffloat imageScale,
                                     ZF_IN const ZFUIColor &color,
                                     ZF_IN const ZFUISize &size)
    {
        QImage *ret = new QImage(size.width, size.height, QImage::Format_ARGB32);
        ret->fill(ZFImpl_sys_Qt_ZFUIKit_impl_ZFUIColorToQColor(color));
        return ret;
    }

private:
    QImage *_scaleImage(ZF_IN QImage *image,
                        ZF_IN zffloat imageScale,
                        ZF_IN const ZFUISize &scaleToSize,
                        ZF_IN const ZFUIMargin &scaleUseNinePatch)
    {
        ZFUIImageImplNinePatchDrawData drawDatas[9];
        zfmemset(drawDatas, 0, sizeof(drawDatas));
        zfindex drawDatasCount = ZFUIImageImplNinePatchCalc(
            drawDatas,
            ZFImpl_sys_Qt_ZFUIKit_impl_ZFUISizeFromQSize(image->size()),
            scaleUseNinePatch,
            scaleToSize);

        QImage *ret = new QImage(scaleToSize.width, scaleToSize.height, QImage::Format_ARGB32);
        QPainter painter(ret);

        for(zfindex i = 0; i < drawDatasCount; ++i)
        {
            const ZFUIImageImplNinePatchDrawData &drawData = drawDatas[i];
            painter.drawImage(ZFImpl_sys_Qt_ZFUIKit_impl_ZFUIRectToQRect(drawData.dst), *image, ZFImpl_sys_Qt_ZFUIKit_impl_ZFUIRectToQRect(drawData.src));
        }
        return ret;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIImageIOImpl_sys_Qt)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIImageIOImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_Qt

