#include "ZFImpl_sys_iOS_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIOnScreenKeyboardState.h"

#if ZF_ENV_sys_iOS

@interface _ZFP_ZFUIOnScreenKeyboardStateImpl_sys_iOS : NSObject
@property (nonatomic, assign) zfbool keyboardShowing;
@property (nonatomic, assign) ZFUIRect keyboardFrame;
- (void)_keyboardOnChange:(NSNotification *)aNotification;
@end
static _ZFP_ZFUIOnScreenKeyboardStateImpl_sys_iOS *_ZFP_ZFUIOnScreenKeyboardStateImpl_sys_iOS_instance = nil;
@implementation _ZFP_ZFUIOnScreenKeyboardStateImpl_sys_iOS
- (instancetype)init
{
    self = [super init];
    if(self)
    {
        self.keyboardShowing = zffalse;
        self.keyboardFrame = ZFUIRectZero();

        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(_keyboardOnChange:) name:UIKeyboardWillChangeFrameNotification object:nil];
    }
    return self;
}
- (void)dealloc
{
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}
- (void)_keyboardOnChange:(NSNotification *)aNotification
{
    CGRect rect = [[aNotification.userInfo objectForKey:UIKeyboardFrameEndUserInfoKey] CGRectValue];
    ZFUIRect keyboardFrameNew = ZFImpl_sys_iOS_ZFUIKit_impl_ZFUIRectFromCGRect(rect);

    self.keyboardShowing = (keyboardFrameNew.height > 0
        && keyboardFrameNew.y < [UIApplication sharedApplication].keyWindow.frame.size.height);
    if(keyboardFrameNew != self.keyboardFrame)
    {
        self.keyboardFrame = keyboardFrameNew;

        ZFPROTOCOL_INTERFACE_CLASS(ZFUIOnScreenKeyboardState) *impl = ZFPROTOCOL_TRY_ACCESS(ZFUIOnScreenKeyboardState);
        if(impl != zfnull && ZFUISysWindow::mainWindowAttached())
        {
            impl->notifyKeyboardStateOnChange(ZFUIOnScreenKeyboardState::instanceForSysWindow());
        }
    }
}
@end

ZF_NAMESPACE_GLOBAL_BEGIN
ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFUIOnScreenKeyboardStateImpl_sys_iOS_DataHolder, ZFLevelZFFrameworkEssential)
{
    _ZFP_ZFUIOnScreenKeyboardStateImpl_sys_iOS_instance = [_ZFP_ZFUIOnScreenKeyboardStateImpl_sys_iOS new];
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFUIOnScreenKeyboardStateImpl_sys_iOS_DataHolder)
{
    _ZFP_ZFUIOnScreenKeyboardStateImpl_sys_iOS_instance = nil;
}
ZF_GLOBAL_INITIALIZER_END(ZFUIOnScreenKeyboardStateImpl_sys_iOS_DataHolder)

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUIOnScreenKeyboardStateImpl_sys_iOS, ZFUIOnScreenKeyboardState, ZFProtocolLevel::e_SystemNormal)
public:
    virtual zfbool keyboardShowing(ZF_IN ZFUIOnScreenKeyboardState *keyboardState)
    {
        return _ZFP_ZFUIOnScreenKeyboardStateImpl_sys_iOS_instance.keyboardShowing;
    }
    virtual ZFUIRect keyboardFrame(ZF_IN ZFUIOnScreenKeyboardState *keyboardState)
    {
        return _ZFP_ZFUIOnScreenKeyboardStateImpl_sys_iOS_instance.keyboardFrame;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFUIOnScreenKeyboardStateImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUIOnScreenKeyboardStateImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

