#include "ZFImpl_sys_Posix_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFObjectMutex.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"

#if ZF_ENV_sys_Posix || ZF_ENV_sys_unknown
#include <pthread.h>
#include <sys/syscall.h>
#include <sys/types.h>

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFObjectMutexImpl_sys_Posix
{
public:
    static void *implInit(void)
    {
        pthread_mutex_t *mutex = (pthread_mutex_t *)zfmalloc(sizeof(pthread_mutex_t));
        pthread_mutexattr_t Attr;
        pthread_mutexattr_init(&Attr);
        pthread_mutexattr_settype(&Attr, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(mutex, &Attr);
        return mutex;
    }
    static void implDealloc(ZF_IN void *implObject)
    {
        pthread_mutex_t *mutex = (pthread_mutex_t *)implObject;
        pthread_mutex_destroy(mutex);
    }
    static void implLock(ZF_IN void *implObject)
    {
        pthread_mutex_t *mutex = (pthread_mutex_t *)implObject;
        pthread_mutex_lock(mutex);
    }
    static void implUnlock(ZF_IN void *implObject)
    {
        pthread_mutex_t *mutex = (pthread_mutex_t *)implObject;
        pthread_mutex_unlock(mutex);
    }
    static zfbool implTryLock(ZF_IN void *implObject)
    {
        pthread_mutex_t *mutex = (pthread_mutex_t *)implObject;
        return (pthread_mutex_trylock(mutex) == 0);
    }
};
ZFOBJECT_MUTEX_IMPL_DEFINE(ZFObjectMutexImpl_sys_Posix, ZFProtocolLevel::e_SystemLow, {
        ZFObjectMutexImplSet(
                _ZFP_ZFObjectMutexImpl_sys_Posix::implInit,
                _ZFP_ZFObjectMutexImpl_sys_Posix::implDealloc,
                _ZFP_ZFObjectMutexImpl_sys_Posix::implLock,
                _ZFP_ZFObjectMutexImpl_sys_Posix::implUnlock,
                _ZFP_ZFObjectMutexImpl_sys_Posix::implTryLock
            );
    })

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Posix || ZF_ENV_sys_unknown

