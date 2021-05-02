#include "ZFImpl_sys_iOS_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUISysWindowEmbedNativeView.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"

#if ZF_ENV_sys_iOS

@interface _ZFP_ZFUISysWindowEmbedNativeViewImpl_sys_iOS_Container : UIView
@end
@implementation _ZFP_ZFUISysWindowEmbedNativeViewImpl_sys_iOS_Container
- (CGSize)sizeThatFits:(CGSize)size
{
    return size;
}
- (void)layoutSubviews
{
    self.frame = self.superview.bounds;

    NSArray *children = self.subviews;
    for(UIView *child in children)
    {
        child.frame = self.bounds;
    }
}
@end

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUISysWindowEmbedNativeViewImpl_sys_iOS, ZFUISysWindowEmbedNativeView, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "iOS:UIView")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()

public:
    virtual void nativeViewAdd(ZF_IN void *parent, ZF_IN void *child)
    {
        UIView *nativeParent = (__bridge UIView *)parent;
        _ZFP_ZFUISysWindowEmbedNativeViewImpl_sys_iOS_Container *container = [_ZFP_ZFUISysWindowEmbedNativeViewImpl_sys_iOS_Container new];
        UIView *nativeChild = (__bridge UIView *)child;
        [nativeParent addSubview:container];
        [container addSubview:nativeChild];
    }
    virtual void nativeViewRemove(ZF_IN void *parent, ZF_IN void *child)
    {
        UIView *nativeChild = (__bridge UIView *)child;
        [nativeChild.superview removeFromSuperview];
        [nativeChild removeFromSuperview];
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUISysWindowEmbedNativeViewImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUISysWindowEmbedNativeViewImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

