/**
 * @file ZFThread_execute.h
 * @brief thread utility
 */

#ifndef _ZFI_ZFThread_execute_h_
#define _ZFI_ZFThread_execute_h_

#include "ZFThread.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief execute in main thread
 *
 * automatically retain runnable's owner and userData, and release them after finish\n
 * runnable won't be executed immediately even if current thread is main thread,
 * it's queued instead,
 * unless you call this function in main thread and set waitUntilDone to zftrue\n
 * \n
 * note, sender would be retained until execute finish,
 * while owner won't
 * @see ZFThreadExecuteInNewThread, ZFThreadExecuteCancel
 */
ZFMETHOD_FUNC_DECLARE_4(zfidentity, ZFThreadExecuteInMainThread,
                        ZFMP_IN(const ZFListener &, runnable),
                        ZFMP_IN_OPT(ZFObject *, userData, zfnull),
                        ZFMP_IN_OPT(ZFObject *, owner, zfnull),
                        ZFMP_IN_OPT(zfbool, waitUntilDone, zffalse))
/**
 * @brief util method for #ZFThreadExecuteInMainThread
 */
ZFMETHOD_FUNC_INLINE_DECLARE_3(zfidentity, ZFThreadExecuteInMainThreadWaitUntilDone,
                               ZFMP_IN(const ZFListener &, runnable),
                               ZFMP_IN_OPT(ZFObject *, userData, zfnull),
                               ZFMP_IN_OPT(ZFObject *, owner, zfnull))
{
    return ZFThreadExecuteInMainThread(runnable, userData, owner, zftrue);
}

/**
 * @brief execute in new thread
 *
 * automatically retain runnable's owner and userData, and release them after finish
 * @see ZFThreadExecuteInMainThread, ZFThreadExecuteCancel
 * @note always try this method first to achieve thread processing,
 *   instead of create new ZFThread instance,
 *   since we may have thread pool for performance
 */
ZFMETHOD_FUNC_DECLARE_3(zfidentity, ZFThreadExecuteInNewThread,
                        ZFMP_IN(const ZFListener &, runnable),
                        ZFMP_IN_OPT(ZFObject *, userData, zfnull),
                        ZFMP_IN_OPT(ZFObject *, owner, zfnull))

/**
 * @brief exeute in main thread after delay, directly schedule an #ZFThreadExecuteInMainThread if (delay <= 0)
 */
ZFMETHOD_FUNC_DECLARE_4(zfidentity, ZFThreadExecuteInMainThreadAfterDelay,
                        ZFMP_IN(zftimet, delay),
                        ZFMP_IN(const ZFListener &, runnable),
                        ZFMP_IN_OPT(ZFObject *, userData, zfnull),
                        ZFMP_IN_OPT(ZFObject *, owner, zfnull))

/**
 * @brief cancel execute of task
 */
ZFMETHOD_FUNC_DECLARE_1(void, ZFThreadExecuteCancel,
                        ZFMP_IN(zfidentity, taskId))

/**
 * @brief cancel all execute task of owner, see #ZFThreadExecuteCancel
 */
ZFMETHOD_FUNC_DECLARE_1(void, ZFThreadExecuteCancelByOwner,
                        ZFMP_IN(ZFObject *, owner))

/**
 * @brief cancel all execute task of runnable, ignoring the userData, see #ZFThreadExecuteCancel
 */
ZFMETHOD_FUNC_DECLARE_1(void, ZFThreadExecuteCancelAll,
                        ZFMP_IN(const ZFListener &, runnable))

/**
 * @brief wait thread task until done
 *
 * return immediately if the task is running in main thread
 */
ZFMETHOD_FUNC_DECLARE_1(void, ZFThreadExecuteWait,
                        ZFMP_IN(zfidentity, taskId))
/**
 * @brief wait thread task until done
 *
 * return ture if wait success,
 * or false if wait timeout or no such task\n
 * return false immediately if the task is running in main thread
 */
ZFMETHOD_FUNC_DECLARE_2(zfbool, ZFThreadExecuteWait,
                        ZFMP_IN(zfidentity, taskId),
                        ZFMP_IN(zftimet, miliSecs))

/**
 * @brief attach observer to the tasks started by ZFThreadExecuteXxx, see #ZFObject::observerNotify
 *
 * do nothing if task not exist, auto remove after task finished \n
 * \n
 * list of supported events:
 * -  #ZFThread::EventThreadOnStart
 * -  #ZFThread::EventThreadOnStop
 * -  #ZFThread::EventThreadOnCancel
 */
ZFMETHOD_FUNC_DECLARE_3(void, ZFThreadExecuteObserverAdd,
                        ZFMP_IN(zfidentity, taskId),
                        ZFMP_IN(zfidentity, eventId),
                        ZFMP_IN(const ZFListener &, callback))
/**
 * @brief see #ZFThreadExecuteObserverAdd
 */
ZFMETHOD_FUNC_DECLARE_3(void, ZFThreadExecuteObserverRemove,
                        ZFMP_IN(zfidentity, taskId),
                        ZFMP_IN(zfidentity, eventId),
                        ZFMP_IN(const ZFListener &, callback))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFThread_execute_h_

