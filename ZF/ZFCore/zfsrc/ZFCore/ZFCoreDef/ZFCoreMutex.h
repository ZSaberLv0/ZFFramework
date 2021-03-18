/**
 * @file ZFCoreMutex.h
 * @brief core mutex
 */

#ifndef _ZFI_ZFCoreMutex_h_
#define _ZFI_ZFCoreMutex_h_

#include "ZFCoreTypeDef.h"
ZF_NAMESPACE_GLOBAL_BEGIN

/** @brief mutex impl */
typedef void (*ZFCoreMutexImplCallbackLock)(ZF_IN void *token);
/** @brief mutex impl */
typedef void (*ZFCoreMutexImplCallbackUnlock)(ZF_IN void *token);

extern ZF_ENV_EXPORT void *_ZFP_ZFCoreMutexImplObject;
extern ZF_ENV_EXPORT ZFCoreMutexImplCallbackLock _ZFP_ZFCoreMutexImplLock;
extern ZF_ENV_EXPORT ZFCoreMutexImplCallbackUnlock _ZFP_ZFCoreMutexImplUnlock;

/**
 * @brief #zfCoreMutexLock's implementation, change with caution
 */
inline void ZFCoreMutexImplSet(ZF_IN void *implObject,
                               ZF_IN ZFCoreMutexImplCallbackLock implLock,
                               ZF_IN ZFCoreMutexImplCallbackUnlock implUnlock)
{
    _ZFP_ZFCoreMutexImplObject = implObject;
    _ZFP_ZFCoreMutexImplLock = implLock;
    _ZFP_ZFCoreMutexImplUnlock = implUnlock;
}

/** @brief see #ZFCoreMutexImplSet */
inline void *ZFCoreMutexImplGetObject(void) {return _ZFP_ZFCoreMutexImplObject;}
/** @brief see #ZFCoreMutexImplSet */
inline ZFCoreMutexImplCallbackLock ZFCoreMutexImplGetLock(void) {return _ZFP_ZFCoreMutexImplLock;}
/** @brief see #ZFCoreMutexImplSet */
inline ZFCoreMutexImplCallbackUnlock ZFCoreMutexImplGetUnlock(void) {return _ZFP_ZFCoreMutexImplUnlock;}

/** @brief see #ZFCoreMutexImplSet */
inline zfbool ZFCoreMutexImplAvailable(void) {return (_ZFP_ZFCoreMutexImplObject != zfnull);}

// ============================================================
/**
 * @brief internal use only
 *
 * you may change it at run time by changing #ZFCoreMutexImplSet
 */
#define zfCoreMutexLock() \
    do { \
        if(_ZFP_ZFCoreMutexImplObject) \
        { \
            _ZFP_ZFCoreMutexImplLock(_ZFP_ZFCoreMutexImplObject); \
        } \
    } while(zffalse)
/** @brief see #zfCoreMutexLock */
#define zfCoreMutexUnlock() \
    do { \
        if(_ZFP_ZFCoreMutexImplObject) \
        { \
            _ZFP_ZFCoreMutexImplUnlock(_ZFP_ZFCoreMutexImplObject); \
        } \
    } while(zffalse)

/**
 * @brief see #zfCoreMutexLocker
 */
zfclassLikePOD ZF_ENV_EXPORT zfCoreMutexLockerHolder
{
public:
    /** @cond ZFPrivateDoc */
    zfCoreMutexLockerHolder(void)
    {
        zfCoreMutexLock();
    }
    ~zfCoreMutexLockerHolder(void)
    {
        zfCoreMutexUnlock();
    }
    /** @endcond */
};
/**
 * @brief util method to lock current block
 *
 * usage:
 * @code
 *   {
 *       zfCoreMutexLocker(); // lock
 *       if(...) return ; // safe to return
 *   } // unlock after block
 * @endcode
 *
 * you may also use the helper #zfCoreMutexLockerHolder
 * @code
 *   Type var = (zfCoreMutexLockerHolder(), yourFuncSynced());
 * @endcode
 */
#define zfCoreMutexLocker() zfCoreMutexLockerHolder _ZFP_ZFCoreMutexLocker_hold

#if 0
    #undef zfCoreMutexLock
    #define zfCoreMutexLock()
    #undef zfCoreMutexUnlock
    #define zfCoreMutexUnlock()
    #undef zfCoreMutexLocker
    #define zfCoreMutexLocker()
#endif

ZF_NAMESPACE_GLOBAL_END

#endif // #ifndef _ZFI_ZFCoreMutex_h_

