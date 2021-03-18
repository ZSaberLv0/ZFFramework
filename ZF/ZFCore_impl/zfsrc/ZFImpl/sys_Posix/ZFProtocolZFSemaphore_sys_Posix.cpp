#include "ZFImpl_sys_Posix_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFSemaphore.h"

#if ZF_ENV_sys_Posix || ZF_ENV_sys_unknown
#include <pthread.h>
#include <sys/time.h>

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFSemaphoreImpl_sys_Posix_Token
{
public:
    zfindex waiterCount;
    pthread_cond_t sema;
    pthread_mutex_t semaLocker;
};

ZFPROTOCOL_IMPLEMENTATION_BEGIN(ZFSemaphoreImpl_sys_Posix, ZFSemaphore, ZFProtocolLevel::e_SystemLow)
public:
    virtual void *nativeSemaphoreCreate(ZF_IN ZFSemaphore *semaphore)
    {
        _ZFP_ZFSemaphoreImpl_sys_Posix_Token *semaphoreToken = zfnew(_ZFP_ZFSemaphoreImpl_sys_Posix_Token);
        semaphoreToken->waiterCount = 0;
        pthread_cond_init(&(semaphoreToken->sema), zfnull);
        pthread_mutex_init(&(semaphoreToken->semaLocker), zfnull);
        return semaphoreToken;
    }
    virtual void nativeSemaphoreDestroy(ZF_IN ZFSemaphore *semaphore,
                                        ZF_IN void *nativeSemaphore)
    {
        _ZFP_ZFSemaphoreImpl_sys_Posix_Token *semaphoreToken = ZFCastStatic(_ZFP_ZFSemaphoreImpl_sys_Posix_Token *, nativeSemaphore);
        pthread_cond_destroy(&(semaphoreToken->sema));
        pthread_mutex_destroy(&(semaphoreToken->semaLocker));
        zfdelete(semaphoreToken);
    }

    virtual void semaphoreLock(ZF_IN ZFSemaphore *semaphore)
    {
        _ZFP_ZFSemaphoreImpl_sys_Posix_Token *semaphoreToken = ZFCastStatic(_ZFP_ZFSemaphoreImpl_sys_Posix_Token *, semaphore->nativeSemaphore());
        pthread_mutex_lock(&(semaphoreToken->semaLocker));
    }
    virtual void semaphoreUnlock(ZF_IN ZFSemaphore *semaphore)
    {
        _ZFP_ZFSemaphoreImpl_sys_Posix_Token *semaphoreToken = ZFCastStatic(_ZFP_ZFSemaphoreImpl_sys_Posix_Token *, semaphore->nativeSemaphore());
        pthread_mutex_unlock(&(semaphoreToken->semaLocker));
    }

    virtual void semaphoreSignal(ZF_IN ZFSemaphore *semaphore)
    {
        _ZFP_ZFSemaphoreImpl_sys_Posix_Token *semaphoreToken = ZFCastStatic(_ZFP_ZFSemaphoreImpl_sys_Posix_Token *, semaphore->nativeSemaphore());

        if(semaphoreToken->waiterCount > 0)
        {
            --(semaphoreToken->waiterCount);
            this->semaSignal(semaphoreToken, 1);
        }
    }
    virtual void semaphoreBroadcast(ZF_IN ZFSemaphore *semaphore)
    {
        _ZFP_ZFSemaphoreImpl_sys_Posix_Token *semaphoreToken = ZFCastStatic(_ZFP_ZFSemaphoreImpl_sys_Posix_Token *, semaphore->nativeSemaphore());

        if(semaphoreToken->waiterCount > 0)
        {
            zfindex tmp = semaphoreToken->waiterCount;
            semaphoreToken->waiterCount = 0;
            this->semaSignal(semaphoreToken, tmp);
        }
    }
    virtual void semaphoreWait(ZF_IN ZFSemaphore *semaphore)
    {
        _ZFP_ZFSemaphoreImpl_sys_Posix_Token *semaphoreToken = ZFCastStatic(_ZFP_ZFSemaphoreImpl_sys_Posix_Token *, semaphore->nativeSemaphore());

        ++(semaphoreToken->waiterCount);
        this->semaWait(semaphoreToken);
    }
    virtual zfbool semaphoreWait(ZF_IN ZFSemaphore *semaphore,
                                 ZF_IN zftimet miliSecsTimeout)
    {
        _ZFP_ZFSemaphoreImpl_sys_Posix_Token *semaphoreToken = ZFCastStatic(_ZFP_ZFSemaphoreImpl_sys_Posix_Token *, semaphore->nativeSemaphore());

        ++(semaphoreToken->waiterCount);

        if(this->semaWait(semaphoreToken, miliSecsTimeout))
        {
            return zftrue;
        }
        else
        {
            if(semaphoreToken->waiterCount > 0)
            {
                --(semaphoreToken->waiterCount);
            }
            return zffalse;
        }
    }

public:
    void semaSignal(ZF_IN _ZFP_ZFSemaphoreImpl_sys_Posix_Token *semaphoreToken,
                    ZF_IN zfindex num1orBroadcastNum)
    {
        if(num1orBroadcastNum == 1)
        {
            pthread_cond_signal(&(semaphoreToken->sema));
        }
        else
        {
            pthread_cond_broadcast(&(semaphoreToken->sema));
        }
    }
    void semaWait(ZF_IN _ZFP_ZFSemaphoreImpl_sys_Posix_Token *semaphoreToken)
    {
        pthread_cond_wait(&(semaphoreToken->sema), &(semaphoreToken->semaLocker));
    }
    zfbool semaWait(ZF_IN _ZFP_ZFSemaphoreImpl_sys_Posix_Token *semaphoreToken,
                    ZF_IN zftimet miliSecs)
    {
        timeval timev;
        timespec t;
        gettimeofday(&timev, zfnull);
        t.tv_sec = (time_t)(timev.tv_sec + (miliSecs / 1000));
        t.tv_nsec = (miliSecs % 1000) * 1000 * 1000;
        return (pthread_cond_timedwait(&(semaphoreToken->sema), &(semaphoreToken->semaLocker), &t) == 0);
    }
ZFPROTOCOL_IMPLEMENTATION_END(ZFSemaphoreImpl_sys_Posix)
ZFPROTOCOL_IMPLEMENTATION_REGISTER(ZFSemaphoreImpl_sys_Posix)

ZF_NAMESPACE_GLOBAL_END
#endif

