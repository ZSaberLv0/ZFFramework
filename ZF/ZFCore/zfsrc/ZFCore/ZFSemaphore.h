/**
 * @file ZFSemaphore.h
 * @brief semaphore utility
 */

#ifndef _ZFI_ZFSemaphore_h_
#define _ZFI_ZFSemaphore_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

zfclassFwd _ZFP_ZFSemaphorePrivate;
/**
 * @brief semaphore utility
 */
zfclass ZF_ENV_EXPORT ZFSemaphore : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFSemaphore, ZFObject)

protected:
    zfoverride
    virtual void objectOnInit(void);
    zfoverride
    virtual void objectOnDealloc(void);

public:
    /** @brief for internal use only */
    virtual void *nativeSemaphore(void);

public:
    /**
     * @brief must be locked before use
     */
    virtual void semaphoreLock(void);
    /**
     * @brief must be unlocked after use
     */
    virtual void semaphoreUnlock(void);

    /**
     * @brief notify one waiter
     *
     * which waiter is notified, is decided by system\n
     * if there's no waiter, do nothing (this signal is ignored),
     * and new waiter need to wait new signal
     * @note must be wrapped by #semaphoreLock and #semaphoreUnlock,
     *   or use semaphoreSignalLocked for short
     */
    virtual void semaphoreSignal(void);
    /** @brief see #semaphoreSignal */
    virtual void semaphoreSignalLocked(void);

    /**
     * @brief notify all waiters
     *
     * @note must be wrapped by #semaphoreLock and #semaphoreUnlock,
     *   or use semaphoreBroadcastLocked for short
     */
    virtual void semaphoreBroadcast(void);
    /** @brief see #semaphoreSignal */
    virtual void semaphoreBroadcastLocked(void);

    /**
     * @brief wait until signal
     *
     * @note must be wrapped by #semaphoreLock and #semaphoreUnlock,
     *   or use semaphoreWaitLocked for short
     */
    virtual void semaphoreWait(void);
    /** @brief see #semaphoreSignal */
    virtual void semaphoreWaitLocked(void);

    /**
     * @brief wait until signal or timeout
     *
     * @note must be wrapped by #semaphoreLock and #semaphoreUnlock,
     *   or use semaphoreWaitLocked for short
     */
    virtual zfbool semaphoreWait(ZF_IN zftimet miliSecs);
    /** @brief see #semaphoreSignal */
    virtual zfbool semaphoreWaitLocked(ZF_IN zftimet miliSecs);

private:
    _ZFP_ZFSemaphorePrivate *d;
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFSemaphore_h_

