#include "ZFImpl_sys_iOS_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFSemaphore.h"

#if ZF_ENV_sys_iOS

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFSemaphoreImpl_sys_iOS, ZFSemaphore, ZFProtocolLevel::e_SystemNormal)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("iOS:NSCondition")
public:
    virtual void *nativeSemaphoreCreate(ZF_IN ZFSemaphore *semaphore)
    {
        return (__bridge_retained void *)[NSCondition new];
    }
    virtual void nativeSemaphoreDestroy(ZF_IN ZFSemaphore *semaphore,
                                        ZF_IN void *nativeSemaphore)
    {
        NSCondition *tmp = (__bridge_transfer NSCondition *)nativeSemaphore;
        tmp = nil;
    }

    virtual void semaphoreLock(ZF_IN ZFSemaphore *semaphore)
    {
        [(__bridge NSCondition *)semaphore->nativeSemaphore() lock];
    }
    virtual void semaphoreUnlock(ZF_IN ZFSemaphore *semaphore)
    {
        [(__bridge NSCondition *)semaphore->nativeSemaphore() unlock];
    }

    virtual void semaphoreSignal(ZF_IN ZFSemaphore *semaphore)
    {
        [(__bridge NSCondition *)semaphore->nativeSemaphore() signal];
    }
    virtual void semaphoreBroadcast(ZF_IN ZFSemaphore *semaphore)
    {
        [(__bridge NSCondition *)semaphore->nativeSemaphore() broadcast];
    }
    virtual void semaphoreWait(ZF_IN ZFSemaphore *semaphore)
    {
        [(__bridge NSCondition *)semaphore->nativeSemaphore() wait];
    }
    virtual zfbool semaphoreWait(ZF_IN ZFSemaphore *semaphore,
                                 ZF_IN zftimet miliSecsTimeout)
    {
        return [(__bridge NSCondition *)semaphore->nativeSemaphore() waitUntilDate:[NSDate dateWithTimeIntervalSinceNow:((NSTimeInterval)miliSecsTimeout / 1000)]];
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFSemaphoreImpl_sys_iOS)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFSemaphoreImpl_sys_iOS)

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

