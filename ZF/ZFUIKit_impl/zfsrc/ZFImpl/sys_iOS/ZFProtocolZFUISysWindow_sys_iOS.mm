#include "ZFImpl_sys_iOS_ZFUIKit_impl.h"
#include "ZFUIKit/protocol/ZFProtocolZFUISysWindow.h"
#include "ZFUIKit/protocol/ZFProtocolZFUIView.h"
#include "ZFImpl/sys_iOS/ZFMainEntry_sys_iOS.h"

#if ZF_ENV_sys_iOS

@interface _ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow_Dummy : UIViewController
@property (nonatomic, weak) UIViewController *_ZFP_owner;
@end
@implementation _ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow_Dummy
- (BOOL)shouldAutorotate
{
    return [self._ZFP_owner shouldAutorotate];
}
- (UIInterfaceOrientationMask)supportedInterfaceOrientations
{
    return [self._ZFP_owner supportedInterfaceOrientations];
}
@end

@interface _ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow : UIViewController
@property (nonatomic, assign) ZFPROTOCOL_INTERFACE_CLASS(ZFUISysWindow) *impl;
@property (nonatomic, assign) ZFUISysWindow *ownerZFUISysWindow;
@property (nonatomic, assign) ZFUIOrientationEnum sysWindowOrientation;
@property (nonatomic, assign) ZFUIOrientationFlags sysWindowOrientationFlags;
@property (nonatomic, assign) zfint _ZFP_windowRotateOverrideFlag;
- (void)_ZFP_updateLayout;
@end
@implementation _ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow
- (void)setSysWindowOrientationFlags:(ZF::ZFUIOrientationFlags)sysWindowOrientationFlags
{
    if(self->_sysWindowOrientationFlags == sysWindowOrientationFlags)
    {
        return ;
    }
    self->_sysWindowOrientationFlags = sysWindowOrientationFlags;

    [UIViewController attemptRotationToDeviceOrientation];
#if 1 // force to refresh orientation state
    if(self.ownerZFUISysWindow != zfnull && self.ownerZFUISysWindow->nativeWindowIsResumed())
    {
        self._ZFP_windowRotateOverrideFlag += 1;
        _ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow_Dummy *dummy = [_ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow_Dummy new];
        UIViewController *t = self;
        while(t.presentedViewController != nil)
        {
            t = t.presentedViewController;
        }
        dummy._ZFP_owner = t;
        [t presentViewController:dummy animated:NO completion:nil];
        __block typeof(self) weakSelf = self;
        [t dismissViewControllerAnimated:NO completion:^{
            weakSelf._ZFP_windowRotateOverrideFlag -= 1;
            if(weakSelf.ownerZFUISysWindow != zfnull)
            {
                if(!weakSelf.ownerZFUISysWindow->nativeWindowIsResumed())
                {
                    weakSelf.impl->notifyOnResume(weakSelf.ownerZFUISysWindow);
                }
            }
        }];
    }
#endif
}
// ============================================================
- (void)_ZFP_updateLayout
{
    if(self.ownerZFUISysWindow != zfnull)
    {
        ZFUIMargin sysWindowMargin = ZFUIMarginZero();

        UIView *tmpView = [UIApplication sharedApplication].keyWindow.rootViewController.view;
        if([tmpView respondsToSelector:@selector(safeAreaInsets)])
        {
            NSMethodSignature *methodSig = [tmpView methodSignatureForSelector:@selector(safeAreaInsets)];
            NSInvocation *invocation = [NSInvocation invocationWithMethodSignature:methodSig];
            invocation.target = tmpView;
            invocation.selector = @selector(safeAreaInsets);
            [invocation invoke];
            UIEdgeInsets t = UIEdgeInsetsZero;
            [invocation getReturnValue:&t];

            sysWindowMargin.left = t.left;
            sysWindowMargin.top = t.top;
            sysWindowMargin.right = t.right;
            sysWindowMargin.bottom = t.bottom;
        }
        else
        {
            sysWindowMargin.top = [UIApplication sharedApplication].statusBarFrame.size.height;
        }

        UIView *nativeRootView = (__bridge UIView *)self.ownerZFUISysWindow->rootView()->nativeView();
        nativeRootView.frame = ZFImpl_sys_iOS_ZFUIKit_impl_ZFUIRectToCGRect(self.impl->notifyMeasureWindow(
            self.ownerZFUISysWindow,
            ZFImpl_sys_iOS_ZFUIKit_impl_ZFUIRectFromCGRect(self.view.bounds),
            sysWindowMargin));
    }
}
// ============================================================
// init and dealloc
- (id)init
{
    self = [super init];
    if(self)
    {
        self.sysWindowOrientation = ZFUIOrientation::e_Top;
        self->_sysWindowOrientationFlags = ZFUIOrientation::e_Top;
    }
    return self;
}

- (void)viewWillTransitionToSize:(CGSize)size withTransitionCoordinator:(id<UIViewControllerTransitionCoordinator>)coordinator
{
    UIInterfaceOrientation toInterfaceOrientation = [[UIApplication sharedApplication] statusBarOrientation];
    switch(toInterfaceOrientation)
    {
        case UIInterfaceOrientationPortrait:
            self.sysWindowOrientation = ZFUIOrientation::e_Top;
            break;
        case UIInterfaceOrientationPortraitUpsideDown:
            self.sysWindowOrientation = ZFUIOrientation::e_Bottom;
            break;
        case UIInterfaceOrientationLandscapeLeft:
            self.sysWindowOrientation = ZFUIOrientation::e_Left;
            break;
        case UIInterfaceOrientationLandscapeRight:
            self.sysWindowOrientation = ZFUIOrientation::e_Right;
            break;
        default:
            self.sysWindowOrientation = ZFUIOrientation::e_Top;
            break;
    }
    if(self.ownerZFUISysWindow != zfnull)
    {
        self.impl->notifyOnRotate(self.ownerZFUISysWindow);
    }
}
- (UIInterfaceOrientationMask)supportedInterfaceOrientations
{
    if(self.presentingViewController != nil)
    {
        return [self.presentingViewController supportedInterfaceOrientations];
    }

    UIInterfaceOrientationMask ret = 0;
    if(ZFBitTest(self.sysWindowOrientationFlags, ZFUIOrientation::e_Top))
    {
        ret |= UIInterfaceOrientationMaskPortrait;
    }
    if(ZFBitTest(self.sysWindowOrientationFlags, ZFUIOrientation::e_Left))
    {
        ret |= UIInterfaceOrientationMaskLandscapeLeft;
    }
    if(ZFBitTest(self.sysWindowOrientationFlags, ZFUIOrientation::e_Right))
    {
        ret |= UIInterfaceOrientationMaskLandscapeRight;
    }
    if(ZFBitTest(self.sysWindowOrientationFlags, ZFUIOrientation::e_Bottom))
    {
        ret |= UIInterfaceOrientationMaskPortraitUpsideDown;
    }
    if(ret == 0)
    {
        ret = UIInterfaceOrientationMaskPortrait;
    }
    return ret;
}
- (BOOL)shouldAutorotate
{
    if(self.presentingViewController != nil)
    {
        return [self.presentingViewController shouldAutorotate];
    }

    return YES;
}

- (void)viewWillLayoutSubviews
{
    [super viewWillLayoutSubviews];
    [self _ZFP_updateLayout];
}
- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    if(self.ownerZFUISysWindow != zfnull && self._ZFP_windowRotateOverrideFlag == 0)
    {
        if(!self.ownerZFUISysWindow->nativeWindowIsResumed())
        {
            self.impl->notifyOnResume(self.ownerZFUISysWindow);
        }
    }
}
- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    if(self.ownerZFUISysWindow != zfnull && self._ZFP_windowRotateOverrideFlag == 0)
    {
        if(self.ownerZFUISysWindow->nativeWindowIsResumed())
        {
            self.impl->notifyOnPause(self.ownerZFUISysWindow);
        }
    }
}
@end

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFUISysWindowImpl_sys_iOS, ZFUISysWindow, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("iOS:UIViewController")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_BEGIN()
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_ITEM(ZFUIView, "iOS:UIView")
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_DEPENDENCY_END()

public:
    zfoverride
    virtual void protocolOnInit(void)
    {
        zfsuper::protocolOnInit();
        this->_mainWindow = zfnull;
    }
    zfoverride
    virtual void protocolOnDeallocPrepare(void)
    {
        this->mainWindowOnCleanup();
        zfsuper::protocolOnDeallocPrepare();
    }
public:
    virtual ZFUISysWindow *mainWindow(void)
    {
        if(this->_mainWindow == zfnull)
        {
            this->_mainWindow = zfRetain(ZFUISysWindow::ClassData()->newInstance().to<ZFUISysWindow *>());
            _ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow *nativeWindow = [_ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow new];
            nativeWindow.ownerZFUISysWindow = this->_mainWindow;

            // delay to create, because:
            // * allow app to change orientation before actually show,
            //   prevent strange rotate animation during launch
            // * during app launch, chaning window orientation would cause
            //   "Unbalanced calls to begin/end appearance transitions"
            dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
                this->notifyOnCreate(this->_mainWindow, (__bridge_retained void *)nativeWindow);

                ZFImpl_sys_iOS_rootWindow().rootViewController = nativeWindow;
                [ZFImpl_sys_iOS_rootWindow() makeKeyAndVisible];
            });
        }
        return this->_mainWindow;
    }
    virtual void mainWindowOnCleanup(void)
    {
        if(this->_mainWindow != zfnull)
        {
            _ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow *nativeWindow = (__bridge_transfer _ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow *)this->_mainWindow->nativeWindow();
            nativeWindow.ownerZFUISysWindow = zfnull;
            if(ZFImpl_sys_iOS_rootWindow().rootViewController == nativeWindow)
            {
                ZFImpl_sys_iOS_rootWindow().rootViewController = nil;
            }

            if(this->_mainWindow->nativeWindowIsResumed())
            {
                this->notifyOnPause(this->_mainWindow);
            }
            zfblockedRelease(this->_mainWindow);
            this->notifyOnDestroy(this->_mainWindow);
            nativeWindow = nil;
        }
    }
    virtual void mainWindowOnDestroy(void)
    {
        this->_mainWindow = zfnull;
    }

    // ============================================================
public:
    virtual void nativeWindowOnCleanup(ZF_IN ZFUISysWindow *sysWindow)
    {
    }

    virtual void nativeWindowRootViewOnAdd(ZF_IN ZFUISysWindow *sysWindow,
                                           ZF_OUT void *&nativeParentView)
    {
        _ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow *nativeWindow = (__bridge _ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow *)sysWindow->nativeWindow();

        UIView *nativeRootView = (__bridge UIView *)sysWindow->rootView()->nativeView();
        [nativeWindow.view addSubview:nativeRootView];
        [nativeWindow _ZFP_updateLayout];
        nativeParentView = (__bridge void *)nativeWindow.view;
    }
    virtual void nativeWindowRootViewOnRemove(ZF_IN ZFUISysWindow *sysWindow)
    {
        UIView *nativeRootView = (__bridge UIView *)sysWindow->rootView()->nativeView();
        [nativeRootView removeFromSuperview];
    }

    virtual zfautoObject modalWindowShow(ZF_IN ZFUISysWindow *sysWindowOwner)
    {
        zfautoObject modalWindow = ZFUISysWindow::ClassData()->newInstance();
        _ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow *nativeModalWindow = [_ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow new];
        nativeModalWindow.ownerZFUISysWindow = modalWindow;

        this->notifyOnCreate(modalWindow, (__bridge_retained void *)nativeModalWindow);
        [(__bridge UIViewController *)sysWindowOwner->nativeWindow() presentViewController:nativeModalWindow animated:YES completion:nil];

        return modalWindow;
    }
    virtual void modalWindowFinish(ZF_IN ZFUISysWindow *sysWindowOwner,
                                   ZF_IN ZFUISysWindow *sysWindowToFinish)
    {
        [(__bridge UIViewController *)sysWindowOwner->nativeWindow() dismissViewControllerAnimated:YES completion:nil];
        this->notifyOnDestroy(sysWindowToFinish);
    }

    virtual void sysWindowLayoutParamOnInit(ZF_IN ZFUISysWindow *sysWindow)
    {
    }
    virtual void sysWindowLayoutParamOnChange(ZF_IN ZFUISysWindow *sysWindow)
    {
        [(__bridge _ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow *)sysWindow->nativeWindow() _ZFP_updateLayout];
    }

    virtual ZFUIOrientationEnum sysWindowOrientation(ZF_IN ZFUISysWindow *sysWindow)
    {
        return ((__bridge _ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow *)sysWindow->nativeWindow()).sysWindowOrientation;
    }
    virtual void sysWindowOrientationFlags(ZF_IN ZFUISysWindow *sysWindow,
                                           ZF_IN const ZFUIOrientationFlags &flags)
    {
        ((__bridge _ZFP_ZFUISysWindowImpl_sys_iOS_NativeWindow *)sysWindow->nativeWindow()).sysWindowOrientationFlags = flags;
    }

private:
    ZFUISysWindow *_mainWindow;
ZFPROTOCOL_IMPLEMENTATION_END(ZFUISysWindowImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFUISysWindowImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

