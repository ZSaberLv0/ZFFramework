#include "ZFImpl_sys_iOS_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFObjectMutex.h"

#if ZF_ENV_sys_iOS

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFObjectMutexImpl_sys_iOS
{
public:
    static void *implInit(void)
    {
        return (__bridge_retained void *)[NSRecursiveLock new];
    }
    static void implDealloc(ZF_IN void *implObject)
    {
        NSRecursiveLock *t = (__bridge_transfer NSRecursiveLock *)implObject;
        t = nil;
    }
    static void implLock(ZF_IN void *implObject)
    {
        [(__bridge NSRecursiveLock *)implObject lock];
    }
    static void implUnlock(ZF_IN void *implObject)
    {
        [(__bridge NSRecursiveLock *)implObject unlock];
    }
    static zfbool implTryLock(ZF_IN void *implObject)
    {
        return (zfbool)[(__bridge NSRecursiveLock *)implObject tryLock];
    }
};
ZFOBJECT_MUTEX_IMPL_DEFINE(ZFObjectMutexImpl_sys_iOS, ZFProtocolLevel::e_SystemNormal, {
        ZFObjectMutexImplSet(
                _ZFP_ZFObjectMutexImpl_sys_iOS::implInit,
                _ZFP_ZFObjectMutexImpl_sys_iOS::implDealloc,
                _ZFP_ZFObjectMutexImpl_sys_iOS::implLock,
                _ZFP_ZFObjectMutexImpl_sys_iOS::implUnlock,
                _ZFP_ZFObjectMutexImpl_sys_iOS::implTryLock
            );
    })

ZF_NAMESPACE_GLOBAL_END

#endif // #if ZF_ENV_sys_iOS

