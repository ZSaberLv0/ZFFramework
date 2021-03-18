#include "ZFImpl_sys_iOS_ZF_impl.h"
#include "ZFMainEntry_sys_iOS.h"
#include "ZFCore/ZFMainEntry.h"
#include "ZFCore/ZFArray.h"
#include "ZFCore/protocol/ZFProtocolZFMainEntry.h"

#if ZF_ENV_sys_iOS

ZF_NAMESPACE_GLOBAL_BEGIN

static UIApplication *_ZFP_ZFImpl_sys_iOS_application = nil;
static UIWindow *_ZFP_ZFImpl_sys_iOS_rootWindow = nil;

UIApplication *ZFImpl_sys_iOS_application(void)
{
    return _ZFP_ZFImpl_sys_iOS_application;
}
UIWindow *ZFImpl_sys_iOS_rootWindow(void)
{
    return _ZFP_ZFImpl_sys_iOS_rootWindow;
}

ZF_NAMESPACE_GLOBAL_END

// ============================================================
// app entry
@interface _ZFP_ZFImpl_sys_iOS_AppEventHolder : NSObject
@property (nonatomic, assign) BOOL _ZFP_appResumeFlag;
@end
@implementation _ZFP_ZFImpl_sys_iOS_AppEventHolder
- (void)appOnCreate:(NSNotification *)notification
{
    _ZFP_ZFImpl_sys_iOS_application = [UIApplication sharedApplication];
    _ZFP_ZFImpl_sys_iOS_rootWindow = _ZFP_ZFImpl_sys_iOS_application.keyWindow;

    if(_ZFP_ZFImpl_sys_iOS_rootWindow.backgroundColor == nil)
    {
        _ZFP_ZFImpl_sys_iOS_rootWindow.backgroundColor = [UIColor whiteColor];
    }

    ZFFrameworkInit();
    ZFMainExecute();

    [self appOnResume];

#if ZF_ENV_DEBUG && 0 // framework's init and dealloc
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(1 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        NSLog(@"test cleanup");
        ZFFrameworkCleanup();
        NSLog(@"test cleanup end");
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(1 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
            NSLog(@"test re-init");
            ZFFrameworkInit();
            NSLog(@"test re-init end");
            ZFMainExecute();
            NSLog(@"test main re-enter end");
        });
    });
#endif
}
- (void)appOnDestroy
{
    ZFFrameworkCleanup();

    _ZFP_ZFImpl_sys_iOS_application = nil;
    _ZFP_ZFImpl_sys_iOS_rootWindow = nil;
}
- (void)appOnPause
{
    if(self._ZFP_appResumeFlag)
    {
        self._ZFP_appResumeFlag = NO;
        [_ZFP_ZFImpl_sys_iOS_rootWindow.rootViewController viewWillDisappear:NO];
    }
}
- (void)appOnResume
{
    if(!self._ZFP_appResumeFlag)
    {
        self._ZFP_appResumeFlag = YES;
        [_ZFP_ZFImpl_sys_iOS_rootWindow.rootViewController viewWillAppear:NO];
    }
}
- (void)appOnReceiveMemoryWarning
{
    ZFGlobalEventCenter::instance()->observerNotify(ZFGlobalEvent::EventAppOnMemoryLow());
}
@end

ZF_NAMESPACE_GLOBAL_BEGIN

_ZFP_ZFImpl_sys_iOS_AppEventHolder *_ZFP_ZFMainEntry_sys_iOS_appEventHolder = nil;
ZF_STATIC_REGISTER_INIT(_ZFP_ZFImpl_sys_iOS_AppEventHolder)
{
    _ZFP_ZFMainEntry_sys_iOS_appEventHolder = [_ZFP_ZFImpl_sys_iOS_AppEventHolder new];

    [[NSNotificationCenter defaultCenter] addObserver:_ZFP_ZFMainEntry_sys_iOS_appEventHolder selector:@selector(appOnCreate:) name:UIApplicationDidFinishLaunchingNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:_ZFP_ZFMainEntry_sys_iOS_appEventHolder selector:@selector(appOnDestroy) name:UIApplicationWillTerminateNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:_ZFP_ZFMainEntry_sys_iOS_appEventHolder selector:@selector(appOnPause) name:UIApplicationWillResignActiveNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:_ZFP_ZFMainEntry_sys_iOS_appEventHolder selector:@selector(appOnResume) name:UIApplicationWillEnterForegroundNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:_ZFP_ZFMainEntry_sys_iOS_appEventHolder selector:@selector(appOnReceiveMemoryWarning) name:UIApplicationDidReceiveMemoryWarningNotification object:nil];
}
ZF_STATIC_REGISTER_DESTROY(_ZFP_ZFImpl_sys_iOS_AppEventHolder)
{
    [[NSNotificationCenter defaultCenter] removeObserver:_ZFP_ZFMainEntry_sys_iOS_appEventHolder];
    _ZFP_ZFMainEntry_sys_iOS_appEventHolder = nil;
}
ZF_STATIC_REGISTER_END(_ZFP_ZFImpl_sys_iOS_AppEventHolder)

ZF_NAMESPACE_GLOBAL_END

#endif

