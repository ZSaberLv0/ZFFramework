#include "ZFImpl_sys_Qt_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFSemaphore.h"

#if ZF_ENV_sys_Qt
#include <QWaitCondition>
#include <QMutex>

class _ZFP_ZFSemaphoreImpl_sys_Qt_Semaphore : public QWaitCondition
{
public:
    _ZFP_ZFSemaphoreImpl_sys_Qt_Semaphore(void)
    : QWaitCondition()
    {
    }
public:
    QMutex _ZFP_mutex;
};

ZF_NAMESPACE_GLOBAL_BEGIN

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFSemaphoreImpl_sys_Qt, ZFSemaphore, ZFProtocolLevel::e_SystemHigh)
    ZFPROTOCOL_IMPLEMENTATION_PLATFORM_HINT("Qt:QWaitCondition")
public:
    virtual void *nativeSemaphoreCreate(ZF_IN ZFSemaphore *semaphore)
    {
        return zfnew(_ZFP_ZFSemaphoreImpl_sys_Qt_Semaphore);
    }
    virtual void nativeSemaphoreDestroy(ZF_IN ZFSemaphore *semaphore,
                                        ZF_IN void *nativeSemaphore)
    {
        zfdelete(ZFCastStatic(_ZFP_ZFSemaphoreImpl_sys_Qt_Semaphore *, nativeSemaphore));
    }

    virtual void semaphoreLock(ZF_IN ZFSemaphore *semaphore)
    {
        _ZFP_ZFSemaphoreImpl_sys_Qt_Semaphore *nativeSemaphore = ZFCastStatic(_ZFP_ZFSemaphoreImpl_sys_Qt_Semaphore *, semaphore->nativeSemaphore());
        nativeSemaphore->_ZFP_mutex.lock();
    }
    virtual void semaphoreUnlock(ZF_IN ZFSemaphore *semaphore)
    {
        _ZFP_ZFSemaphoreImpl_sys_Qt_Semaphore *nativeSemaphore = ZFCastStatic(_ZFP_ZFSemaphoreImpl_sys_Qt_Semaphore *, semaphore->nativeSemaphore());
        nativeSemaphore->_ZFP_mutex.unlock();
    }

    virtual void semaphoreSignal(ZF_IN ZFSemaphore *semaphore)
    {
        _ZFP_ZFSemaphoreImpl_sys_Qt_Semaphore *nativeSemaphore = ZFCastStatic(_ZFP_ZFSemaphoreImpl_sys_Qt_Semaphore *, semaphore->nativeSemaphore());
        nativeSemaphore->wakeOne();
    }
    virtual void semaphoreBroadcast(ZF_IN ZFSemaphore *semaphore)
    {
        _ZFP_ZFSemaphoreImpl_sys_Qt_Semaphore *nativeSemaphore = ZFCastStatic(_ZFP_ZFSemaphoreImpl_sys_Qt_Semaphore *, semaphore->nativeSemaphore());
        nativeSemaphore->wakeAll();
    }
    virtual void semaphoreWait(ZF_IN ZFSemaphore *semaphore)
    {
        _ZFP_ZFSemaphoreImpl_sys_Qt_Semaphore *nativeSemaphore = ZFCastStatic(_ZFP_ZFSemaphoreImpl_sys_Qt_Semaphore *, semaphore->nativeSemaphore());
        nativeSemaphore->wait(&(nativeSemaphore->_ZFP_mutex));
    }
    virtual zfbool semaphoreWait(ZF_IN ZFSemaphore *semaphore,
                                 ZF_IN zftimet miliSecsTimeout)
    {
        _ZFP_ZFSemaphoreImpl_sys_Qt_Semaphore *nativeSemaphore = ZFCastStatic(_ZFP_ZFSemaphoreImpl_sys_Qt_Semaphore *, semaphore->nativeSemaphore());
        zfbool ret = nativeSemaphore->wait(&(nativeSemaphore->_ZFP_mutex), miliSecsTimeout);
        return ret;
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFSemaphoreImpl_sys_Qt)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFSemaphoreImpl_sys_Qt)

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Qt

