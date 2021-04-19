#include "ZFImpl_sys_iOS_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIImageIO.h"

#if ZF_ENV_sys_iOS
#import <UIKit/UIKit.h>

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIImageIOImpl_sys_iOS, ZFUIImageIO, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("iOS:UIImage")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIImage, "iOS:UIImage")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()

public:
    virtual void *imageApplyScale(ZF_IN zffloat imageScale,
                                  ZF_IN void *nativeImage,
                                  ZF_IN const ZFUISize &newSize,
                                  ZF_IN const ZFUIMargin &ninePatch)
    {
        UIImage *ret = this->_scaleUIImage(
            imageScale,
            (__bridge UIImage *)nativeImage,
            newSize,
            ninePatch);
        return (__bridge_retained void *)ret;
    }
    virtual void *imageLoadInFrame(ZF_IN zffloat imageScale,
                                   ZF_IN void *nativeImage,
                                   ZF_IN const ZFUIRect &frameInImage)
    {
        UIImage *src = (__bridge UIImage *)nativeImage;
        UIImage *ret = [UIImage imageWithCGImage:CGImageCreateWithImageInRect(
            src.CGImage,
            ZFImpl_sys_iOS_ZFUIRectToCGRect(frameInImage))];
        return (__bridge_retained void *)ret;
    }
    virtual void *imageLoadFromColor(ZF_IN zffloat imageScale,
                                     ZF_IN const ZFUIColor &color,
                                     ZF_IN const ZFUISize &size)
    {
        CGSize tmpSize = ZFImpl_sys_iOS_ZFUISizeToCGSize(size);
        CGRect rect = CGRectMake(0.0f, 0.0f, tmpSize.width, tmpSize.height);
        UIGraphicsBeginImageContext(rect.size);
        CGContextRef context = UIGraphicsGetCurrentContext();
        CGContextSetFillColorWithColor(context, [ZFImpl_sys_iOS_ZFUIColorToUIColor(color) CGColor]);
        CGContextFillRect(context, rect);
        UIImage *ret = UIGraphicsGetImageFromCurrentImageContext();
        UIGraphicsEndImageContext();

        ret = [UIImage imageWithCGImage:ret.CGImage scale:imageScale orientation:UIImageOrientationUp];
        return (__bridge_retained void *)ret;
    }

private:
    UIImage *_scaleUIImage(ZF_IN zffloat imageScale,
                           ZF_IN UIImage *image,
                           ZF_IN const ZFUISize &scaleToSize,
                           ZF_IN const ZFUIMargin &scaleUseNinePatch)
    {
        if(image.scale == 0)
        {
            return nil;
        }
        ZFUISize srcSize = ZFUISizeMake(image.size.width * image.scale, image.size.height * image.scale);
        zfbool sizeChanged = !ZFUISizeIsEqual(scaleToSize, srcSize);
        if([image respondsToSelector:@selector(resizableImageWithCapInsets:resizingMode:)])
        {
            if(sizeChanged && !ZFUIMarginIsEqual(scaleUseNinePatch, ZFUIMarginZero()))
            {
                UIEdgeInsets edgeInsets = UIEdgeInsetsZero;
                if(scaleUseNinePatch.left + scaleUseNinePatch.right < srcSize.width)
                {
                    edgeInsets.left = scaleUseNinePatch.left / image.scale;
                    edgeInsets.right = scaleUseNinePatch.right / image.scale;
                }
                if(scaleUseNinePatch.top + scaleUseNinePatch.bottom < srcSize.height)
                {
                    edgeInsets.top = scaleUseNinePatch.top / image.scale;
                    edgeInsets.bottom = scaleUseNinePatch.bottom / image.scale;
                }
                image = [image resizableImageWithCapInsets:edgeInsets resizingMode:UIImageResizingModeStretch];
            }

            if(sizeChanged)
            {
                return this->_drawScaleUIImage(imageScale, image, scaleToSize);
            }
            else
            {
                return image;
            }
        }
        else
        {
            if(ZFUIMarginIsEqual(scaleUseNinePatch, ZFUIMarginZero())
               || (scaleUseNinePatch.left + scaleUseNinePatch.right > srcSize.width
                   && scaleUseNinePatch.top + scaleUseNinePatch.bottom > srcSize.height))
            {
                if(sizeChanged)
                {
                    return this->_drawScaleUIImage(imageScale, image, scaleToSize);
                }
                else
                {
                    return image;
                }
            }
            else
            {
                return this->_drawNinePatchUIImage(imageScale, image, scaleToSize, scaleUseNinePatch);
            }
        }
    }
    UIImage *_drawScaleUIImage(ZF_IN zffloat imageScale,
                               ZF_IN UIImage *image,
                               ZF_IN const ZFUISize &scaleToSize)
    {
        CGSize tmpSize = CGSizeMake((scaleToSize.width == 0) ? image.size.width * image.scale : (CGFloat)scaleToSize.width,
                                    (scaleToSize.height == 0) ? image.size.height * image.scale : (CGFloat)scaleToSize.height);
        UIGraphicsBeginImageContext(tmpSize);
        [image drawInRect:CGRectMake(0, 0, tmpSize.width, tmpSize.height)];
        UIImage *imageTmp = UIGraphicsGetImageFromCurrentImageContext();
        UIGraphicsEndImageContext();
        return [UIImage imageWithCGImage:imageTmp.CGImage scale:imageScale orientation:UIImageOrientationUp];
    }
    UIImage *_drawNinePatchUIImage(ZF_IN zffloat imageScale,
                                   ZF_IN UIImage *image,
                                   ZF_IN const ZFUISize &scaleToSize,
                                   ZF_IN const ZFUIMargin &scaleUseNinePatch)
    {
        ZFUIImageImplNinePatchDrawData drawDatas[9];
        zfmemset(drawDatas, 0, sizeof(drawDatas));
        zfindex drawDatasCount = ZFUIImageImplNinePatchCalc(
            drawDatas,
            ZFUISizeApplyScale(ZFImpl_sys_iOS_ZFUISizeFromCGSize(image.size), image.scale),
            scaleUseNinePatch,
            scaleToSize);

        CGSize tmpSize = ZFImpl_sys_iOS_ZFUISizeToCGSize(scaleToSize);
        UIGraphicsBeginImageContext(tmpSize);
        CGContextRef ctx = UIGraphicsGetCurrentContext();

        CGImageRef src = image.CGImage;
        CGImageRef tmp = nil;
        for(zfindex i = 0; i < drawDatasCount; ++i)
        {
            const ZFUIImageImplNinePatchDrawData &drawData = drawDatas[i];

            tmp = CGImageCreateWithImageInRect(src, ZFImpl_sys_iOS_ZFUIRectToCGRect(drawData.src));
            this->_drawCGImage(imageScale, ctx, ZFImpl_sys_iOS_ZFUIRectToCGRect(drawData.dst), tmp);
            CGImageRelease(tmp);
        }

        UIImage *ret = UIGraphicsGetImageFromCurrentImageContext();
        UIGraphicsEndImageContext();
        return [UIImage imageWithCGImage:ret.CGImage scale:imageScale orientation:UIImageOrientationUp];
    }
    void _drawCGImage(ZF_IN zffloat imageScale,
                      CGContextRef ctx,
                      const CGRect &rect,
                      CGImageRef imageRef)
    {
        CGContextSaveGState(ctx);
        CGContextScaleCTM(ctx, 1, -1);
        CGContextTranslateCTM(ctx, 0, (-2 * rect.origin.y - rect.size.height));
        CGContextDrawImage(ctx, rect, imageRef);
        CGContextRestoreGState(ctx);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIImageIOImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIImageIOImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

