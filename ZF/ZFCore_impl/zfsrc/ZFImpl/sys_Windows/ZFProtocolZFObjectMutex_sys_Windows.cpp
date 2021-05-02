#include "ZFImpl_sys_Windows_ZFCore_impl.h"
#include "ZFCore/protocol/ZFProtocolZFObjectMutex.h"
#include "ZFCore/ZFSTLWrapper/zfstl_map.h"

#if ZF_ENV_sys_Windows
#include <Windows.h>

ZF_NAMESPACE_GLOBAL_BEGIN

zfclassNotPOD _ZFP_ZFObjectMutexImpl_sys_Windows
{
public:
    zfclassNotPOD _Mutex
    {
    public:
        CRITICAL_SECTION mutex;
        CRITICAL_SECTION mutexParamLocker;
        zfstlmap<DWORD, zfindex> lockedThreadCountMap;
        DWORD runningThreadId;
        DWORD INVALID_THREAD_ID;
    };

public:
    static void *implInit(void)
    {
        _Mutex *mutex = zfnew(_Mutex);
        mutex->INVALID_THREAD_ID = (DWORD)-1;
        InitializeCriticalSection(&(mutex->mutex));
        InitializeCriticalSection(&(mutex->mutexParamLocker));
        mutex->runningThreadId = mutex->INVALID_THREAD_ID;
        return mutex;
    }
    static void implDealloc(ZF_IN void *implObject)
    {
        _Mutex *mutex = (_Mutex *)implObject;
        DeleteCriticalSection(&(mutex->mutex));
        DeleteCriticalSection(&(mutex->mutexParamLocker));
        zfdelete(mutex);
    }
    static void implLock(ZF_IN void *implObject)
    {
        _Mutex *mutex = (_Mutex *)implObject;

        EnterCriticalSection(&(mutex->mutexParamLocker));
        DWORD curThreadId = GetCurrentThreadId();
        if(mutex->runningThreadId == mutex->INVALID_THREAD_ID)
        {
            mutex->lockedThreadCountMap[curThreadId] = 1;
            LeaveCriticalSection(&(mutex->mutexParamLocker));
            // first time to lock the object
            EnterCriticalSection(&(mutex->mutex));

            // now we owned the lock
            mutex->runningThreadId = curThreadId;
        }
        else
        {
            zfstlmap<DWORD, zfindex>::iterator it = mutex->lockedThreadCountMap.find(curThreadId);
            if(curThreadId == mutex->runningThreadId)
            {
                ++(it->second);
                LeaveCriticalSection(&(mutex->mutexParamLocker));
                // lock the same thread more than one time, no need to lock again
            }
            else
            {
                if(it == mutex->lockedThreadCountMap.end())
                {
                    mutex->lockedThreadCountMap[curThreadId] = 1;
                }
                else
                {
                    ++(it->second);
                }
                LeaveCriticalSection(&(mutex->mutexParamLocker));
                // lock while another thread acquiring the lock,
                // should be locked here until another thread release the lock
                EnterCriticalSection(&(mutex->mutex));

                // now we owned the lock
                mutex->runningThreadId = curThreadId;
            }
        }
    }
    static void implUnlock(ZF_IN void *implObject)
    {
        _Mutex *mutex = (_Mutex *)implObject;

        EnterCriticalSection(&(mutex->mutexParamLocker));
        DWORD curThreadId = GetCurrentThreadId();
        zfstlmap<DWORD, zfindex>::iterator it = mutex->lockedThreadCountMap.find(curThreadId);
        if(curThreadId == mutex->runningThreadId)
        {
            --(it->second);
            if(it->second == 0)
            {
                mutex->lockedThreadCountMap.erase(it);
                mutex->runningThreadId = mutex->INVALID_THREAD_ID;
                LeaveCriticalSection(&(mutex->mutexParamLocker));
                // all locks locked by current thread are relesed, unlock
                LeaveCriticalSection(&(mutex->mutex));
            }
            else
            {
                LeaveCriticalSection(&(mutex->mutexParamLocker));
                // current thread still hold some locks, just return
            }
        }
        else
        {
            LeaveCriticalSection(&(mutex->mutexParamLocker));

            // current thread doesn't lock, it's a error state
            zfCoreCriticalShouldNotGoHere();
            return ;
        }
    }
    static zfbool implTryLock(ZF_IN void *implObject)
    {
        _Mutex *mutex = (_Mutex *)implObject;

        EnterCriticalSection(&(mutex->mutexParamLocker));
        DWORD curThreadId = GetCurrentThreadId();
        if(mutex->runningThreadId == mutex->INVALID_THREAD_ID)
        {
            mutex->runningThreadId = curThreadId;
            mutex->lockedThreadCountMap[mutex->runningThreadId] = 1;
            LeaveCriticalSection(&(mutex->mutexParamLocker));
            // first time to lock the object
            EnterCriticalSection(&(mutex->mutex));
            return zftrue;
        }
        else
        {
            zfstlmap<DWORD, zfindex>::iterator it = mutex->lockedThreadCountMap.find(curThreadId);
            if(curThreadId == mutex->runningThreadId)
            {
                ++(it->second);
                LeaveCriticalSection(&(mutex->mutexParamLocker));
                // lock the same thread more than one time, no need to lock again
                return zftrue;
            }
            else
            {
                LeaveCriticalSection(&(mutex->mutexParamLocker));
                // lock while another thread acquiring the lock,
                // just return false for mutexTryLock
                return zffalse;
            }
        }
    }
};
ZFOBJECT_MUTEX_IMPL_DEFINE(ZFObjectMutexImpl_sys_Windows, ZFProtocolLevel::e_SystemLow, {
        ZFObjectMutexImplSet(
                _ZFP_ZFObjectMutexImpl_sys_Windows::implInit,
                _ZFP_ZFObjectMutexImpl_sys_Windows::implDealloc,
                _ZFP_ZFObjectMutexImpl_sys_Windows::implLock,
                _ZFP_ZFObjectMutexImpl_sys_Windows::implUnlock,
                _ZFP_ZFObjectMutexImpl_sys_Windows::implTryLock
            );
    })

ZF_NAMESPACE_GLOBAL_END
#endif // #if ZF_ENV_sys_Windows

