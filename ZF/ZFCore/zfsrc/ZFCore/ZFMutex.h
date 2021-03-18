/**
 * @file ZFMutex.h
 * @brief mutex utility
 */

#ifndef _ZFI_ZFMutex_h_
#define _ZFI_ZFMutex_h_

#include "ZFObject.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/**
 * @brief mutex utility
 */
zfclass ZF_ENV_EXPORT ZFMutex : zfextends ZFObject
{
    ZFOBJECT_DECLARE(ZFMutex, ZFObject)

public:
    /**
     * @brief wait until successfully acquired the lock
     *
     * wait until none of other threads hold the lock, then acquire the lock\n
     * you must call mutexUnlock to release lock,
     * and mutexLock and mutexUnlock must be paired
     * @see mutexTryLock, mutexUnlock
     */
    virtual inline void mutexLock(void)
    {
        this->_ZFP_ZFObjectLock();
    }
    /**
     * @brief try to lock, or return false immediately if failed
     * @note if mutexTryLock success, you should unlock it somewhere,
     *   otherwise, there's no need to unlock
     * @see mutexLock, mutexUnlock
     */
    virtual inline zfbool mutexTryLock(void)
    {
        return this->_ZFP_ZFObjectTryLock();
    }
    /**
     * @brief release the lock, must be paired with mutexLock or mutexTryLock,
     *   and must be called in the same thread where mutexLock or mutexTryLock called
     */
    virtual inline void mutexUnlock(void)
    {
        this->_ZFP_ZFObjectUnlock();
    }
};

ZF_NAMESPACE_GLOBAL_END
#endif // #ifndef _ZFI_ZFMutex_h_

