#include "ZFImpl_sys_iOS_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFThread.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"

#if ZF_ENV_sys_iOS

// ============================================================
// global type
@interface _ZFP_ZFThreadImpl_sys_iOS_ThreadOwner : NSObject
@property (nonatomic, assign) zfidentity taskId;
@property (nonatomic, assign) ZFThread *ownerZFThread;
@property (nonatomic, assign) ZFListener runnable;
@property (nonatomic, assign) ZFObject *param0;
@property (nonatomic, assign) ZFObject *param1;
// private
@property (nonatomic, assign) void *_nativeThreadRegisterToken;
@property (nonatomic, strong) id _selfHolder;
- (void)threadCallback:(id)dummy;
@end
@implementation _ZFP_ZFThreadImpl_sys_iOS_ThreadOwner
- (void)threadCallback:(id)dummy
{
    @autoreleasepool {
        if(self.ownerZFThread != zfnull)
        {
            self._nativeThreadRegisterToken = ZFPROTOCOL_ACCESS(ZFThread)->nativeThreadRegister(self.ownerZFThread);
        }

        self.runnable.execute(ZFListenerData().param0(self.param0).param1(self.param1));

        if(self.ownerZFThread != zfnull)
        {
            ZFPROTOCOL_ACCESS(ZFThread)->nativeThreadUnregister(self._nativeThreadRegisterToken);
            self._nativeThreadRegisterToken = zfnull;
        }
    }

    self._selfHolder = nil;
}
@end

@interface _ZFP_ZFThreadImpl_sys_iOS_ExecuteInMainThreadAfterDelayOwner : NSObject
- (void)taskStart:(zftimet)delay
         runnable:(const ZFListener &)runnable
           param0:(ZFObject *)param0
           param1:(ZFObject *)param1;
- (void)taskStop;
@property (nonatomic, strong) id _selfHolder;
@property (nonatomic, assign) zfbool _running;
@property (nonatomic, assign) ZFListener _runnable;
@property (nonatomic, assign) ZFObject *_param0;
@property (nonatomic, assign) ZFObject *_param1;
- (void)_action;
@end
@implementation _ZFP_ZFThreadImpl_sys_iOS_ExecuteInMainThreadAfterDelayOwner
- (instancetype)init
{
    self = [super init];
    if(self)
    {
        self._running = zftrue;
    }
    return self;
}
- (void)taskStart:(zftimet)delay
         runnable:(const ZFListener &)runnable
           param0:(ZFObject *)param0
           param1:(ZFObject *)param1
{
    self._selfHolder = self;
    self._runnable = runnable;
    self._param0 = param0;
    self._param1 = param1;
    [self performSelector:@selector(_action) withObject:nil afterDelay:(delay / 1000.0f)];
}
- (void)taskStop
{
    if(self._running)
    {
        self._running = zffalse;
        [NSObject cancelPreviousPerformRequestsWithTarget:self];
        self._selfHolder = nil;
    }
}
- (void)_action
{
    if(self._running)
    {
        self._running = zffalse;
        self._runnable.execute(ZFListenerData().param0(self._param0).param1(self._param1));
        self._selfHolder = nil;
    }
}
@end

ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
// global data
typedef void * _ZFP_ZFThreadImpl_sys_iOS_NativeThreadIdType;
typedef zfstlmap<_ZFP_ZFThreadImpl_sys_iOS_NativeThreadIdType, ZFThread *> _ZFP_ZFThreadImpl_sys_iOS_ThreadMapType;

static _ZFP_ZFThreadImpl_sys_iOS_NativeThreadIdType _ZFP_ZFThreadImpl_sys_iOS_getNativeThreadId(void)
{
    return (__bridge void *)[NSThread currentThread];
}

ZF_GLOBAL_INITIALIZER_INIT_WITH_LEVEL(ZFThreadImpl_sys_iOS_DataHolder, ZFLevelZFFrameworkHigh)
{
    mainThread = zfAlloc(ZFThreadMainThread);
    threadMap[_ZFP_ZFThreadImpl_sys_iOS_getNativeThreadId()] = mainThread;
}
ZF_GLOBAL_INITIALIZER_DESTROY(ZFThreadImpl_sys_iOS_DataHolder)
{
    zfRelease(mainThread);
    mainThread = zfnull;
}
public:
    ZFThread *mainThread;
    _ZFP_ZFThreadImpl_sys_iOS_ThreadMapType threadMap;
ZF_GLOBAL_INITIALIZER_END(ZFThreadImpl_sys_iOS_DataHolder)
#define _ZFP_ZFThreadImpl_sys_iOS_mainThreadInstance (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFThreadImpl_sys_iOS_DataHolder)->mainThread)
#define _ZFP_ZFThreadImpl_sys_iOS_threadMap (ZF_GLOBAL_INITIALIZER_INSTANCE(ZFThreadImpl_sys_iOS_DataHolder)->threadMap)

// ============================================================
ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFThreadImpl_sys_iOS, ZFThread, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("iOS:NSThread")
public:
    zfoverride
    virtual void protocolOnInit(void)
    {
        zfsuper::protocolOnInit();
        this->_syncObj = [NSObject new];
    }
    zfoverride
    virtual void protocolOnDealloc(void)
    {
        this->_syncObj = nil;
        zfsuper::protocolOnDealloc();
    }
    virtual void *nativeThreadRegister(ZF_IN ZFThread *ownerZFThread)
    {
        _ZFP_ZFThreadImpl_sys_iOS_NativeThreadIdType *token = zfnew(_ZFP_ZFThreadImpl_sys_iOS_NativeThreadIdType);
        *token = _ZFP_ZFThreadImpl_sys_iOS_getNativeThreadId();
        zfbool exist = zffalse;
        @synchronized(this->_syncObj)
        {
            exist = (_ZFP_ZFThreadImpl_sys_iOS_threadMap.find(*token) != _ZFP_ZFThreadImpl_sys_iOS_threadMap.end());
        }
        zfCoreAssertWithMessage(!exist, "thread already registered: %s", ownerZFThread->objectInfo().cString());
        @synchronized(this->_syncObj)
        {
            _ZFP_ZFThreadImpl_sys_iOS_threadMap[*token] = ownerZFThread;
        }
        return ZFCastStatic(void *, token);
    }
    virtual void nativeThreadUnregister(ZF_IN void *token)
    {
        @synchronized(this->_syncObj)
        {
            _ZFP_ZFThreadImpl_sys_iOS_threadMap.erase(_ZFP_ZFThreadImpl_sys_iOS_getNativeThreadId());
        }
        zfdelete(ZFCastStatic(_ZFP_ZFThreadImpl_sys_iOS_NativeThreadIdType *, token));
    }
    virtual ZFThread *threadForToken(ZF_IN void *token)
    {
        @synchronized(this->_syncObj)
        {
            _ZFP_ZFThreadImpl_sys_iOS_ThreadMapType::iterator it = _ZFP_ZFThreadImpl_sys_iOS_threadMap.find(
                *ZFCastStatic(_ZFP_ZFThreadImpl_sys_iOS_NativeThreadIdType *, token));
            if(it != _ZFP_ZFThreadImpl_sys_iOS_threadMap.end())
            {
                return it->second;
            }
        }
        return zfnull;
    }
    virtual ZFThread *mainThread(void)
    {
        return _ZFP_ZFThreadImpl_sys_iOS_mainThreadInstance;
    }
    virtual ZFThread *currentThread(void)
    {
        @synchronized(this->_syncObj)
        {
            _ZFP_ZFThreadImpl_sys_iOS_ThreadMapType::const_iterator it =
                _ZFP_ZFThreadImpl_sys_iOS_threadMap.find(_ZFP_ZFThreadImpl_sys_iOS_getNativeThreadId());
            if(it == _ZFP_ZFThreadImpl_sys_iOS_threadMap.end())
            {
                return zfnull;
            }
            return it->second;
        }
    }

    virtual void sleep(ZF_IN zftimet miliSecs)
    {
        [NSThread sleepForTimeInterval:((double)miliSecs / 1000)];
    }

    virtual void *executeInMainThread(ZF_IN zfidentity taskId,
                                      ZF_IN const ZFListener &runnable,
                                      ZF_IN ZFObject *param0,
                                      ZF_IN ZFObject *param1)
    {
        _ZFP_ZFThreadImpl_sys_iOS_ThreadOwner *threadOwner = [_ZFP_ZFThreadImpl_sys_iOS_ThreadOwner new];
        threadOwner._selfHolder = threadOwner;
        threadOwner.taskId = taskId;
        threadOwner.ownerZFThread = zfnull;
        threadOwner.runnable = runnable;
        threadOwner.param0 = param0;
        threadOwner.param1 = param1;
#if 1
        dispatch_async(dispatch_get_main_queue(), ^{
            [threadOwner threadCallback:nil];
        });
#else
        [threadOwner performSelectorOnMainThread:@selector(threadCallback:) withObject:zfnull waitUntilDone:NO];
#endif
        return zfnull;
    }
    virtual void executeInMainThreadCancel(ZF_IN zfidentity taskId,
                                           ZF_IN void *nativeToken)
    {
        // nothing to do
    }

    virtual void *executeInNewThread(ZF_IN zfidentity taskId,
                                     ZF_IN ZFThread *ownerZFThread,
                                     ZF_IN const ZFListener &runnable,
                                     ZF_IN ZFObject *param0,
                                     ZF_IN ZFObject *param1)
    {
        _ZFP_ZFThreadImpl_sys_iOS_ThreadOwner *threadOwner = [_ZFP_ZFThreadImpl_sys_iOS_ThreadOwner new];
        threadOwner._selfHolder = threadOwner;
        threadOwner.taskId = taskId;
        threadOwner.ownerZFThread = ownerZFThread;
        threadOwner.runnable = runnable;
        threadOwner.param0 = param0;
        threadOwner.param1 = param1;
#if 1
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
            [threadOwner threadCallback:nil];
        });
#else
        [threadOwner performSelectorInBackground:@selector(threadCallback:) withObject:nil];
#endif
        return zfnull;
    }
    virtual void executeInNewThreadCancel(ZF_IN zfidentity taskId,
                                          ZF_IN void *nativeToken)
    {
        // nothing to do
    }

    virtual void *executeInMainThreadAfterDelay(ZF_IN zfidentity taskId,
                                                ZF_IN zftimet delay,
                                                ZF_IN const ZFListener &runnable,
                                                ZF_IN ZFObject *param0,
                                                ZF_IN ZFObject *param1)
    {
        _ZFP_ZFThreadImpl_sys_iOS_ExecuteInMainThreadAfterDelayOwner *holder = [_ZFP_ZFThreadImpl_sys_iOS_ExecuteInMainThreadAfterDelayOwner new];
        [holder taskStart:delay runnable:runnable param0:param0 param1:param1];
        return (__bridge_retained void *)holder;
    }
    virtual void executeInMainThreadAfterDelayCancel(ZF_IN zfidentity taskId,
                                                     ZF_IN void *nativeToken)
    {
        _ZFP_ZFThreadImpl_sys_iOS_ExecuteInMainThreadAfterDelayOwner *holder = (__bridge _ZFP_ZFThreadImpl_sys_iOS_ExecuteInMainThreadAfterDelayOwner *)nativeToken;
        [holder taskStop];
    }
    virtual void executeInMainThreadAfterDelayCleanup(ZF_IN zfidentity taskId,
                                                      ZF_IN void *nativeToken)
    {
        _ZFP_ZFThreadImpl_sys_iOS_ExecuteInMainThreadAfterDelayOwner *holder = (__bridge_transfer _ZFP_ZFThreadImpl_sys_iOS_ExecuteInMainThreadAfterDelayOwner *)nativeToken;
        holder = nil;
    }

private:
    NSObject *_syncObj;
ZFPROTOCOL_IMPLEMENTATION_END(ZFThreadImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFThreadImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

