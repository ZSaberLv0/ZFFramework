/**
 * @file ZFThread_observerNotifyInMainThread.h
 * @brief thread utility
 */

#ifndef _ZFI_ZFThread_observerNotifyInMainThread_h_
#define _ZFI_ZFThread_observerNotifyInMainThread_h_

#include "ZFThread.h"
ZF_NAMESPACE_GLOBAL_BEGIN

// ============================================================
/**
 * @brief notify observer in main thread, see #ZFObject::observerNotify
 *
 * the observer would be scheduled to run in future
 * even if current thread is main thread\n
 * the task can be canceled by #ZFObserverNotifyInMainThreadCancel if not actually started
 * @note obj, customSender, params would be retained during running
 */
ZFMETHOD_FUNC_DECLARE_5(zfidentity, ZFObserverNotifyInMainThreadWithCustomSender,
                        ZFMP_IN(ZFObject *, customSender),
                        ZFMP_IN(ZFObject *, obj),
                        ZFMP_IN(zfidentity, eventId),
                        ZFMP_IN_OPT(ZFObject *, param0, zfnull),
                        ZFMP_IN_OPT(ZFObject *, param1, zfnull))
/** @brief see #ZFObserverNotifyInMainThreadWithCustomSender */
ZFMETHOD_FUNC_INLINE_DECLARE_4(zfidentity, ZFObserverNotifyInMainThread,
                               ZFMP_IN(ZFObject *, obj),
                               ZFMP_IN(zfidentity, eventId),
                               ZFMP_IN_OPT(ZFObject *, param0, zfnull),
                               ZFMP_IN_OPT(ZFObject *, param1, zfnull))
{
    return ZFObserverNotifyInMainThreadWithCustomSender(obj, obj, eventId, param0, param1);
}
/** @brief see #ZFObserverNotifyInMainThreadWithCustomSender */
ZFMETHOD_FUNC_DECLARE_1(void, ZFObserverNotifyInMainThreadCancel,
                        ZFMP_IN(zfidentity, taskId))

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFThread_observerNotifyInMainThread_h_

