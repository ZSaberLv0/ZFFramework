/**
 * @file ZFProtocolZFThread.h
 * @brief protocol for ZFThread
 */

#ifndef _ZFI_ZFProtocolZFThread_h_
#define _ZFI_ZFProtocolZFThread_h_

#include "ZFCore/ZFProtocol.h"
#include "ZFCore/ZFThread.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief protocol for ZFThread
 */
ZFPROTOCOL_INTERFACE_BEGIN(ZFThread)
public:
    /**
     * @brief see #ZFThread::nativeThreadUnregister
     *
     * you should not retain or release ownerZFThread
     */
    virtual void *nativeThreadRegister(ZF_IN ZFThread *ownerZFThread) zfpurevirtual;
    /**
     * @brief see #ZFThread::nativeThreadUnregister, #nativeThreadRegister
     *
     * you should not retain or release ownerZFThread
     */
    virtual void nativeThreadUnregister(ZF_IN void *token) zfpurevirtual;
    /**
     * @brief get thread from token created by nativeThreadRegister
     */
    virtual ZFThread *threadForToken(ZF_IN void *token) zfpurevirtual;

    /**
     * @brief see #ZFThread::mainThread
     */
    virtual ZFThread *mainThread(void) zfpurevirtual;

    /**
     * @brief see #ZFThread::currentThread
     */
    virtual ZFThread *currentThread(void) zfpurevirtual;

    /**
     * @brief see #ZFThread::sleep
     */
    virtual void sleep(ZF_IN zftimet miliSecs) zfpurevirtual;

    /**
     * @brief see #ZFExecuteInMainThread
     *
     * you should not retain or release runnable or params\n
     * you should queue it to run even if current thread is main thread
     */
    virtual void *executeInMainThread(ZF_IN zfidentity taskId,
                                      ZF_IN const ZFListener &runnable,
                                      ZF_IN ZFObject *param0,
                                      ZF_IN ZFObject *param1) zfpurevirtual;
    /**
     * @brief cancel execute task
     *
     * you could do nothing if task can't be canceled\n
     * this function is designed for performance,
     * that you may cancel execute task if it hasn't started to run
     */
    virtual void executeInMainThreadCancel(ZF_IN zfidentity taskId,
                                           ZF_IN void *nativeToken)
    {
    }
    /**
     * @brief used to cleanup nativeToken
     */
    virtual void executeInMainThreadCleanup(ZF_IN zfidentity taskId,
                                            ZF_IN void *nativeToken)
    {
    }

    /**
     * @brief see #ZFExecuteInNewThread
     *
     * you should not retain or release runnable or params\n
     * you should not retain or release ownerZFThread\n
     * you should call runnable in new thread,
     * also, remember to register so that currentThread should work
     */
    virtual void *executeInNewThread(ZF_IN zfidentity taskId,
                                     ZF_IN ZFThread *ownerZFThread,
                                     ZF_IN const ZFListener &runnable,
                                     ZF_IN ZFObject *param0,
                                     ZF_IN ZFObject *param1) zfpurevirtual;
    /**
     * @brief cancel execute task
     *
     * you could do nothing if task can't be canceled\n
     * this function is designed for performance,
     * that you may cancel execute task if it hasn't started to run
     */
    virtual void executeInNewThreadCancel(ZF_IN zfidentity taskId,
                                          ZF_IN void *nativeToken)
    {
    }
    /**
     * @brief used to cleanup nativeToken
     */
    virtual void executeInNewThreadCleanup(ZF_IN zfidentity taskId,
                                           ZF_IN void *nativeToken)
    {
    }

    /**
     * @brief see #ZFExecuteAfterDelay
     *
     * you should not retain or release runnable or params\n
     * you should call runnable in main thread after specified delay\n
     * since delay may be interrupted,
     * you must implement #executeInMainThreadAfterDelayCancel to support task's cancel
     */
    virtual void *executeInMainThreadAfterDelay(ZF_IN zfidentity taskId,
                                                ZF_IN zftimet delay,
                                                ZF_IN const ZFListener &runnable,
                                                ZF_IN ZFObject *param0,
                                                ZF_IN ZFObject *param1) zfpurevirtual;
    /**
     * @brief cancel execute task
     */
    virtual void executeInMainThreadAfterDelayCancel(ZF_IN zfidentity taskId,
                                                     ZF_IN void *nativeToken) zfpurevirtual;
    /**
     * @brief used to cleanup nativeToken
     */
    virtual void executeInMainThreadAfterDelayCleanup(ZF_IN zfidentity taskId,
                                                      ZF_IN void *nativeToken)
    {
    }
ZFPROTOCOL_INTERFACE_END(ZFThread)

/** @brief see #ZFThread */
zfclass ZF_ENV_EXPORT ZFThreadMainThread : zfextends ZFThread
{
    ZFOBJECT_DECLARE(ZFThreadMainThread, ZFThread)

public:
    zfoverride
    virtual void threadStart(ZF_IN_OPT ZFObject *userData = zfnull,
                             ZF_IN_OPT ZFObject *param0 = zfnull,
                             ZF_IN_OPT ZFObject *param1 = zfnull)
    {
        zfCoreLogTrim("you must not start main thread");
    }
    zfoverride
    virtual zfbool threadStarted(void)
    {
        return zftrue;
    }
    zfoverride
    virtual zfbool threadRunning(void)
    {
        return zftrue;
    }
    zfoverride
    virtual void threadStop(void)
    {
        zfCoreLogTrim("you must not stop main thread");
    }
    zfoverride
    virtual void threadWait(void)
    {
        zfCoreLogTrim("you must not wait main thread");
    }
    zfoverride
    virtual zfbool threadWait(ZF_IN zftimet miliSecs)
    {
        zfCoreLogTrim("you must not wait main thread");
        return zffalse;
    }
    zfoverride
    virtual zfbool isMainThread(void)
    {
        return zftrue;
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFProtocolZFThread_h_

