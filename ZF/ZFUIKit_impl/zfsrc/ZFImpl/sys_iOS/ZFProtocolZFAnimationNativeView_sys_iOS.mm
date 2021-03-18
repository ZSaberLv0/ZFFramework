#include "ZFImpl_sys_iOS_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFAnimationNativeView.h"

#if ZF_ENV_sys_iOS

#import <QuartzCore/QuartzCore.h>

@class _ZFP_ZFAnimationNativeViewImpl_sys_iOS_AniDelegate;

@interface _ZFP_ZFAnimationNativeViewImpl_sys_iOS_Ani : CAAnimationGroup
@property (nonatomic, assign) ZFAnimationNativeView *ownerAni;
@property (nonatomic, assign) zffloat nativeAniScale;
- (void)nativeAniStart;
- (void)nativeAniStop;

@property (nonatomic, strong) _ZFP_ZFAnimationNativeViewImpl_sys_iOS_AniDelegate *_nativeAniDelegate;
@property (nonatomic, strong) NSString *_nativeAniKey;
- (void)_nativeAniOnStart:(zfidentity)aniId;
- (void)_nativeAniOnStop:(zfidentity)aniId finished:(BOOL)finished;
@end

@interface _ZFP_ZFAnimationNativeViewImpl_sys_iOS_AniDelegate : NSObject<CAAnimationDelegate>
@property (nonatomic, weak) _ZFP_ZFAnimationNativeViewImpl_sys_iOS_Ani *owner;
@property (nonatomic, assign) zfidentity aniId;
@end
@implementation _ZFP_ZFAnimationNativeViewImpl_sys_iOS_AniDelegate
- (void)animationDidStart:(CAAnimation *)anim
{
    [self.owner _nativeAniOnStart:self.aniId];
}
- (void)animationDidStop:(CAAnimation *)anim finished:(BOOL)flag
{
    [self.owner _nativeAniOnStop:self.aniId finished:flag];
}
@end

@implementation _ZFP_ZFAnimationNativeViewImpl_sys_iOS_Ani
- (instancetype)init
{
    self = [super init];
    if(self)
    {
        self.nativeAniScale = 1;

        self._nativeAniKey = [NSString stringWithFormat:@"_ZFP_%08X", (zfuint)[self hash]];

        self.removedOnCompletion = NO;
        self.fillMode = kCAFillModeForwards;
    }
    return self;
}
- (void)dealloc
{
    self._nativeAniDelegate = nil;
    self._nativeAniKey = nil;
}
- (void)nativeAniStart
{
    UIView *aniTarget = (__bridge UIView *)ZFCastZFObjectUnchecked(ZFUIView *, self.ownerAni->aniTarget())->nativeView();
    CAMediaTimingFunction *nativeCurve = nil;
    switch(self.ownerAni->aniCurve())
    {
        case ZFAnimationNativeViewCurve::e_Linear:
            nativeCurve = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionLinear];
            break;
        case ZFAnimationNativeViewCurve::e_EaseIn:
            nativeCurve = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseIn];
            break;
        case ZFAnimationNativeViewCurve::e_EaseOut:
            nativeCurve = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseOut];
            break;
        case ZFAnimationNativeViewCurve::e_EaseInOut:
            nativeCurve = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionEaseInEaseOut];
            break;
        default:
            nativeCurve = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionLinear];
            break;
    }
    NSTimeInterval nativeDuration = (NSTimeInterval)self.ownerAni->aniDurationFixed() / 1000;
    NSMutableArray *nativeAnimations = [NSMutableArray array];

    {
        CGFloat refWidth = aniTarget.frame.size.width;
        CGFloat refHeight = aniTarget.frame.size.height;
        CGFloat refZ = zfmMin(refWidth, refHeight);
        CATransform3D transformFrom = CATransform3DIdentity;
        CATransform3D transformTo = CATransform3DIdentity;

        if(self.ownerAni->aniRotateXFrom() != 0)
        {
            transformFrom = CATransform3DRotate(transformFrom, self.ownerAni->aniRotateXFrom() * M_PI / 360, 1, 0, 0);
        }
        if(self.ownerAni->aniRotateYFrom() != 0)
        {
            transformFrom = CATransform3DRotate(transformFrom, self.ownerAni->aniRotateYFrom() * M_PI / 360, 0, 1, 0);
        }
        if(self.ownerAni->aniRotateZFrom() != 0)
        {
            transformFrom = CATransform3DRotate(transformFrom, self.ownerAni->aniRotateZFrom() * M_PI / 360, 0, 0, 1);
        }
        if(self.ownerAni->aniRotateXTo() != 0)
        {
            transformTo = CATransform3DRotate(transformTo, self.ownerAni->aniRotateXTo() * M_PI / 360, 1, 0, 0);
        }
        if(self.ownerAni->aniRotateYTo() != 0)
        {
            transformTo = CATransform3DRotate(transformTo, self.ownerAni->aniRotateYTo() * M_PI / 360, 0, 1, 0);
        }
        if(self.ownerAni->aniRotateZTo() != 0)
        {
            transformTo = CATransform3DRotate(transformTo, self.ownerAni->aniRotateZTo() * M_PI / 360, 0, 0, 1);
        }

        if(self.ownerAni->aniScaleXFrom() != 1 || self.ownerAni->aniScaleYFrom() != 1)
        {
            transformFrom = CATransform3DScale(transformFrom, self.ownerAni->aniScaleXFrom(), self.ownerAni->aniScaleYFrom(), 1);
        }
        if(self.ownerAni->aniScaleXTo() != 1 || self.ownerAni->aniScaleYTo() != 1)
        {
            transformTo = CATransform3DScale(transformTo, self.ownerAni->aniScaleXTo(), self.ownerAni->aniScaleYTo(), 1);
        }

        transformFrom = CATransform3DTranslate(
            transformFrom,
            refWidth * self.ownerAni->aniTranslateXFrom() + self.ownerAni->aniTranslatePixelXFrom() * self.nativeAniScale,
            refHeight * self.ownerAni->aniTranslateYFrom() + self.ownerAni->aniTranslatePixelYFrom() * self.nativeAniScale,
            refZ * self.ownerAni->aniTranslateZFrom() + self.ownerAni->aniTranslatePixelZFrom() * self.nativeAniScale);
        transformTo = CATransform3DTranslate(
            transformTo,
            refWidth * self.ownerAni->aniTranslateXTo() + self.ownerAni->aniTranslatePixelXTo() * self.nativeAniScale,
            refHeight * self.ownerAni->aniTranslateYTo() + self.ownerAni->aniTranslatePixelYTo() * self.nativeAniScale,
            refZ * self.ownerAni->aniTranslateZTo() + self.ownerAni->aniTranslatePixelZTo() * self.nativeAniScale);

        CABasicAnimation *nativeTransformAni = [CABasicAnimation animationWithKeyPath:@"transform"];
        [nativeAnimations addObject:nativeTransformAni];
        nativeTransformAni.fromValue = [NSValue valueWithCATransform3D:transformFrom];
        nativeTransformAni.toValue = [NSValue valueWithCATransform3D:transformTo];
        nativeTransformAni.additive = YES;
    }

    if(self.ownerAni->aniAlphaFrom() != 1 || self.ownerAni->aniAlphaTo() != 1)
    {
        CABasicAnimation *nativeAlphaAni = [CABasicAnimation animationWithKeyPath:@"opacity"];
        [nativeAnimations addObject:nativeAlphaAni];
        nativeAlphaAni.fromValue = [NSNumber numberWithFloat:self.ownerAni->aniAlphaFrom()];
        nativeAlphaAni.toValue = [NSNumber numberWithFloat:self.ownerAni->aniAlphaTo()];
    }

    for(NSUInteger i = 0; i < [nativeAnimations count]; ++i)
    {
        CABasicAnimation *ani = (CABasicAnimation *)[nativeAnimations objectAtIndex:i];
        ani.duration = nativeDuration;
        ani.timingFunction = nativeCurve;
    }
    self.animations = nativeAnimations;
    self.duration = nativeDuration;
    self.timingFunction = nativeCurve;

    self._nativeAniDelegate = [_ZFP_ZFAnimationNativeViewImpl_sys_iOS_AniDelegate new];
    self._nativeAniDelegate.owner = self;
    self._nativeAniDelegate.aniId = self.ownerAni->aniId();
    self.delegate = self._nativeAniDelegate;
    [aniTarget.layer addAnimation:self forKey:self._nativeAniKey];
}
- (void)nativeAniStop
{
    UIView *aniTarget = (__bridge UIView *)ZFCastZFObjectUnchecked(ZFUIView *, self.ownerAni->aniTarget())->nativeView();
    [aniTarget.layer removeAnimationForKey:self._nativeAniKey];
    self._nativeAniDelegate.owner = nil;
    self._nativeAniDelegate = nil;
    self.delegate = nil;
}

- (void)_nativeAniOnStart:(zfidentity)aniId
{
}
- (void)_nativeAniOnStop:(zfidentity)aniId finished:(BOOL)finished
{
    if(aniId == self.ownerAni->aniId())
    {
        self.delegate = nil;
        UIView *aniTarget = (__bridge UIView *)ZFCastZFObjectUnchecked(ZFUIView *, self.ownerAni->aniTarget())->nativeView();
        [aniTarget.layer removeAnimationForKey:self._nativeAniKey];
        ZFPROTOCOL_ACCESS(ZFAnimationNativeView)->notifyAniStop(self.ownerAni);
    }
}
@end

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFAnimationNativeViewImpl_sys_iOS, ZFAnimationNativeView, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("iOS:CAAnimation")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "iOS:UIView")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    virtual void *nativeAniCreate(ZF_IN ZFAnimationNativeView *ani)
    {
        _ZFP_ZFAnimationNativeViewImpl_sys_iOS_Ani *nativeAni = [_ZFP_ZFAnimationNativeViewImpl_sys_iOS_Ani new];
        nativeAni.ownerAni = ani;
        return (__bridge_retained void *)nativeAni;
    }
    virtual void nativeAniDestroy(ZF_IN ZFAnimationNativeView *ani,
                                  ZF_IN void *nativeAni)
    {
        NSObject *tmp = (__bridge_transfer NSObject *)nativeAni;
        tmp = nil;
    }

    virtual void nativeAniStart(ZF_IN ZFAnimationNativeView *ani,
                                ZF_IN zffloat nativeAniScale)
    {
        _ZFP_ZFAnimationNativeViewImpl_sys_iOS_Ani *nativeAni = (__bridge _ZFP_ZFAnimationNativeViewImpl_sys_iOS_Ani *)ani->nativeAnimation();
        nativeAni.nativeAniScale = nativeAniScale;
        [nativeAni nativeAniStart];
    }
    virtual void nativeAniStop(ZF_IN ZFAnimationNativeView *ani)
    {
        _ZFP_ZFAnimationNativeViewImpl_sys_iOS_Ani *nativeAni = (__bridge _ZFP_ZFAnimationNativeViewImpl_sys_iOS_Ani *)ani->nativeAnimation();
        [nativeAni nativeAniStop];
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFAnimationNativeViewImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFAnimationNativeViewImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

