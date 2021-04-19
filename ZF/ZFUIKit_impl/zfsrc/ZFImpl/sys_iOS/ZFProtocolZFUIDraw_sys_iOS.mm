#include "ZFImpl_sys_iOS_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIDraw.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIImage.h"

#if ZF_ENV_sys_iOS

@interface _ZFP_ZFUIDrawableViewImpl_sys_iOS : UIView
@property (nonatomic, assign) ZFUIDrawableView *_ZFP_owner;
@property (nonatomic, assign) ZFPROTOCOL_INTERFACE_CLASS(ZFUIDrawForView) *_ZFP_impl;
@end
@implementation _ZFP_ZFUIDrawableViewImpl_sys_iOS
- (void)drawRect:(CGRect)rect
{
    [super drawRect:rect];
    if(self._ZFP_owner != zfnull)
    {
        self._ZFP_impl->notifyOnDraw(self._ZFP_owner);
    }
}
@end

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// ZFUIDrawForView
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIDrawForViewImpl_sys_iOS, ZFUIDrawForView, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("iOS:UIView")

public:
    virtual void *nativeDrawableViewCreate(ZF_IN ZFUIDrawableView *drawableView)
    {
        _ZFP_ZFUIDrawableViewImpl_sys_iOS *nativeDrawableView = [_ZFP_ZFUIDrawableViewImpl_sys_iOS new];
        nativeDrawableView._ZFP_owner = drawableView;
        nativeDrawableView._ZFP_impl = this;
        nativeDrawableView.backgroundColor = [UIColor clearColor];
        return (__bridge_retained void *)nativeDrawableView;
    }
    virtual void nativeDrawableViewDestroy(ZF_IN ZFUIDrawableView *drawableView,
                                           ZF_IN void *nativeDrawableView)
    {
        _ZFP_ZFUIDrawableViewImpl_sys_iOS *nativeDrawableViewTmp = (__bridge_transfer _ZFP_ZFUIDrawableViewImpl_sys_iOS *)nativeDrawableView;
        nativeDrawableViewTmp._ZFP_owner = zfnull;
        nativeDrawableViewTmp = nil;
    }

    virtual void drawRequest(ZF_IN ZFUIDrawableView *drawableView)
    {
        _ZFP_ZFUIDrawableViewImpl_sys_iOS *nativeDrawableViewTmp = (__bridge_transfer _ZFP_ZFUIDrawableViewImpl_sys_iOS *)drawableView->nativeImplView();
        [nativeDrawableViewTmp setNeedsDisplay];
    }

public:
    virtual zfbool beginForView(ZF_IN_OUT ZFUIDrawToken &token)
    {
        token.impl = (void *)UIGraphicsGetCurrentContext();
        return (token.impl != zfnull);
    }
    virtual void endForView(ZF_IN_OUT ZFUIDrawToken &token)
    {
        // nothing to do
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIDrawForViewImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIDrawForViewImpl_sys_iOS)

// ============================================================
// ZFUIDrawForImage
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIDrawForImageImpl_sys_iOS, ZFUIDrawForImage, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("iOS:UIImage")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIImage, "iOS:UIImage")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()

public:
    virtual zfbool beginForImage(ZF_IN_OUT ZFUIDrawToken &token,
                                 ZF_IN const ZFUISize &imageSizePixel)
    {
        UIGraphicsBeginImageContext(ZFImpl_sys_iOS_ZFUISizeToCGSize(imageSizePixel));
        token.impl = (void *)UIGraphicsGetCurrentContext();
        return (token.impl != zfnull);
    }
    virtual void *endForImage(ZF_IN_OUT ZFUIDrawToken &token)
    {
        UIImage *nativeImage = UIGraphicsGetImageFromCurrentImageContext();
        UIGraphicsEndImageContext();
        return (__bridge_retained void *)nativeImage;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIDrawForImageImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIDrawForImageImpl_sys_iOS)

// ============================================================
// ZFUIDraw
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIDrawImpl_sys_iOS, ZFUIDraw, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("iOS:CGContextRef")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIDrawForView, "iOS:UIView")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIDrawForImage, "iOS:UIImage")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()

public:
    virtual void antialiasing(ZF_IN ZFUIDrawToken &token, ZF_IN zfbool antialiasing)
    {
        CGContextRef context = (CGContextRef)token.impl;
        CGContextSetAllowsAntialiasing(context, antialiasing);
    }

public:
    virtual void drawClear(ZF_IN_OUT ZFUIDrawToken &token,
                           ZF_IN const ZFUIRect &targetFramePixel)
    {
        CGContextRef context = (CGContextRef)token.impl;
        CGContextClearRect(context,
            ZFImpl_sys_iOS_ZFUIRectToCGRect(targetFramePixel));
    }
    virtual void drawColor(ZF_IN_OUT ZFUIDrawToken &token,
                           ZF_IN const ZFUIColor &color,
                           ZF_IN const ZFUIRect &targetFramePixel)
    {
        CGContextRef context = (CGContextRef)token.impl;
        CGContextSetFillColorWithColor(context, ZFImpl_sys_iOS_ZFUIColorToUIColor(color).CGColor);
        CGContextFillRect(context,
            ZFImpl_sys_iOS_ZFUIRectToCGRect(targetFramePixel));
    }
    virtual void drawImage(ZF_IN_OUT ZFUIDrawToken &token,
                           ZF_IN ZFUIImage *image,
                           ZF_IN const ZFUIRect &imageFramePixel,
                           ZF_IN const ZFUIRect &targetFramePixel)
    {
        CGContextRef context = (CGContextRef)token.impl;
        UIImage *srcImage = (__bridge UIImage *)image->nativeImage();
        CGImageRef subImage = CGImageCreateWithImageInRect(srcImage.CGImage, ZFImpl_sys_iOS_ZFUIRectToCGRect(imageFramePixel));
        CGContextDrawImage(context,
            ZFImpl_sys_iOS_ZFUIRectToCGRect(targetFramePixel),
            subImage);
        CGImageRelease(subImage);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIDrawImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIDrawImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

