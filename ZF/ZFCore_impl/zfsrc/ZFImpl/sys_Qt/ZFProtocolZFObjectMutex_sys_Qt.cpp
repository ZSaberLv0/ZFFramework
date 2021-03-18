#include "ZFImpl_sys_Qt_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFObjectMutex.h"

#if ZF_ENV_sys_Qt
#include <QRecursiveMutex>

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFObjectMutexImpl_sys_Qt
{
public:
    static void *implInit(void)
    {
        return zfnew(QRecursiveMutex);
    }
    static void implDealloc(ZF_IN void *implObject)
    {
        QRecursiveMutex *mutex = (QRecursiveMutex *)implObject;
        zfdelete(mutex);
    }
    static void implLock(ZF_IN void *implObject)
    {
        QRecursiveMutex *mutex = (QRecursiveMutex *)implObject;
        mutex->lock();
    }
    static void implUnlock(ZF_IN void *implObject)
    {
        QRecursiveMutex *mutex = (QRecursiveMutex *)implObject;
        mutex->unlock();
    }
    static zfbool implTryLock(ZF_IN void *implObject)
    {
        QRecursiveMutex *mutex = (QRecursiveMutex *)implObject;
        return mutex->tryLock();
    }
};
ZFOBJECT_MUTEX_IMPL_DEFINE(ZFObjectMutexImpl_sys_Qt, ZFProtocolLevel::e_SystemHigh, {
        ZFObjectMutexImplSet(
                _ZFP_ZFObjectMutexImpl_sys_Qt::implInit,
                _ZFP_ZFObjectMutexImpl_sys_Qt::implDealloc,
                _ZFP_ZFObjectMutexImpl_sys_Qt::implLock,
                _ZFP_ZFObjectMutexImpl_sys_Qt::implUnlock,
                _ZFP_ZFObjectMutexImpl_sys_Qt::implTryLock
            );
    })

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Qt

