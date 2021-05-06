#include "ZFImpl_sys_iOS_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIViewTransform.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"

#if ZF_ENV_sys_iOS

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIViewTransformImpl_sys_iOS, ZFUIViewTransform, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("iOS:UIView")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "iOS:UIView")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()
public:
    virtual void viewTransform(ZF_IN ZFUIView *view)
    {
        // transform won't work when changed immediately after UIView created,
        // delay for some time
        zfautoObject holder = view->objectHolder();
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
            ZFUIView *viewTmp = holder->objectHolded();
            if(viewTmp == zfnull)
            {
                return;
            }
            UIView *nativeView = (__bridge UIView *)viewTmp->nativeView();
            CATransform3D t = CATransform3DIdentity;
            if(viewTmp->viewRotation() != 0)
            {
                t = CATransform3DRotate(t, ((int)viewTmp->viewRotation()) * M_PI / 180, 0, 0, 1);
            }
            if(viewTmp->viewScaleX() != 1 || viewTmp->viewScaleY() != 1)
            {
                t = CATransform3DScale(t, viewTmp->viewScaleX(), viewTmp->viewScaleY(), 1);
            }
            nativeView.layer.transform = t;
        });
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIViewTransformImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIViewTransformImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

