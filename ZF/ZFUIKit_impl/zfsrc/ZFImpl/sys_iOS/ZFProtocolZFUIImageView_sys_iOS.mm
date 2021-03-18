#include "ZFImpl_sys_iOS_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIImageView.h"

#if ZF_ENV_sys_iOS

@interface _ZFP_ZFUIImageViewImpl_sys_iOS_ImageView : UIImageView
@property (nonatomic, assign) ZFUIImage *ownerZFUIImage;
- (void)updateImageWithNinePatch;
@end
@implementation _ZFP_ZFUIImageViewImpl_sys_iOS_ImageView
// ============================================================
// init and dealloc
- (id)init
{
    self = [super init];
    self.autoresizesSubviews = NO;
    self.contentMode = UIViewContentModeScaleToFill;
    return self;
}
// ============================================================
// nine patch logic
- (void)updateImageWithNinePatch
{
    if(self.ownerZFUIImage == zfnull)
    {
        self.image = nil;
    }
    else if(self.ownerZFUIImage->imageNinePatch() == ZFUIMarginZero())
    {
        UIImage *nativeImage = (__bridge UIImage *)self.ownerZFUIImage->nativeImage();
        self.image = [UIImage imageWithCGImage:nativeImage.CGImage scale:nativeImage.scale orientation:UIImageOrientationUp];
    }
    else
    {
        UIImage *nativeImage = (__bridge UIImage *)self.ownerZFUIImage->nativeImage();
        UIImage *tmpImage = [UIImage imageWithCGImage:nativeImage.CGImage scale:nativeImage.scale orientation:UIImageOrientationUp];
        UIEdgeInsets ninePatch = UIEdgeInsetsMake(
            self.ownerZFUIImage->imageNinePatch().top,
            self.ownerZFUIImage->imageNinePatch().left,
            self.ownerZFUIImage->imageNinePatch().bottom,
            self.ownerZFUIImage->imageNinePatch().right);
        if(ninePatch.left + ninePatch.right >= tmpImage.size.width)
        {
            if(ninePatch.left + ninePatch.right == tmpImage.size.width)
            {
                --(ninePatch.right);
            }
            else
            {
                ninePatch.left = 0;
                ninePatch.right = 0;
            }
        }
        if(ninePatch.top + ninePatch.bottom >= tmpImage.size.height)
        {
            if(ninePatch.top + ninePatch.bottom == tmpImage.size.height)
            {
                --(ninePatch.bottom);
            }
            else
            {
                ninePatch.left = 0;
                ninePatch.right = 0;
            }
        }
        self.image = [tmpImage resizableImageWithCapInsets:ninePatch resizingMode:UIImageResizingModeStretch];
    }
}
@end

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIImageViewImpl_sys_iOS, ZFUIImageView, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("iOS:UIImageView")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIImage, "iOS:UIImage")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()

public:
    virtual void *nativeImageViewCreate(ZF_IN ZFUIImageView *imageView)
    {
        return (__bridge_retained void *)[_ZFP_ZFUIImageViewImpl_sys_iOS_ImageView new];
    }
    virtual void nativeImageViewDestroy(ZF_IN ZFUIImageView *imageView,
                                        ZF_IN void *nativeImageView)
    {
        UIView *tmp = (__bridge_transfer UIView *)nativeImageView;
        tmp = nil;
    }

    virtual void image(ZF_IN ZFUIImageView *imageView,
                       ZF_IN ZFUIImage *image)
    {
        _ZFP_ZFUIImageViewImpl_sys_iOS_ImageView *nativeImplView = (__bridge _ZFP_ZFUIImageViewImpl_sys_iOS_ImageView *)imageView->nativeImplView();
        nativeImplView.ownerZFUIImage = image;
        [nativeImplView updateImageWithNinePatch];
    }
    virtual void imageNinePatchChanged(ZF_IN ZFUIImageView *imageView,
                                       ZF_IN zffloat imageScale,
                                       ZF_IN const ZFUIMargin &imageNinePatch)
    {
        _ZFP_ZFUIImageViewImpl_sys_iOS_ImageView *nativeImplView = (__bridge _ZFP_ZFUIImageViewImpl_sys_iOS_ImageView *)imageView->nativeImplView();
        [nativeImplView updateImageWithNinePatch];
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIImageViewImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIImageViewImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

